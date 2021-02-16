#include "Planet.h"
#include <Game/ResourceManager/ResourceManager.h>
#include <Game/Game.h>
#include <iostream>

Planet::Planet(float x, float y, bool animated)
{
	_animated = animated;
	_position = sf::Vector2f(x, y);
	_sprite.setPosition(_position);
}

void Planet::render(sf::RenderTarget& target)
{
	target.draw(_sprite);
}

void Planet::update(float dt)
{
	if (_animated) {
		_animator.update(_animClock.restart());
		_animator.animate(_sprite);
	}
}

void Planet::loadTexture(const sf::Texture& texture)
{
	_sprite.setTexture(texture);
	if (_animated) {
		int frames = 60;
		int tWidth = (int)(texture.getSize().x / 60);
		int tHeight = (int)(texture.getSize().y / 1);

		sf::IntRect rect(0, 0, tWidth, tHeight);
		_sprite.setTextureRect(rect);
		_sprite.setScale(2.0f, 2.0f);

		for (int i = 0; i < frames; i++) {
			sf::IntRect newRect(i * tWidth, 0, tWidth, tHeight);
			_animation.addFrame(0.5f, newRect);
		}
		_animator.addAnimation("spin", _animation, sf::seconds(4.0f));
		_animator.playAnimation("spin", true);
	}
}
