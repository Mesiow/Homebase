#pragma once
#include <Thor/Math.hpp>
#include "Multiplayer.h"

class Game;

class Peer {
public:
	Peer(Game *game, bool host = false);
	~Peer();
	void render(sf::RenderTarget& target);
	void update(float dt);
	void handleInput(float dt);
	void handleEvents(sf::Event& ev);

	void setID(Peer_t id) { _id = id; }
	bool isHost()const { return _host; }
	Peer_t getID()const { return _id; }

	Player& getPlayer() { return _multiplayer.getPlayer(_id); }

private:
	void initSocket();

	Peer_t _id;
	Game* _game;
	Multiplayer _multiplayer;

	bool _host;
};