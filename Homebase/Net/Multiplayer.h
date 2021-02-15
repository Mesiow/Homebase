#pragma once
#include <Game/Net/Net.h>
#include "Packets.h"
#include "../Entities/Player.h"
#include <memory>
#include <array>

#define MAX_CONNECTIONS 4

/*
	Main class responsible for sending and receiving messages and
	containing multiplayer entities
*/
class Multiplayer : public Net{
public:
	Multiplayer();
	~Multiplayer();
	void receivePackets()override;
	void sendPacket(sf::Packet& packet, const EndPoint& endPoint)override;
	void setSocket(EndPoint& endPoint);

private:
	void setup();

private:
	std::array<std::unique_ptr<Player>, MAX_CONNECTIONS> _players;
};