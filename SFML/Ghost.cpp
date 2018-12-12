/**
* @file
* Ghost.cpp
* @author
* Vaughn Rowse 2018
* @version 1.0
*
* @section DESCRIPTION
* For Test 1
* @section LICENSE
*
* Copyright 2018
* Permission to use, copy, modify, and/or distribute this software for
* any purpose with or without fee is hereby granted, provided that the
* above copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
* @section Academic Integrity
* I certify that this work is solely my own and complies with
* NBCC Academic Integrity Policy (policy 1111)
*/

#include "Ghost.h"
#include "DataTables.h"
#include "Utility.h"

namespace GEX
{
	namespace {
		const GhostData TABLE = initalizeGhostData();
	}

	Ghost::Ghost(const GEX::TextureManager& textures)
		: Entity()
		, sprite_(textures.get(TABLE.texture), sf::IntRect(1,1,1,1))
		, state_(State::Up)
		, direction_(Direction::Up)
	{
		for (auto a : TABLE.animations)
		{
			animations_[a.first] = a.second;
		}

		setVelocity(0.f, -100.f);
	}

	unsigned Ghost::getCategory() const
	{
		return Category::Ghost;
	}

	void Ghost::setDirection(State direction)
	{
		state_ = direction;
		animations_[state_].restart();
	}

	Ghost::State Ghost::getDirection()
	{
		return state_;
	}

	sf::FloatRect Ghost::getBoundingBox() const
	{
		return getWorldTransform().transformRect(sprite_.getGlobalBounds());
	}

	void Ghost::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite_, states);
	}

	void Ghost::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		Entity::updateCurrent(dt, commands);
		auto rec = animations_.at(state_).update(dt);
		sprite_.setTextureRect(rec);
		centerOrigin(sprite_);

		updateMovementPattern(dt);
	}

	void Ghost::updateStates()
	{
	}

	void Ghost::updateMovementPattern(sf::Time dt)
	{
		
	}

	void Ghost::remove()
	{
		Entity::remove();
	}
}
