#include "Starfield.h"

Starfield::Starfield()
{
	_starCount = 250;
	_stars.clear();
}

Starfield::~Starfield()
{
}

void Starfield::render(sf::RenderTarget& target)
{
	for (auto& s : _stars) target.draw(s.star);
}

void Starfield::update()
{
	
}

void Starfield::setStarCount(int count)
{
	_starCount = count;
}

void Starfield::populate(sf::RenderWindow& window, const sf::Vector2f& position)
{
	Star star;
	star.star.setFillColor(sf::Color::White);

	for (size_t i = 0; i < _starCount; i++) {
		float posX = thor::random(0.0f, (float)SCREEN_WIDTH);
		float posY = thor::random(0.0f, (float)SCREEN_HEIGHT);
		float size = thor::random(0.1f, 1.0f);
		sf::Vector2f position(posX, posY);

	
		star.star.setPosition(position);
		star.star.setRadius(size);
		addStar(star);
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
	_stars.emplace_back(star);
}
