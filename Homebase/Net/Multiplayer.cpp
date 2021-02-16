#include "Multiplayer.h"
#include <Game/Game.h>
#include <iostream>

Multiplayer::Multiplayer()
{
	setup();
}

Multiplayer::~Multiplayer()
{
	socket.unbind();
}

void Multiplayer::run()
{
	receivePackets();
}

void Multiplayer::render(sf::RenderTarget& target)
{
	_game.render(target);
}

void Multiplayer::update(Game *game, Peer_t id, float dt)
{
	_game.players[id]->update(game->getWindowHandle(), dt);
}

void Multiplayer::handleInput(Peer_t id, float dt)
{
	_game.getPlayerById(id).handleInput(dt);
}

void Multiplayer::handleEvents(Peer_t id, sf::Event& ev)
{
	_game.getPlayerById(id).handleEvents(ev);
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

sf::Socket::Status Multiplayer::bindSocket(EndPoint& endPoint)
{
	auto& ep = endPoint;
	if (socket.bind(ep.port, ep.address) == sf::Socket::Done) {
		socket.setBlocking(false);
		return sf::Socket::Done;
	}
	return sf::Socket::Error;
}

void Multiplayer::handleReceivedPeerState(sf::Packet& packet)
{

}

void Multiplayer::handleUserAdded(Peer_t id)
{
	_game.add(id);
}

void Multiplayer::setup()
{
	_game.zero();
}
