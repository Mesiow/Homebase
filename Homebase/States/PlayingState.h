#pragma once
#include <Game/State/State.h>
#include <Game/ResourceManager/ResourceManager.h>
#include "../Entities/Player.h"
#include "../Entities/Starfield.h"

class PlayingState : public State {
public:
	PlayingState(Game* game);
	~PlayingState();

	void render(sf::RenderWindow& window)override;
	void update(float dt)override;
	void handleInput(float dt)override;
	void handleEvents(sf::Event& ev, sf::RenderWindow& window);

private:
	void loadResources();
	void setupEntities();

private:
	Player* _player;
	Starfield _starfield;
	sf::View _view;
};