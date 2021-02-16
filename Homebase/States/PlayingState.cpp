#include "PlayingState.h"
#include <Game/Game.h>

PlayingState::PlayingState(Game* game)
	:State(game)
{
	_netgame = std::make_unique<MainGame>(_game);
}

PlayingState::~PlayingState()
{
	
}

void PlayingState::render(sf::RenderWindow& window)
{
	_netgame->render(window);
}

void PlayingState::update(float dt)
{
	_netgame->update(_game->getWindowHandle(), dt);
}

void PlayingState::handleInput(float dt)
{
	_netgame->handleInput(dt);
}

void PlayingState::handleEvents(sf::Event& ev, sf::RenderWindow& window)
{
	_netgame->handleEvents(ev);
}
