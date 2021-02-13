#pragma once
#include <Thor/Math.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "../Constants.h"

struct Star {
	sf::CircleShape star;
};

class Starfield {
public:
	Starfield();
	~Starfield();
	void render(sf::RenderTarget& target);
	void update();
	void setStarCount(int count);
	/*
	   Populate around players location
	*/
	void populate(sf::RenderWindow &window, const sf::Vector2f &position);

	bool isInView(sf::RenderWindow &window, const sf::FloatRect& view);

private:
	void addStar(Star &star);

private:
	int _starCount;
	std::vector<Star> _stars;
};