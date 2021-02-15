#pragma once
#include <SFML/Graphics.hpp>
#include "Peer.h"

class Game;
/*
	Networked game instance
	contains methods used for multiplayer gameplay
*/
class NetGame {
public:
	NetGame(Game* game);
	~NetGame();
	void render(sf::RenderTarget& target);
	void update(sf::RenderWindow& window, float dt);
	void handleInput(float dt);
	void handleEvents(sf::Event& ev);

private:
	void loadResources();

private:
	Peer _peer;
};