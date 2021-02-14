#pragma once
#include <Game/Net/Net.h>
#include "Packets.h"

/*
	Main class responsible for sending and receiving messages
*/
class Multiplayer : public Net{
public:
	void receivePackets()override;
	void sendPacket(sf::Packet& packet, const EndPoint& endPoint)override;
};