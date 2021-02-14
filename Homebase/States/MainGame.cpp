#include "MainGame.h"
#include <Game/Game.h>

MainGame::MainGame(Game *game)
{
	_game = game;

	loadResources();
	setupEntities();
}

MainGame::~MainGame()
{
	delete _player;
}

void MainGame::render(sf::RenderTarget& target)
{
	_starfield.render(target);
	_player->render(target);

	for (auto& p : _planets) p.render(target);
}

void MainGame::update(sf::RenderWindow& window, float dt)
{
	_player->update(window, dt);

	for (auto& p : _planets) p.update(dt);
}

void MainGame::handleInput(float dt)
{
	_player->handleInput(dt);
}

void MainGame::handleEvents(sf::Event& ev)
{
	_player->handleEvents(ev);
}

void MainGame::loadResources()
{
	ResourceManager::loadTexture("Defender", "Res/PlayerShip.png");
	ResourceManager::loadTexture("Moon1", "Res/Planets/moon_sheet.png");
}

void MainGame::setupEntities()
{
	_player = new Player();
	_starfield.setStarCount(200);
	_starfield.populate(_game->getWindowHandle(), _player->getView());

	_systemSize = 5;
	_planets.resize(_systemSize);

	Planet test(300, 300, true);
	test.loadTexture(ResourceManager::getTexture("Moon1"));

	_planets.push_back(test);
}
