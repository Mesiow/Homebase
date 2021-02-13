#include "PlayingState.h"
#include <Game/Game.h>

PlayingState::PlayingState(Game* game)
	:State(game)
{
	loadResources();
	setupEntities();

	_starfield.setStarCount(300);
	_starfield.populate(_game->getWindowHandle(), _player->getPosition());
}

PlayingState::~PlayingState()
{
	delete _player;
}

void PlayingState::render(sf::RenderWindow& window)
{
	_starfield.render(window);
	_player->render(window);
}

void PlayingState::update(float dt)
{
	_player->update(_game->getWindowHandle(), dt);
}

void PlayingState::handleInput(float dt)
{
	_player->handleInput(dt);
}

void PlayingState::handleEvents(sf::Event& ev, sf::RenderWindow& window)
{
	_player->handleEvents(ev);
}

void PlayingState::loadResources()
{
	ResourceManager::loadTexture("Defender", "Res/PlayerShip.png");
	ResourceManager::loadTexture("Planet1", "Res/Planets/System6.png");
}

void PlayingState::setupEntities()
{
	_player = new Player();
}
