#include "SpriteNode.h"
#include <SFML/Graphics.hpp>


namespace GEX {
	SpriteNode::SpriteNode(const sf::Texture & texture) : sprite_(texture)
	{}

	SpriteNode::SpriteNode(const sf::Texture & texture, const sf::IntRect & textureRect) : sprite_(texture,textureRect)
	{}

	sf::FloatRect SpriteNode::getBoundingBox() const
	{
		return getWorldTransform().transformRect(sprite_.getGlobalBounds());
	}

	void SpriteNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(sprite_, states);
	}

}
