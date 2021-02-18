#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Animations.hpp>
#include <string>

struct Animation {
	void setAnimationSheet(const sf::Texture& texture);
	void add(const std::string& name, sf::Time duration);
	void play(const std::string& name, bool loop = false); //Which animation to play
	void update(sf::Sprite &sprite); //Update animation on sprite

	bool isDone();


	int frameWidth;
	int frameHeight;
	thor::Animator<sf::Sprite, std::string> animator;
	thor::FrameAnimation frames;
	sf::Clock clock;
};