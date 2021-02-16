#pragma once
#include <SFML/Graphics.hpp>
#include "Peer.h"
#include "../Entities/Starfield.h"

class Game;
/*
	Networked game instance,
	has a peer that renders a multiplayer game instance
*/
class MainGame {
public:
	MainGame(Game* game);
	~MainGame();
	void render(sf::RenderTarget& target);
	void update(sf::RenderWindow& window, float dt);
	void handleInput(float dt);
	void handleEvents(sf::Event& ev);

private:
	void loadResources();

	std::unique_ptr<Peer> _peer;
	Starfield _starfield;
	//
	//	int _systemSize;
	//	std::vector<Planet> _planets;
};