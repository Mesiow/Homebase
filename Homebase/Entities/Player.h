#pragma once
#include <Game/Math/Math.h>
#include "Bullet.h"
#include "Ship.h"
#include "../Constants.h"
#include <Thor/Math.hpp>
#include <algorithm>

class Game;

enum class Moving {
	Forward = 0,
	Left,
	Right
};



class Player : public Ship {
public:
	Player(Game *game, const sf::Texture &texture);
	void render(sf::RenderTarget& target);
	void update(sf::RenderWindow& window, float dt);
	void handleInput(float dt);
	void handleEvents(sf::Event& ev);
	const sf::View& getView()const { return _view; }

	void enableMinimapView(sf::RenderTarget& target);
	void enablePlayerView(sf::RenderTarget& target);

	void shoot(float x, float y, float dx, float dy);
	void updateBullets(float dt);

private:
	void setup();
	void updateVelocity(float dt);
	void updateRotation(sf::RenderWindow &window, float dt);
	void updateDirection(sf::RenderWindow& window, float dt);
	void updateViews();

	void thrust(Moving thrust, float dt);
	void clampVelocity();

private:
	struct Minimap {
		sf::View view;
		sf::Vector2f position;
		sf::RectangleShape background;
	}_minimap;

	std::vector<Bullet> _bullets;
	sf::View _view;
	sf::Vector2f _minimapPosition;

	float _thrustSpeed;
	float _maxVelocity;
	float _zoomValue;
	bool _rotationLocked;
};