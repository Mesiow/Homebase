#include "MainGame.h"
#include <Game/Game.h>
#include <Game/ResourceManager/ResourceManager.h>

MainGame::MainGame(Game* game)
{
	loadResources();

	char c;
	std::cout << "1. Host\n";
	std::cout << "2. Peer\n";
	std::cin >> c;
	if (c == '1')
		_peer = std::make_unique<Peer>(game, true);
	else
		_peer = std::make_unique<Peer>(game);

	_starfield.setStarCount(10000);
	_starfield.populate();
}

MainGame::~MainGame()
{

}

void MainGame::render(sf::RenderTarget& target)
{
	_starfield.render(target);
	_peer->render(target);
}

void MainGame::update(sf::RenderWindow& window, float dt)
{
	_peer->update(dt);
}

void MainGame::handleInput(float dt)
{
	_peer->handleInput(dt);
}

void MainGame::handleEvents(sf::Event& ev)
{
	_peer->handleEvents(ev);
}

void MainGame::loadResources()
{
	//Load all player textures
	ResourceManager::loadTexture("Player1", "Res/Player1.png");
	ResourceManager::loadTexture("Player2", "Res/Player2.png");
	ResourceManager::loadTexture("Player3", "Res/Player3.png");
	ResourceManager::loadTexture("Player4", "Res/Player4.png");

	ResourceManager::loadTexture("Defender", "Res/PlayerShip.png");
	ResourceManager::loadTexture("Moon1", "Res/Planets/moon_sheet.png");
	ResourceManager::loadTexture("Terran_Cloudy", "Res/Planets/Terran_Sheet.png");
	ResourceManager::loadTexture("Gas_Giant_Red", "Res/Planets/Gas_Giant_Sheet.png");

	ResourceManager::loadTexture("Laser", "Res/laser.png");
}
