#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Animations.hpp>



class Planet {
public:
	enum class Type {
		Planet = 0,
		Moon
	};

	enum class Status {
		Neutral = 0,
		Captured
	};

	struct PlanetOutline {
		PlanetOutline(float radius = 25.0f);
		void render(sf::RenderTarget& target);
		void update(float dt);
		void attach(Planet& planet);
		void setOutline(Planet::Status status);
		void setRadius(float radius);
		sf::CircleShape shape;
		sf::CircleShape orbiter;
		float angle;
	};

public:
	Planet() = default;
	Planet(Planet::Type type, float x, float y, bool animated = true);
	void render(sf::RenderTarget& target);
	void update(sf::RenderWindow &window, float dt);
	void loadTexture(const sf::Texture& texture);


private:
	void updateStatus(Planet::Status newStatus);
	/*
		Show outline of planet, if captured, neutral or not
	*/
	void onMouseHover(sf::RenderWindow &window);

private:
	sf::Sprite _sprite;
	sf::Vector2f _position;

	PlanetOutline _outline;
	Planet::Type _type;
	Planet::Status _status;
	bool _renderOutline;

	thor::Animator<sf::Sprite, std::string> _animator;
	thor::FrameAnimation _animation;
	sf::Clock _animClock;
	float _animProgress = 0.0f;
	bool _animated;
};