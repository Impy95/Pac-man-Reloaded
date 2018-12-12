/**
* @file
* Cherry.cpp
* @author
* Vaughn Rowse 2018
* @version 1.0
*
* @section DESCRIPTION
* Part of Test 1
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

#include "Cherry.h"
#include "DataTables.h"
#include "Utility.h"

namespace GEX
{
	namespace {
		const CherryData TABLE = initalizeCherryData();
	}

	Cherry::Cherry(const TextureManager& textures)
		: Entity()
		, sprite_(textures.get(TABLE.texture), TABLE.textureRect)
	{
		//centerOrigin(sprite_);
	}

	unsigned Cherry::getCategory() const
	{
		return Category::Cherry;
	}

	sf::FloatRect Cherry::getBoundingBox() const
	{
		return getWorldTransform().transformRect(sprite_.getGlobalBounds());
	}

	void Cherry::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite_, states);
	}
}
