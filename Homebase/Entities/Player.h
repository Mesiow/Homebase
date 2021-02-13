#pragma once
#include <Game/Math/Math.h>
#include "Ship.h"

enum class Moving {
	Forward = 0,
	Left,
	Right
};

class Player : public Ship {
public:
	Player();
	void render(sf::RenderTarget& target);
	void update(sf::RenderWindow& window, float dt);
	void handleInput(float dt);
	void handleEvents(sf::Event& ev);

private:
	void setup();
	void updateVelocity(Moving moving, float dt);
	void updateRotation(float dt);
	void updateDirection(sf::RenderWindow& window, float dt);

private:
	bool _rotationLocked;
	sf::RenderWindow* _w;
	sf::View _view;
};