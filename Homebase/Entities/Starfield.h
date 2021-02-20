#pragma once
#include <Thor/Math.hpp>
#include <SFML/Graphics.hpp>
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
	   Populate the arena with stars
	*/
	void populate();

private:
	void initBorder();
	void addStar(Star &star);

private:
	sf::VertexArray _stars;
	int _starCount;

	sf::RectangleShape _border;
};