#pragma once
#include <Thor/Math.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "../Constants.h"

struct Star {
	sf::Vertex vert;
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
	void populate(sf::RenderWindow &window, const sf::View &view);

	bool isInView(sf::RenderWindow &window, const sf::FloatRect& view);

private:
	void addStar(Star &star);

private:
	sf::VertexArray _stars;
	int _starCount;
};