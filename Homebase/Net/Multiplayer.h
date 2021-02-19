#pragma once
#include "NetworkedGame.h"

class Game;

struct Host {
	sf::IpAddress address;
	uint16_t port;
	Peer_t id;
};

/*
	Class responsible for sending and receiving messages/data
*/
class Multiplayer : public Net{
public:
	Multiplayer(Game *game, bool host);
	~Multiplayer();
	void render(sf::RenderTarget& target);
	void update(Peer_t id, float dt);
	void handleInput(Peer_t id, float dt);
	void handleEvents(Peer_t id, sf::Event& ev);

	void receivePackets()override;
	void sendPacket(sf::Packet& packet, const EndPoint& endPoint)override;

	void sendBroadcastLAN(uint16_t portToSendBroadcast);

	sf::Socket::Status bindSocket(EndPoint& endPoint);
	bool isHost()const;

	NetworkedGame& game() { return _game; }
	void handleUserAdded(Peer_t id, const EndPoint &endPoint);

private:
	void handleReceivedPeerState(sf::Packet& packet);
	void setup();

	NetworkedGame _game;
	std::unique_ptr<Host> _host = nullptr;
};