#include "PlayingState.h"
#include <Game/Game.h>

PlayingState::PlayingState(Game* game)
	:State(game)
{
	_mgame = std::make_unique<MainGame>(_game);
}

PlayingState::~PlayingState()
{
	
}

void PlayingState::render(sf::RenderWindow& window)
{
	_mgame->render(window);
}

void PlayingState::update(float dt)
{
	_mgame->update(State::_game->getWindowHandle(), dt);
}

void PlayingState::handleInput(float dt)
{
	_mgame->handleInput(dt);
}

void PlayingState::handleEvents(sf::Event& ev, sf::RenderWindow& window)
{
	_mgame->handleEvents(ev);
}
