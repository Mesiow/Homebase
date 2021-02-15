#include "NetGame.h"
#include <Game/ResourceManager/ResourceManager.h>

NetGame::NetGame(Game* game)
{
	loadResources();
}

NetGame::~NetGame()
{
}

void NetGame::render(sf::RenderTarget& target)
{
}

void NetGame::update(sf::RenderWindow& window, float dt)
{

}

void NetGame::handleInput(float dt)
{
}

void NetGame::handleEvents(sf::Event& ev)
{
}

void NetGame::loadResources()
{
	ResourceManager::loadTexture("Defender", "Res/PlayerShip.png");
	ResourceManager::loadTexture("Moon1", "Res/Planets/moon_sheet.png");
}
