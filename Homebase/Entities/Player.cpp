#include "Player.h"
#include <Game/ResourceManager/ResourceManager.h>
#include <iostream>

Player::Player()
{
	loadTexture(ResourceManager::getTexture("Defender"));
	setup();
}

void Player::render(sf::RenderTarget& target)
{
	Ship::render(target);
}

void Player::update(sf::RenderWindow& window, float dt)
{
	//Set window view to the player's view
	window.setView(_view);

	updateDirection(window, dt);
	updateRotation(window, dt);

	position += velocity * dt;
	sprite.setPosition(position);

	_view.setCenter(sprite.getPosition());
}

void Player::handleInput(float dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		updateVelocity(dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		thrust(Moving::Left, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		thrust(Moving::Right, dt);
	}
}

void Player::handleEvents(sf::Event& ev)
{
	switch (ev.type) {
		case sf::Event::KeyPressed: {
			if (ev.key.code == sf::Keyboard::C) {
				_rotationLocked = true;
			}
		}break;
		case sf::Event::KeyReleased: {
			if (ev.key.code == sf::Keyboard::C) {
				_rotationLocked = false;
			}
		}break;
		case sf::Event::MouseWheelScrolled: {
			if (ev.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
				if (ev.mouseWheelScroll.delta == -1) {
					_zoomValue = 1 + 0.1f;
				}
				else {
					_zoomValue = 1 - 0.1f;
				}
				_view.zoom(_zoomValue);
			}
		}break;
	}
}

void Player::setup()
{
	_thrustSpeed = 200.0f;
	_zoomValue = 1.0f;
	_rotationLocked = false;

	sprite.setScale(sf::Vector2f(0.1f, 0.1f));
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f));

	position = sf::Vector2f(200, 200);
	sprite.setPosition(position);
	sprite.setRotation(90);
	
	speed = 150.0f;

	/*
		Player viewport
	*/
	_view.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	_view.setCenter(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
	_view.setViewport(sf::FloatRect(0, 0, 1, 1));
}

void Player::updateVelocity(float dt)
{
	velocity += (direction * speed) * dt;
}

void Player::updateRotation(sf::RenderWindow& window, float dt)
{
	if (!_rotationLocked) {
		auto mpos = sf::Mouse::getPosition(window);
		auto worldpos = window.mapPixelToCoords(mpos);
		auto diff = (sf::Vector2f)worldpos - position;

		angle = (atan2(diff.y, diff.x)) * 180.0f / (float)M_PI;
		angle = angle + 90;
		sprite.setRotation(angle);
	}
}

void Player::updateDirection(sf::RenderWindow &window, float dt)
{
	direction.x = sinf(M_PI * sprite.getRotation() / 180.0f);
	direction.y = -cos(M_PI * sprite.getRotation() / 180.0f);
	direction = normalize(direction);
}

void Player::thrust(Moving thrust, float dt)
{
	//Get Cross product of direction vector 

	sf::Vector2f right(direction.y, -direction.x);
	right = -normalize(right); //inverse because thrusting moves us the opposite direction of the vector

	sf::Vector2f left(-direction.y, direction.x);
	left = -normalize(left);

	if (thrust == Moving::Left) {
		velocity -= (left * _thrustSpeed) * dt;
	}
	if (thrust == Moving::Right) {
		velocity -= (right * _thrustSpeed) * dt;
	}
}
