#pragma once
#include "NetworkedGame.h"

class Game;

struct Host {
	sf::IpAddress address;
	uint16_t port;
	Peer_t id;
};

class Peer;

//TODO: Sent packet to all peers notifing them a new player has joined

/*
	Class responsible for sending and receiving messages/data
*/
class Multiplayer : public Net{
public:
	Multiplayer(Peer *peer, Game *game, bool host);
	~Multiplayer();
	void render(sf::RenderTarget& target);
	void update(Peer_t id, float dt);
	void handleInput(Peer_t id, float dt);
	void handleEvents(Peer_t id, sf::Event& ev);
	/*
		Starts the listening thread
	*/
	void listen();

	void run();
	void receivePackets()override;
	void sendState();
	void sendPacket(sf::Packet& packet, const EndPoint& endPoint)override;
	void sendBroadcastLAN(uint16_t portToSendBroadcast);
	/*
		Sends out a packet of the new connected player to all connected peers
	*/
	void notifyNewPlayerConnected(Peer_t newPlayerId, const EndPoint &endPoint);

	sf::Socket::Status bindSocket(const EndPoint& endPoint);

	void handleUserAdded(Peer_t id, const EndPoint &endPoint);
	bool isHost()const;

	Player& getPlayer(Peer_t id) { return _game.getPlayerById(id); }

private:
	void handleReceivedStateData(sf::Packet &packet);
	void handleReceivedPeerState(sf::Packet& packet);
	void handleReceivedBroadcast(const EndPoint& endPoint);
	void handleReceivedNewPlayerConnected(sf::Packet &packet);
	void handleAckNewPlayer(sf::Packet& packet, const EndPoint &endPoint);
	void handleBroadcastResponse(const EndPoint& endPoint);
	void handleConnectionRequest(const EndPoint& endPoint);
	void handleConnectionResponse(sf::Packet &packet, const EndPoint& endPoint);

	void attemptConnect();
	void setup();

	NetworkedGame _game;
	Peer* _peer = nullptr;
	std::unique_ptr<Host> _host = nullptr;
	std::unique_ptr<sf::Thread> _listeningThread = nullptr;

	sf::Clock _tickRate;
	bool _listening = false;

public:
	constexpr static uint16_t hostPort = 45322;
};