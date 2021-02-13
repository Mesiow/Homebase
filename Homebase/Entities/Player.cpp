#include "Player.h"
#include <Game/ResourceManager/ResourceManager.h>

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
	_w = &window;

	updateDirection(window, dt);
	updateRotation(dt);

	position += velocity * dt;
	sprite.setPosition(position);
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
	}
}

void Player::setup()
{
	_rotationLocked = false;

	sprite.setScale(sf::Vector2f(0.2f, 0.2f));
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f));

	position = sf::Vector2f(200, 200);
	sprite.setPosition(position);
	sprite.setRotation(90);
	
	speed = 150.0f;
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

void Player::updateRotation(float dt)
{
	if (!_rotationLocked) {
		auto mpos = sf::Mouse::getPosition(*_w);
		auto diff = (sf::Vector2f)mpos - position;

		angle = (float)atan2(diff.y, diff.x) * 180.0f / M_PI;
		//angle = angle + 90;
		sprite.setRotation(angle + 90);
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
