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

private:
	void initSocket();

	Peer_t _id;
	Game* _game;
	Multiplayer _multiplayer;
};