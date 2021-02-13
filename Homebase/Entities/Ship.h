#pragma once
#include <SFML/Graphics.hpp>

/*
	Base class for all ships
*/

class Ship {
public:
	Ship();
	void setPosition(const sf::Vector2f& position);

protected:
	void render(sf::RenderTarget& target);
	void loadTexture(const sf::Texture& texture);

protected:
	sf::Sprite sprite;

	sf::Vector2f position;
	sf::Vector2f direction;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;

	float angle;
	float aVelocity;
	float aAcceleration;
	float speed;
};