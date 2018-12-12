/**
* @file
* Ghost.h
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
#include "Animation2.h"
#include <map>

namespace GEX
{
	class TextureManager;

	class Ghost : public Entity
	{
	public:
		enum class State
		{
			Up,
			Down
		};
		enum class Direction
		{
			Up,
			Down
		};

		Ghost(const GEX::TextureManager& textures);
		~Ghost() = default;
		
		unsigned int	getCategory() const override;
		void			setDirection(State direction);
		State			getDirection();
		sf::FloatRect	getBoundingBox() const override;

	private:
		void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		void			updateCurrent(sf::Time dt, CommandQueue& commands) override;
		void			updateStates();
		void			updateMovementPattern(sf::Time dt);

		void			remove() override;

	private:
		mutable sf::Sprite					sprite_;
		State								state_;
		mutable std::map<State, Animation2> animations_;
		Direction							direction_;

	};
}

