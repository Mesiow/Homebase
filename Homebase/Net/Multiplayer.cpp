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
}

Multiplayer::~Multiplayer()
{
	socket.unbind();
}

void Multiplayer::render(sf::RenderTarget& target)
{
	_game.render(target);
}

void Multiplayer::update(Peer_t id, float dt)
{
	receivePackets();

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

void Multiplayer::receivePackets()
{
	sf::Packet packet;
	sf::IpAddress remoteAddress;
	uint16_t remotePort;
	if (socket.receive(packet, remoteAddress, remotePort) == sf::Socket::Done) {
		ePacket id;
		packet >> id;

		switch (id) {
			case ePacket::PeerState: handleReceivedPeerState(packet); break;
			case ePacket::PeerShot: break;
			case ePacket::PeerDied:  break;
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
	broadcastPacket << eCommandToHost::Broadcast;
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

void Multiplayer::handleReceivedPeerState(sf::Packet& packet)
{

}

void Multiplayer::handleUserAdded(Peer_t id, const EndPoint &endPoint)
{
	_game.add(id, endPoint);
}

void Multiplayer::setup()
{
	
}
