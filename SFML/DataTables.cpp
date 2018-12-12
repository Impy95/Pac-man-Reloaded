/**
* @file
* DataTables.cpp
* @author
* Vaughn Rowse 2018
* @version 1.0
*
* @section DESCRIPTION
*
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
#include "DataTables.h"
#include "JsonFrameParser.h"

namespace GEX
{

	PacmanData initalizePacmanData()
	{
		PacmanData data;

		JsonFrameParser frames = JsonFrameParser("Media/Textures/atlas.json");

		data.texture = TextureID::Player;
		data.animations[Pacman::State::Eating].addFrameSet(frames.getFramesFor("pacman walk"));
		data.animations[Pacman::State::Eating].setDuration(sf::seconds(1.f));
		data.animations[Pacman::State::Eating].setRepeat(true);

		data.animations[Pacman::State::Dying].addFrameSet(frames.getFramesFor("die"));
		data.animations[Pacman::State::Dying].setDuration(sf::seconds(1.f));
		data.animations[Pacman::State::Dying].setRepeat(false);

		return data;
	}

	CherryData initalizeCherryData()
	{
		CherryData data;
		//JsonFrameParser frames = JsonFrameParser("Media/Textures/Atlas.json");
		data.texture = TextureID::Cherry;
		data.textureRect = sf::IntRect(1, 1, 40, 40);

	
		return data;
	}

	GhostData initalizeGhostData()
	{
		GhostData data;

		JsonFrameParser frames = JsonFrameParser("Media/Textures/atlas.json");

		data.texture = TextureID::Ghost;
		data.animations[Ghost::State::Up].addFrameSet(frames.getFramesFor("ghost"));
		data.animations[Ghost::State::Up].setDuration(sf::seconds(1.f));
		data.animations[Ghost::State::Up].setRepeat(true);
		data.animations[Ghost::State::Up].addFrameSet(frames.getFramesFor("ghost (2)"));
		data.animations[Ghost::State::Up].setDuration(sf::seconds(1.f));
		data.animations[Ghost::State::Up].setRepeat(true);
		data.animations[Ghost::State::Down].addFrameSet(frames.getFramesFor("ghost (3)"));
		data.animations[Ghost::State::Down].setDuration(sf::seconds(1.f));
		data.animations[Ghost::State::Down].setRepeat(true);
		data.animations[Ghost::State::Down].addFrameSet(frames.getFramesFor("ghost (4)"));
		data.animations[Ghost::State::Down].setDuration(sf::seconds(1.f));
		data.animations[Ghost::State::Down].setRepeat(true);

		return data;
	}

	std::map<Particle::Type, ParticleData> initalizeParticleData()
	{
		std::map<Particle::Type, ParticleData> data;

		//data[Particle::Type::Propellant].color = sf::Color(255, 255, 50);
		data[Particle::Type::Propellant].color = sf::Color(160, 10, 20);
		data[Particle::Type::Propellant].lifetime = sf::seconds(0.6f);

		data[Particle::Type::Smoke].color = sf::Color(50, 50, 50);
		data[Particle::Type::Smoke].lifetime = sf::seconds(4.f);

		return data;
	}
}
