#pragma once
#include <Game/Math/Math.h>
#include "Bullet.h"
#include "Ship.h"
#include "../Constants.h"

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
	const sf::View& getView()const { return _view; }

	void shoot(float x, float y, float dx, float dy);

private:
	void setup();
	void updateVelocity(float dt);
	void updateRotation(sf::RenderWindow &window, float dt);
	void updateDirection(sf::RenderWindow& window, float dt);

	void thrust(Moving thrust, float dt);

private:
	std::vector<Bullet> _bullets;
	sf::View _view;

	float _thrustSpeed;
	float _zoomValue;
	bool _rotationLocked;
};