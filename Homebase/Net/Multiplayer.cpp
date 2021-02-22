#include "Multiplayer.h"
#include <Game/Game.h>
#include "Peer.h"
#include <iostream>

Multiplayer::Multiplayer(Peer *peer, Game *game, bool host)
{
	_peer = peer;

	_game.setup();
	_game.game = game;

	if (host) {
		_host = std::make_unique<Host>();
	}

	_listeningThread = std::make_unique<sf::Thread>(&Multiplayer::receivePackets, this);
}

Multiplayer::~Multiplayer()
{
	socket.unbind();
	_listeningThread->terminate();
}

void Multiplayer::render(sf::RenderTarget& target)
{
	_game.render(target);
}

void Multiplayer::update(Peer_t id, float dt)
{
	_game.update(id, dt);
}

void Multiplayer::handleInput(Peer_t id, float dt)
{
	_game.handleInput(id, dt);
}

void Multiplayer::handleEvents(Peer_t id, sf::Event& ev)
{
	_game.handleEvents(id, ev);
}

void Multiplayer::listen()
{
	_listening = true;
	_listeningThread->launch();
}

void Multiplayer::receivePackets()
{
	while (_listening) {
		sf::Packet packet;
		sf::IpAddress remoteAddress;
		uint16_t remotePort;
		if (socket.receive(packet, remoteAddress, remotePort) == sf::Socket::Done) {

			sf::Packet copy = packet;
			unsigned int identifier;
			copy >> identifier;
			copy.clear();

			bool isCommand = false;
			if (identifier == commandIdentifier) {
				isCommand = true;
				packet >> identifier; //Extract value from original packet to continue
			}

			/*
				If we are the host check if we have received a specific packet intented for just the host
			*/

			if (isHost()) {
				if (isCommand) { //Check for command packets for the host
					eCommandToHost id;
					packet >> id;
					switch (id) {
						case eCommandToHost::Broadcast: handleReceivedBroadcast({ remoteAddress, remotePort }); break;
						case eCommandToHost::ConnectRequest: handleConnectionRequest({ remoteAddress, remotePort }); break;
					}
				}
				else { //Check for regular state data packets
					handleReceivedStateData(packet);
				}
			}
			else {
				if (isCommand) { //Command data for the peer
					eCommandToPeer id;
					packet >> id;
					switch (id) {
						case eCommandToPeer::BroadcastResponse: handleBroadcastResponse({ remoteAddress, remotePort }); break;
						case eCommandToPeer::ConnectResponse: handleConnectionResponse(packet, { remoteAddress, remotePort }); break;
					}
				}
				else { //Check for regular state data packets
					handleReceivedStateData(packet);
				}
			}
		}
	}
}

void Multiplayer::sendPacket(sf::Packet& packet, const EndPoint& endPoint)
{
	socket.send(packet, endPoint.address, endPoint.port);
}

void Multiplayer::sendBroadcastLAN(uint16_t portToSendBroadcast)
{
	sf::Packet broadcastPacket;
	broadcastPacket << commandIdentifier << eCommandToHost::Broadcast;
	socket.send(broadcastPacket, sf::IpAddress::Broadcast, portToSendBroadcast);
}

sf::Socket::Status Multiplayer::bindSocket(const EndPoint& endPoint)
{
	auto& ep = endPoint;

	/* Initialize the address and port for this user */
	this->address = ep.address;
	this->port = ep.port;

	if (socket.bind(ep.port, ep.address) == sf::Socket::Done) {
		if (isHost()) {
			_host->address = ep.address;
			_host->port = ep.port;
		}
		socket.setBlocking(false);
		return sf::Socket::Done;
	}
	return sf::Socket::Error;
}

bool Multiplayer::isHost() const
{
	return _peer->isHost();
}

void Multiplayer::handleReceivedStateData(sf::Packet& packet)
{
	ePacket id;
	packet >> id;
	switch (id) {
		case ePacket::PeerState: handleReceivedPeerState(packet); break;
		case ePacket::PeerShot: break;
		case ePacket::PeerDied:  break;
	}
}

void Multiplayer::handleReceivedPeerState(sf::Packet& packet)
{

}

void Multiplayer::handleReceivedBroadcast(const EndPoint& endPoint)
{
	std::cout << "Broadcast received from: " << endPoint.address << ":" << endPoint.port << std::endl;
	sf::Packet broadcastResponse;
	broadcastResponse << commandIdentifier << eCommandToPeer::BroadcastResponse;
	sendPacket(broadcastResponse, endPoint);
}

void Multiplayer::handleBroadcastResponse(const EndPoint& endPoint)
{
	_host = std::make_unique<Host>();
	_host->address = endPoint.address;
	_host->port = endPoint.port;

	std::cout << "Host found: " << _host->address.toString() << ":" << _host->port << std::endl;

	attemptConnect();
}

void Multiplayer::handleConnectionRequest(const EndPoint& endPoint)
{
	std::cout << "Connection request from: " << endPoint.address << ":" << endPoint.port << std::endl;
	/*
		1. Make sure there is room for another connection, if there is send a response back with the peer's new id
	*/
	int slot = _game.getFreeSlot();
	if (slot) {
		/* Add peer to the networked game for the host*/
		_game.add(slot, endPoint);
		
		/* Send the hosts' id along with the peers new slot id */
		sf::Packet connectResponsePacket;
		connectResponsePacket << commandIdentifier << eCommandToPeer::ConnectResponse << 
			(Peer_t)_peer->getID() << (Peer_t)slot;

		sendPacket(connectResponsePacket, endPoint);
	}
	else { //No slots available
		std::cout << "No slots remaining\n";
	}
}

void Multiplayer::handleConnectionResponse(sf::Packet &packet, const EndPoint& endPoint)
{
	if (_host->address == endPoint.address && _host->port == endPoint.port) {
		Peer_t host_id;
		Peer_t slot_id;
		if (packet >> host_id >> slot_id) {
			_peer->setID(slot_id);
			std::cout << "Connection successful\n";
			std::cout << "Slot id: " << (int)slot_id << std::endl;

			/* Add the host and us to the networked game */
			_game.add(host_id, { _host->address, _host->port });
			_game.add(_peer->getID(), { this->address, this->port });
		}
	}
}

void Multiplayer::handleUserAdded(Peer_t id, const EndPoint &endPoint)
{
	_game.add(id, endPoint);
}

void Multiplayer::attemptConnect()
{
	sf::Packet connectReqPacket;
	connectReqPacket << commandIdentifier << eCommandToHost::ConnectRequest;
	sendPacket(connectReqPacket, { _host->address, _host->port });
}

void Multiplayer::setup()
{
	
}
