#include "Starfield.h"
#include <iostream>

Starfield::Starfield()
{
	_starCount = 250;
	_stars.clear();
	_stars.resize(_starCount);
	initBorder();
}

Starfield::~Starfield()
{

}

void Starfield::render(sf::RenderTarget& target)
{
	target.draw(_border);
	target.draw(_stars);	
}

void Starfield::update()
{
	
}

void Starfield::setStarCount(int count)
{
	_starCount = count;
	_stars.resize(count);
}

void Starfield::populate()
{
	Star star;
	for (size_t i = 0; i < _starCount; i++) {
		float z = thor::random(0.0f, 1.0f);
		sf::Color color;
		if (z >= 0.5f) {
			color = sf::Color(150, 0, 100, 255);
		}
		else if (z <= 0.5f && z >= 0.3f) {
			color = sf::Color(1, 100, 150, 255);
		}
		else if (z <= 0.3f) {
			color = sf::Color(240, 240, 255, 255);
		}
		star.vert.color = color;

		float left = -ARENA_WIDTH;
		float top = -ARENA_HEIGHT;
		float right = ARENA_WIDTH;
		float bottom = ARENA_HEIGHT;

		float posX = thor::random(left, right);
		float posY = thor::random(top, bottom);
		float size = thor::random(0.1f, 1.0f);
		sf::Vector2f position(posX, posY);

		if (size < 0.5f) {
			//make star as 1 pixel
			star.vert.position = position;
			addStar(star);
		}
		else if (size > 0.5f && size < 0.8f) {
			//make star 3 pixels
			star.vert.position = position;
			addStar(star);

			star.vert.position.x += 1;
			addStar(star);
			
			star.vert.position.y -= 1;
			addStar(star);
		}
		else if (size >= 0.9f) {
			//make star 4 pixels
			star.vert.position = position;
			addStar(star);

			star.vert.position.x += 1;
			addStar(star);

			star.vert.position.y -= 1;
			addStar(star);

			star.vert.position.y += 1;
			addStar(star);
		}
	}
}

void Starfield::initBorder()
{
	_border.setFillColor(sf::Color::Transparent);
	_border.setSize(sf::Vector2f(ARENA_WIDTH + ARENA_WIDTH, ARENA_HEIGHT + ARENA_HEIGHT));
	_border.setOrigin(sf::Vector2f(_border.getLocalBounds().width / 2.0f, _border.getLocalBounds().height / 2.0f));
	_border.setPosition(0, 0);
	_border.setOutlineThickness(1.0f);
	_border.setOutlineColor(sf::Color::Red);
}

void Starfield::addStar(Star &star)
{
	_stars.append(star.vert);
}
