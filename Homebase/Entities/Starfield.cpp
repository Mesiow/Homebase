#include "Starfield.h"
#include <iostream>

Starfield::Starfield()
{
	_starCount = 250;
	_stars.clear();
	_stars.resize(_starCount);
}

Starfield::~Starfield()
{
}

void Starfield::render(sf::RenderTarget& target)
{
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

void Starfield::populate(sf::RenderWindow& window, const sf::View &view)
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

		float left = 0 - view.getCenter().x;
		float top = 0 - view.getCenter().y;

		float posX = thor::random(left - 600, (float)SCREEN_WIDTH + 600);
		float posY = thor::random(top - 600, (float)SCREEN_HEIGHT + 600);
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

bool Starfield::isInView(sf::RenderWindow& window, const sf::FloatRect& view)
{
	for (size_t i = 0; i < _starCount; ++i) {
		
	}
	return false;
}

void Starfield::addStar(Star &star)
{
	_stars.append(star.vert);
}
