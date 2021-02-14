#pragma once
#include <Game/ResourceManager/ResourceManager.h>
#include <Thor/Animations.hpp>
#include "../Entities/Player.h"
#include "../Entities/Starfield.h"
#include "../Entities/Planet.h"
#include <vector>

class Game;

class MainGame {
public:
	MainGame(Game *game);
	~MainGame();
	void render(sf::RenderTarget& target);
	void update(sf::RenderWindow& window, float dt);
	void handleInput(float dt);
	void handleEvents(sf::Event& ev);

private:
	void loadResources();
	void setupEntities();

private:
	Game* _game;

	Player* _player;
	Starfield _starfield;

	int _systemSize;
	std::vector<Planet> _planets;
};