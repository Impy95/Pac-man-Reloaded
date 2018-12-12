/**
* @file
* Pacman.h
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

#pragma once
#include "Entity.h"
#include "TextNode.h"
#include <SFML/Graphics/Sprite.hpp>
#include "Animation2.h"

namespace GEX {
	
	
	enum class SoundEffectID;
	class TextureManager;


	class Pacman : public GEX::Entity
	{
		
	public:
		/*enum class PacmanType
		{
			Player,
		};*/
		enum class State
		{
			Eating,
			Dying,
			Left,
			Right,
			Up,
			Down
		};
		enum class Direction
		{
			Left,
			Right,
			Up,
			Down
		};
						Pacman(const GEX::TextureManager& textures);
						~Pacman() = default;

		unsigned int	getCategory() const override;

		void			playLocalSound(CommandQueue& commands, SoundEffectID effect);

		/*bool			isPlayer() const;*/
		bool			isDead();
		void			die();
		int				getLives();

		void			setState(State state);
		void			setDirection(Direction direction);

		void			collectCherry();
		int				getScore();
		sf::FloatRect	getBoundingBox() const override;

		bool			isMarkedForRemoval() const override;

	private:
		void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		void			updateCurrent(sf::Time dt, CommandQueue& commands) override;
		void			updateStates();
		void			updateDirection();
		void			updateMovementPattern(sf::Time dt);
		float			getMaxSpeed() const;

		void			remove() override;

	private:
		mutable sf::Sprite					sprite_;
		State								state_;
		//PacmanType							type_;
		mutable std::map<State, Animation2> animations_;
		Direction							direction_;
		int									lives_;
		int									score_;

		float								travelDistance_;
		std::size_t							directionIndex_;

		bool								isMarkedForRemoval_;
		bool								hasPlayedExplosionSound_;

		bool								spawnPickup_;
	};
}

