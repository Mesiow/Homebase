#include "Peer.h"
#include <iostream>

Peer::Peer(Game *game, bool host)
	:_game(game), _multiplayer(this, game, host)
{
	_host = host;
	_id = 0;
	initSocket();
}

Peer::~Peer()
{

}

void Peer::render(sf::RenderTarget& target)
{
	_multiplayer.render(target);
}

void Peer::update(float dt)
{
	_multiplayer.update(_id, dt);
}

void Peer::handleInput(float dt)
{
	_multiplayer.handleInput(_id, dt);
}

void Peer::handleEvents(sf::Event& ev)
{
	_multiplayer.handleEvents(_id, ev);
}

void Peer::initSocket()
{
	EndPoint ep;
	ep.address = sf::IpAddress::getLocalAddress();
	if (!_multiplayer.isHost()) {
		uint16_t port = thor::random(1024, 44151);
		ep.port = port;
	}
	else {
		ep.port = _multiplayer.hostPort;
	}

	sf::Socket::Status status = _multiplayer.bindSocket(ep);
	if (status == sf::Socket::Done) {
		/*
			Start listening thread to listen to incoming packets
		*/
		_multiplayer.listen();

		std::cout << "Socket bound to "
			<< ep.address.toString() << ":"
			<< ep.port << std::endl;

		if (_multiplayer.isHost()) {
			_id = 0;
			/*
				Add our user locally
			*/
			_multiplayer.handleUserAdded(_id, ep);
		}
		else {
			_multiplayer.sendBroadcastLAN(_multiplayer.hostPort);
		}
	}
	else if (status == sf::Socket::Error) {
		std::cerr << "Socket failed to bind\n";
	}
}
