#include "Player.h"
#include <Game/ResourceManager/ResourceManager.h>
#include <Game/Game.h>
#include <iostream>

Player::Player(Game *game)
{
	loadTexture(ResourceManager::getTexture("Defender"));
	setup();
}

void Player::render(sf::RenderTarget& target)
{
	for (auto& b : _bullets) b.render(target);
	Ship::render(target);

	target.draw(_minimap.background);
}

void Player::update(sf::RenderWindow& window, float dt)
{
	window.setView(_view);

	updateDirection(window, dt);
	updateRotation(window, dt);

	position += velocity * dt;
	sprite.setPosition(position);

	updateBullets(dt);
	updateViews();

	

	/*sf::RectangleShape s;
	s.setFillColor(sf::Color::Transparent);
	s.setOutlineThickness(2.0f);
	s.setOutlineColor(sf::Color::Green);
	s.setSize(_minimap.getSize());
	s.setPosition(_minimap.getCenter());
	window.draw(s);*/
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
		case sf::Event::MouseButtonPressed: {
			if (ev.mouseButton.button == sf::Mouse::Left) {
				shoot(position.x, position.y, direction.x, direction.y);
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
				//Lerp the view
				auto prevView = _view.getSize();
				auto targetView = _view.getSize() * _zoomValue;
				
				float vx = lerp(prevView.x, targetView.x, 0.08f);
				float vy = lerp(prevView.y, targetView.y, 0.08f);

				sf::Vector2f lerpedView = sf::Vector2f(vx, vy);
				_view.setSize(lerpedView);
			}
		}break;
	}
}

void Player::enableMinimapView(sf::RenderTarget &target)
{
	target.setView(_minimap.view);
}

void Player::enablePlayerView(sf::RenderTarget& target)
{
	//Set window view to the player's view
	target.setView(_view);
}

void Player::shoot(float x, float y, float dx, float dy)
{
	Bullet bullet(x, y, dx, dy);
	_bullets.emplace_back(bullet);
}

void Player::updateBullets(float dt)
{
	for (auto& b : _bullets) b.update(dt);

	if (!_bullets.empty()) {
		_bullets.erase(std::remove_if(
			_bullets.begin(), _bullets.end(),
			[&](const Bullet& b) { return b.outOfBounds(); }
		), _bullets.end());
	}
}

void Player::setup()
{
	_bullets.clear();

	_thrustSpeed = 200.0f;
	_maxVelocity = 250.0f;
	_zoomValue = 1.0f;
	_rotationLocked = false;

	sprite.setScale(sf::Vector2f(0.1f, 0.1f));
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f));

	float x = thor::random(-200, 200);
	float y = thor::random(-200, 200);
	position = sf::Vector2f(x, y);
	sprite.setPosition(position);
	sprite.setRotation(90);
	
	speed = 125.0f;

	/*
		Player viewport
	*/
	_view.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	_view.setCenter(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
	_view.setViewport(sf::FloatRect(0, 0, 1, 1));

	/*
		Player's minimap viewport
	*/
	/*_minimap.view.setViewport(sf::FloatRect(0.75f, 0.0f, 0.25f, 0.25f));
	_minimap.view.setSize(sf::Vector2f(8050, 4528));

	_minimap.background.setFillColor(sf::Color(50, 50, 50, 70));
	_minimap.background.setSize(sf::Vector2f(250, 160));
	_minimap.background.setOrigin(sf::Vector2f(_minimap.background.getLocalBounds().width / 2.0f, _minimap.background.getLocalBounds().height / 2.0f));*/
}

void Player::updateVelocity(float dt)
{
	velocity += (direction * speed) * dt;
	clampVelocity();
}

void Player::updateRotation(sf::RenderWindow& window, float dt)
{
	if (window.hasFocus()) {
		auto prevmouse = sf::Mouse::getPosition(window);
		auto prevworld = window.mapPixelToCoords(prevmouse);

		float prevAngle = sprite.getRotation();
		if (!_rotationLocked) {
			auto mpos = sf::Mouse::getPosition(window);
			auto worldpos = window.mapPixelToCoords(mpos);
			auto diff = (sf::Vector2f)worldpos - position;


			angle = (atan2(diff.y, diff.x)) * 180.0f / (float)M_PI;
			angle = angle + 90;

			//Lerp rotation
			float difference = abs(angle - prevAngle);
			if (difference > 180) {
				if (angle > prevAngle) {
					prevAngle += 360.0f;
				}
				else {
					angle += 360.0f;
				}
			}
			float newangle = lerp(prevAngle, angle, 5.0f * dt);
			sprite.setRotation(newangle);
		}
	}
}

void Player::updateDirection(sf::RenderWindow &window, float dt)
{
	direction.x = sinf((float)M_PI * sprite.getRotation() / 180.0f);
	direction.y = -cosf((float)M_PI * sprite.getRotation() / 180.0f);
	direction = normalize(direction);
}

void Player::updateViews()
{
	_view.setCenter(sprite.getPosition());

	//_minimap.position = sf::Vector2f(
	//	((_view.getCenter().x + _view.getSize().x / 3.0f) + _minimap.view.getSize().x / 2.0f),
	//	((_view.getCenter().y - _view.getSize().y / 2.5f) - _minimap.view.getSize().y / 2.0f)
	//);
	
	/*_minimap.background.setPosition((_view.getCenter().x + SCREEN_WIDTH / 2.0F) - _minimap.background.getSize().x,
		(_view.getCenter().y - SCREEN_HEIGHT / 3.0F) - _minimap.background.getSize().y / 2.0f);*/
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
	clampVelocity();
}

void Player::clampVelocity()
{
	velocity.x = std::clamp(velocity.x, -_maxVelocity, _maxVelocity);
	velocity.y = std::clamp(velocity.y, -_maxVelocity, _maxVelocity);
}
