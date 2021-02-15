#include "Multiplayer.h"
#include <iostream>

Multiplayer::Multiplayer()
{
	setup();
}

Multiplayer::~Multiplayer()
{
	socket.unbind();
}

void Multiplayer::receivePackets()
{

}

void Multiplayer::sendPacket(sf::Packet& packet, const EndPoint& endPoint)
{
	socket.send(packet, endPoint.address, endPoint.port);
}

void Multiplayer::setSocket(EndPoint& endPoint)
{
	auto& ep = endPoint;
	if (socket.bind(ep.port, ep.address) == sf::Socket::Done) {
		std::cout << "Socket bound to " << ep.address.toString() << ":"
			<< ep.port << std::endl;
	}
}

void Multiplayer::setup()
{
	
}
