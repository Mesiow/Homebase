#include "PlayingState.h"
#include <Game/Game.h>

PlayingState::PlayingState(Game* game)
	:State(game)
{
	loadResources();
	setupEntities();

	_starfield.setStarCount(300);
	_starfield.populate();

	_view.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	_view.setCenter(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
	_view.setViewport(sf::FloatRect(0, 0, 1, 1));
}

PlayingState::~PlayingState()
{
	delete _player;
}

void PlayingState::render(sf::RenderWindow& window)
{
	//window.setView(_view);

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
