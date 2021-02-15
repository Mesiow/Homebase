#include "Peer.h"

Peer::Peer()
{
	initSocket();
}

Peer::~Peer()
{
}

void Peer::initSocket()
{
	EndPoint ep;
	ep.address = sf::IpAddress::LocalHost;

	uint16_t port = thor::random(1024, 49151);
	ep.port = port;

	_multiplayer.setSocket(ep);
}
