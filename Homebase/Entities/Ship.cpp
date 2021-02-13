#include "Ship.h"

Ship::Ship()
{
	position = sf::Vector2f(0, 0);
	direction = sf::Vector2f(0, 0);
	velocity = sf::Vector2f(0, 0);
	acceleration = sf::Vector2f(0, 0);

	angle = 0;
	aVelocity = 0;
	aAcceleration = 0;
	speed = 20.0f;
}

void Ship::setPosition(const sf::Vector2f& position)
{
	this->position = position;
}

void Ship::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}

void Ship::loadTexture(const sf::Texture& texture)
{
	sprite.setTexture(texture);
}
