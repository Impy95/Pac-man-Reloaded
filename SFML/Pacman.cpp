/**
* @file
* Pacman.cpp
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
#include "Pacman.h"
#include "DataTables.h"
#include "Utility.h"

namespace GEX
{
	namespace
	{
		const PacmanData TABLE = initalizePacmanData();
	}

	Pacman::Pacman(const GEX::TextureManager& textures)
		: Entity()
		, sprite_(textures.get(TABLE.texture), TABLE.textureRect)
		, state_(State::Eating)
		//, type_(type)
		, direction_(Direction::Right)
		, lives_(5)
		, score_(0)
		, travelDistance_(0.f)
		, directionIndex_(0)
	{
		for (auto a : TABLE.animations)
		{
			animations_[a.first] = a.second;
		}
	}

	unsigned Pacman::getCategory() const
	{
		return Category::Player;
	}

	void Pacman::playLocalSound(CommandQueue& commands, SoundEffectID effect)
	{
	}

	/*bool Pacman::isPlayer() const
	{
		return type_ == PacmanType::Player;
	}*/

	bool Pacman::isDead()
	{
		return state_ == State::Dying;
	}

	void Pacman::die()
	{
		if (state_ != State::Dying) {
			if (lives_ > 0)
			{
				setState(State::Dying);
				lives_--;
			}
			else {
			}

		}
	}

	int Pacman::getLives()
	{
		return lives_;
	}

	void Pacman::setState(State state)
	{
		state_ = state;
		animations_[state_].restart();
	}

	void Pacman::setDirection(Direction direction)
	{
		// sets the direction and changes the direction the player moves
		// based off direction passed in
		direction_ = direction;
		if (direction_ == Direction::Left)
			setVelocity(-150.f, 0.f);
		if (direction_ == Direction::Right)
			setVelocity(150.f, 0.f);
		if (direction_ == Direction::Up)
			setVelocity(0.f, -150.f);
		if (direction == Direction::Down)
			setVelocity(0.f, 150.f);
	}

	void Pacman::collectCherry()
	{
		score_ += 200;
	}

	int Pacman::getScore()
	{
		return score_;
	}

	sf::FloatRect Pacman::getBoundingBox() const
	{
		return getWorldTransform().transformRect(sprite_.getGlobalBounds());
	}

	bool Pacman::isMarkedForRemoval() const
	{
		return isDestroyed() && animations_[State::Dying].isFinished();
	}

	void Pacman::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite_, states);
	}

	void Pacman::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		if (isDestroyed()) {
			return;
		}
		// don't move if its dead
		if (state_ != State::Dying) {
			Entity::updateCurrent(dt, commands);
		}
		if (isDead() && animations_[state_].isFinished()) {
			state_ = State::Eating;
		}
		auto rec = animations_.at(state_).update(dt);
		sprite_.setTextureRect(rec);
		centerOrigin(sprite_);

		if (direction_ == Direction::Left)
			flip(rec);

		updateDirection();

		
	}

	void Pacman::updateStates()
	{
		if (isDestroyed())
			state_ = State::Dying;
	}

	void Pacman::updateDirection()
	{
		// rotates the player based off the direction they are moving
		if (direction_ == Direction::Up)
		{
			setRotation(-90);
		}
		if (direction_ == Direction::Down)
		{
			setRotation(90);
		}
		if (direction_ == Direction::Left)
		{
			setRotation(180);
		}
		if (direction_ == Direction::Right)
		{
			setRotation(0);
		}
	}

	void Pacman::updateMovementPattern(sf::Time dt)
	{
	}

	void Pacman::remove()
	{
		Entity::remove();
	}
}
