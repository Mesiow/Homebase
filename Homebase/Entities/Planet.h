#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Animations.hpp>

class Planet {
public:
	Planet() = default;
	Planet(float x, float y, bool animated = false);
	void render(sf::RenderTarget& target);
	void update(float dt);
	void loadTexture(const sf::Texture& texture);

private:
	sf::Sprite _sprite;
	sf::Vector2f _position;

	thor::Animator<sf::Sprite, std::string> _animator;
	thor::FrameAnimation _animation;
	sf::Clock _animClock;
	float _animProgress = 0.0f;
	bool _animated;
};