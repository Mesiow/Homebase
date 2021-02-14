#include "PlayingState.h"
#include <Game/Game.h>

PlayingState::PlayingState(Game* game)
	:State(game)
{
	_mainGame = new MainGame(_game);
}

PlayingState::~PlayingState()
{
	delete _mainGame;
}

void PlayingState::render(sf::RenderWindow& window)
{
	_mainGame->render(window);
}

void PlayingState::update(float dt)
{
	_mainGame->update(_game->getWindowHandle(), dt);
}

void PlayingState::handleInput(float dt)
{
	_mainGame->handleInput(dt);
}

void PlayingState::handleEvents(sf::Event& ev, sf::RenderWindow& window)
{
	_mainGame->handleEvents(ev);
}
