#include "Planet.h"
#include <Game/ResourceManager/ResourceManager.h>
#include <Game/Game.h>
#include <iostream>

Planet::Planet(Planet::Type type, float x, float y, bool animated)
{
	_type = type;
	_status = Planet::Status::Neutral;
	_animated = animated;
	_position = sf::Vector2f(x, y);
	_sprite.setPosition(_position);
}

void Planet::render(sf::RenderTarget& target)
{
	target.draw(_sprite);
	if (_renderOutline) {
		_outline.render(target);
	}
}

void Planet::update(sf::RenderWindow &window, float dt)
{
	onMouseHover(window);
	if (_renderOutline) {
		_outline.update(dt);
	}

	if (_animated) {
		_animation.update(_sprite);
	}

}

void Planet::loadTexture(const sf::Texture& texture)
{
	_sprite.setTexture(texture);
	if (_animated) {
		_animation.setAnimationSheet(texture);

		sf::IntRect rect(0, 0, _animation.frameWidth, _animation.frameHeight);
		_sprite.setTextureRect(rect);

		float rotationTime = _animation.nFrames / 6.0f;

		//Setup planet
		switch (_type) {
			case Planet::Type::Planet: {
				_sprite.setScale(4.0f, 4.0f); 
				_outline.setRadius(_animation.frameWidth * 4.0f);
			}break;
			case Planet::Type::Moon: { 
				_sprite.setScale(2.5f, 2.5f); 
				_outline.setRadius(_animation.frameWidth * 2.5f);
			} break;
		}
		_sprite.setOrigin(sf::Vector2f(_sprite.getLocalBounds().width / 2.0f, _sprite.getLocalBounds().height / 2.0f));
		_outline.attach(*this);
		updateStatus(_status);

		_animation.add("spin", sf::seconds(rotationTime));
		_animation.play("spin", true);
	}
}

void Planet::updateStatus(Planet::Status newStatus)
{
	_status = newStatus;
	_outline.setOutline(_status);
}

void Planet::onMouseHover(sf::RenderWindow &window)
{
	sf::Vector2i mouse = sf::Mouse::getPosition(window);
	auto point = window.mapPixelToCoords(mouse);
	if (_sprite.getGlobalBounds().contains((sf::Vector2f)point)) {
		_renderOutline = true;
	}
	else {
		_renderOutline = false;
	}
}

Planet::PlanetOutline::PlanetOutline(float radius)
{
	angle = 0.0f;
	shape.setRadius(radius);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(2.0f);
	shape.setOrigin(radius, radius);

	orbiter.setRadius(5.0f);
	orbiter.setOrigin(5.0f, 5.0f);
	orbiter.setFillColor(sf::Color::Transparent);
	orbiter.setOutlineThickness(1.0f);
	orbiter.setOutlineColor(sf::Color::White);
}

void Planet::PlanetOutline::render(sf::RenderTarget& target)
{
	target.draw(shape);
	target.draw(orbiter);
}

void Planet::PlanetOutline::update(float dt)
{
	float x = shape.getRadius() * cosf(angle);
	float y = shape.getRadius() * sinf(angle);
	angle += 2.0f * dt;
	orbiter.setPosition(shape.getPosition().x + x, shape.getPosition().y + y);
}

void Planet::PlanetOutline::attach(Planet& planet)
{
	shape.setPosition(planet._position);
	orbiter.setPosition(sf::Vector2f(shape.getPosition().x + shape.getRadius(), shape.getPosition().y));
}

void Planet::PlanetOutline::setOutline(Planet::Status status)
{
	switch (status) {
		case Planet::Status::Neutral: shape.setOutlineColor(sf::Color(255, 255, 255, 100)); break;
	}
}

void Planet::PlanetOutline::setRadius(float radius)
{
	shape.setRadius(radius);
	shape.setOrigin(radius, radius);
}
