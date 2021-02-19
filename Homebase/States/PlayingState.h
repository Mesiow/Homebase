#pragma once
#include <Game/State/State.h>
#include "../Net/MainGame.h"


class PlayingState : public State {
public:
	PlayingState(Game* game);
	~PlayingState();

	void render(sf::RenderWindow& window)override;
	void update(float dt)override;
	void handleInput(float dt)override;
	void handleEvents(sf::Event& ev, sf::RenderWindow& window);

private:
	std::unique_ptr<MainGame> _mgame;
};