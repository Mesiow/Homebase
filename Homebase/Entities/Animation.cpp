#include "Animation.h"

void Animation::setAnimationSheet(const sf::Texture& texture)
{
	int nFramesRow = (int)(texture.getSize().x) / (int)(texture.getSize().y);
	int nFramesColumn = (int)(texture.getSize().y) / (int)(texture.getSize().y);

	int tWidth = (int)(texture.getSize().x / nFramesRow); //width of one frame in a row
	int tHeight = (int)(texture.getSize().y / nFramesColumn); //width of one frame in a column

	frameWidth = tWidth;
	frameHeight = tHeight;

	for (int i = 0; i < nFramesRow; i++) {
		for (int j = 0; j < nFramesColumn; j++) {
			sf::IntRect newRect(i * tWidth, j * tHeight, tWidth, tHeight);
			frames.addFrame(0.5f, newRect);
		}
	}
}

void Animation::play(const std::string& name, bool loop)
{
	animator.playAnimation(name, loop);
}

void Animation::update(sf::Sprite &sprite)
{
	animator.update(clock.restart());
	animator.animate(sprite);
}

bool Animation::isDone()
{
	return !animator.isPlayingAnimation();
}

void Animation::add(const std::string& name, sf::Time duration)
{
	animator.addAnimation(name, frames, duration);
}
