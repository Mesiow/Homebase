#include "MainGame.h"
#include <Game/Game.h>
#include <Game/ResourceManager/ResourceManager.h>

MainGame::MainGame(Game* game)
{
	loadResources();
	_peer = std::make_unique<Peer>(game);

	_starfield.setStarCount(1500);
	_starfield.populate(game->getWindowHandle(), sf::View());
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
	ResourceManager::loadTexture("Defender", "Res/PlayerShip.png");
	ResourceManager::loadTexture("Moon1", "Res/Planets/moon_sheet.png");
}
