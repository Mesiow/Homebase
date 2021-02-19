#include "Bullet.h"
#include <Game/ResourceManager/ResourceManager.h>

Bullet::Bullet(float x, float y, float dx, float dy)
{
	_position = sf::Vector2f(x, y);
	_direction = sf::Vector2f(dx, dy);
	_speed = 390.0f;
	loadTexture();

	/* Constant velocity */
	_velocity = (_direction * _speed);
}

void Bullet::render(sf::RenderTarget& target)
{
	target.draw(_sprite);
}

void Bullet::update(float dt)
{	
	_position += _velocity * dt;
	_sprite.setPosition(_position);
}

void Bullet::loadTexture()
{
	auto &texture = ResourceManager::getTexture("Laser");
	_sprite.setTexture(texture);
	_sprite.setOrigin(sf::Vector2f(_sprite.getLocalBounds().width / 2.0f, _sprite.getLocalBounds().height / 2.0f));
	_sprite.setScale(0.3f, 0.3f);

	float angle = (atan2(_direction.y, _direction.x)) * 180.0f / (float)M_PI;
	angle += 90;
	_sprite.setRotation(angle);
}
