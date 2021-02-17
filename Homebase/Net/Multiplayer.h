#pragma once
#include "NetworkedGame.h"

class Game;

/*
	Class responsible for sending and receiving messages/data
*/
class Multiplayer : public Net{
public:
	Multiplayer(Game *game);
	~Multiplayer();
	void run();
	void render(sf::RenderTarget& target);
	void update(Peer_t id, float dt);
	void handleInput(Peer_t id, float dt);
	void handleEvents(Peer_t id, sf::Event& ev);

	void receivePackets()override;
	void sendPacket(sf::Packet& packet, const EndPoint& endPoint)override;

	sf::Socket::Status bindSocket(EndPoint& endPoint);

	NetworkedGame& game() { return _game; }
	void handleUserAdded(Peer_t id);

private:
	void handleReceivedPeerState(sf::Packet& packet);
	void setup();

	NetworkedGame _game;
};