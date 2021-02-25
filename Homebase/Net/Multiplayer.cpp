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
		_host->id = 0;
	}

	_listeningThread = std::make_unique<sf::Thread>(&Multiplayer::run, this);
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
	sf::Packet packet;
	sf::IpAddress remoteAddress;
	uint16_t remotePort;
	if (socket.receive(packet, remoteAddress, remotePort) == sf::Socket::Done) {

		sf::Packet copy = packet;
		unsigned int identifier = 0;
		copy >> identifier;
		copy.clear();

		bool isCommand = false;
		if (identifier == commandIdentifier) {
			isCommand = true;
			packet >> identifier; //Extract value from original packet to continue reading properly below
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
		else { //
			if (isCommand) { //Command data for the peer
				eCommandToPeer id;
				packet >> id;
				switch (id) {
					case eCommandToPeer::BroadcastResponse: handleBroadcastResponse({ remoteAddress, remotePort }); break;
					case eCommandToPeer::ConnectResponse: handleConnectionResponse(packet, { remoteAddress, remotePort }); break;
					case eCommandToPeer::NewPlayerConnected: handleReceivedNewPlayerConnected(packet); break;
					case eCommandToPeer::AckPlayer: handleAckNewPlayer(packet, { remoteAddress, remotePort }); break;
				}
			}
			else { //Check for regular state data packets
				handleReceivedStateData(packet);
			}
		}
	}
}

void Multiplayer::run()
{
	_tickRate.restart();
	while (_listening) {
		receivePackets();
		/* Send state every 20ms */
		if (_tickRate.getElapsedTime().asSeconds() >= 0.20f) {
			sendState();
		}
	}
}

void Multiplayer::sendState()
{
	//Send Peer State
	
	sf::Packet statePacket;
	statePacket << ePacket::PeerState;

	if (isHost()) {
		const Peer_t our_id = _peer->getID();
		const auto& player = _game.getPlayerById(our_id);

		const sf::Vector2f peerPosition = player.getPosition();
		const float peerRotation = player.getRotation();

		statePacket << our_id << peerPosition.x << peerPosition.y << peerRotation;

		//Have host send data about its' state to all connected peers
		for (size_t i = 0; i < MAX_CONNECTIONS; ++i) {
			if (_game.isConnected(i)) {
				const EndPoint& ep = _game.getEndPoint(i);

				if (ep.port != _game.getEndPoint(our_id).port) { /* Make sure data will not being sent to ourself as the host */
					sendPacket(statePacket, ep);
				}
			}
		}
	}
	else {
		const Peer_t our_id = _peer->getID();
		if (_game.isConnected(our_id)) {
			const auto& player = _game.getPlayerById(our_id);

			const sf::Vector2f peerPosition = player.getPosition();
			const float peerRotation = player.getRotation();

		
			statePacket << our_id << peerPosition.x << peerPosition.y << peerRotation;
			
			//Send data Peer to peer
			for (size_t i = 0; i < MAX_CONNECTIONS; ++i) {
				if (_game.isConnected(i)) {
					const EndPoint& ep = _game.getEndPoint(i);

					if (ep.port != _game.getEndPoint(our_id).port) { /* Make sure data will not being sent to ourself as the peer */
						sendPacket(statePacket, ep);
					}
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

void Multiplayer::notifyNewPlayerConnected(Peer_t newPlayerId, const EndPoint& endPoint)
{
	const auto& connected = _game.getAlreadyConnectedSlots();
	if (connected.size() > 2) { /* If greater than 2 then a third player has joined so notify the other peer */
		sf::Packet newPlayerPacket;
		newPlayerPacket << commandIdentifier << eCommandToPeer::NewPlayerConnected << newPlayerId << endPoint.address.toInteger() << endPoint.port;

		for (size_t i = 0; i < connected.size(); ++i) {
			if (_game.isConnected(connected[i])) {
				// We only want to send this data about the new player to the other peer who is connected,
				// Not the host or ourselves
				if (connected[i] != _host->id && connected[i] != newPlayerId) {
					const EndPoint& ep = _game.getEndPoint(connected[i]);
					std::cout << "Slot " << connected[i] << " is connected at " << ep.address.toString() << ":" << ep.port << std::endl;
					sendPacket(newPlayerPacket, ep);
				}
			}	
		}
	}
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
	Peer_t sender_id;
	float x, y, rotation;
	if (packet >> sender_id) {
		packet >> x >> y >> rotation;
		_game.getPlayerById(sender_id).setPosition(sf::Vector2f(x, y));
		_game.getPlayerById(sender_id).setRotation(rotation);
	}
}

void Multiplayer::handleReceivedBroadcast(const EndPoint& endPoint)
{
	std::cout << "Broadcast received from: " << endPoint.address << ":" << endPoint.port << std::endl;
	sf::Packet broadcastResponse;
	broadcastResponse << commandIdentifier << eCommandToPeer::BroadcastResponse;
	sendPacket(broadcastResponse, endPoint);
}

void Multiplayer::handleReceivedNewPlayerConnected(sf::Packet& packet)
{
	Peer_t newPlayerId;
	if (packet >> newPlayerId) {
		std::cout << "Received new player connected slot #" << (int)newPlayerId << std::endl;
		uint32_t address;
		uint16_t port;
		packet >> address >> port;

		EndPoint ep;
		ep.address = sf::IpAddress(address);
		ep.port = port;

		_game.add(newPlayerId, ep);

		sf::Packet p;
		p << commandIdentifier << eCommandToPeer::AckPlayer << _peer->getID();
		std::cout << "Sending ack from peer " << (int)_peer->getID() << " to " << ep.address.toString() << ":" << ep.port << std::endl;
		sendPacket(p, ep);
	}
}

void Multiplayer::handleAckNewPlayer(sf::Packet& packet, const EndPoint& endPoint)
{
	Peer_t id;
	if (packet >> id) {
		std::cout << "Acknowledge peer " << (int)id << std::endl;
		_game.add(id, endPoint);
	}
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
		/* Add peer to the networked game for the host */
		_game.add(slot, endPoint);

		//Notify already connected peers of new player
		notifyNewPlayerConnected(slot, endPoint);
		
		/* Send the hosts' id along with the peers new slot id */
		sf::Packet connectResponsePacket;

		const auto &connected = _game.getAlreadyConnectedSlots();
		std::cout << "Host id: " <<(int)_host->id << std::endl;
		connectResponsePacket << commandIdentifier << eCommandToPeer::ConnectResponse << _host->id << (Peer_t)slot;
		std::cout << "Connected size: " << connected.size() << std::endl;

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
			_host->id = host_id;
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
