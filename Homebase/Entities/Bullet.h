#pragma once
#include <Game/Math/Math.h>
#include "../Constants.h"

class Bullet {
public:
	Bullet(float x, float y, float dx, float dy);
	void render(sf::RenderTarget& target);
	void update(float dt);

	bool outOfBounds()const;

private:
	void loadTexture();

private:
	sf::Sprite _sprite;
	sf::Vector2f _velocity;
	sf::Vector2f _position;
	sf::Vector2f _direction;
	float _speed;
};