#include "Multiplayer.h"
#include <Game/Game.h>
#include <iostream>

Multiplayer::Multiplayer(Game *game, bool host)
{
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
			bool isCommand = false;
			if (packet.getDataSize() == commandSizeInBytes) {
				packet >> isCommand;
			}

			/*
				If we are the host check if we have received a specific packet intented for just the host
			*/

			if (_host) {
				if (isCommand) { //Check for command packets for the host
					eCommandToHost id;
					packet >> id;
					switch (id) {
					case eCommandToHost::Broadcast: handleReceivedBroadcast({ remoteAddress, remotePort }); break;
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
	bool isCommand = true;
	broadcastPacket << isCommand << eCommandToHost::Broadcast;
	socket.send(broadcastPacket, sf::IpAddress::Broadcast, portToSendBroadcast);
}

sf::Socket::Status Multiplayer::bindSocket(EndPoint& endPoint)
{
	auto& ep = endPoint;
	if (socket.bind(ep.port, ep.address) == sf::Socket::Done) {
		if (_host) {
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
	return (_host != nullptr);
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
	bool isCommand = true;
	broadcastResponse << isCommand << eCommandToPeer::BroadcastResponse;
	sendPacket(broadcastResponse, endPoint);
}

void Multiplayer::handleBroadcastResponse(const EndPoint& endPoint)
{
	std::cout<<"Broadcast response from: " << endPoint.address << ":" << endPoint.port << std::endl;
}

void Multiplayer::handleUserAdded(Peer_t id, const EndPoint &endPoint)
{
	_game.add(id, endPoint);
}

void Multiplayer::setup()
{
	
}
