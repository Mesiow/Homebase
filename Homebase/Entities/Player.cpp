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
		updateVelocity(Moving::Forward, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		updateVelocity(Moving::Left, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		updateVelocity(Moving::Right, dt);
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
	_zoomValue = 1.0f;
	_rotationLocked = false;

	sprite.setScale(sf::Vector2f(0.1f, 0.1f));
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f));

	position = sf::Vector2f(-1000, 200);
	sprite.setPosition(position);
	sprite.setRotation(90);
	
	speed = 150.0f;

	/*
		Player viewport
	*/
	_view.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	_view.setCenter(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
	_view.setViewport(sf::FloatRect(0, 0, 1, 1));

	std::cout << "View size: " << _view.getSize().x << _view.getSize().y << std::endl;
}

void Player::updateVelocity(Moving moving, float dt)
{
	if(moving == Moving::Forward)
		velocity += (direction * speed) * dt;

	//if (moving == Moving::Left)
	//	velocity.x -= (direction.x * speed) * dt;
	//else if(moving == Moving::Right)
	//	velocity.x += (direction.x * speed) * dt;
}

void Player::updateRotation(sf::RenderWindow& window, float dt)
{
	if (!_rotationLocked) {
		auto mpos = sf::Mouse::getPosition(window);
		auto worldpos = window.mapPixelToCoords(mpos);
		auto diff = (sf::Vector2f)worldpos - position;

		angle = (float)(atan2(diff.y, diff.x)) * 180.0f / M_PI;
		angle = angle + 90;
		sprite.setRotation(angle);
	}
}

void Player::updateDirection(sf::RenderWindow &window, float dt)
{
	//sf::Vector2i mpos = sf::Mouse::getPosition(window);
	//sf::Vector2f dist = distance((sf::Vector2f)mpos, position);
	///*dist.x = cosf(angle * M_PI / 180);
	//dist.y = sinf(angle * M_PI / 180);*/
	//direction = normalize(dist);
	direction.x = sinf(M_PI * sprite.getRotation() / 180.0f);
	direction.y = -cos(M_PI * sprite.getRotation() / 180.0f);
	direction = normalize(direction);
}
