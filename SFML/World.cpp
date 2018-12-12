/**
* @file
* World.cpp
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
#include "World.h"
#include "Category.h"
#include "DataTables.h"
#include "ParticleNode.h"
#include "SoundNode.h"
#include "Pacman.h"
#include <iostream>

namespace GEX {
	World::World(sf::RenderTarget& outputTarget, SoundPlayer& sounds)
		: target_(outputTarget),
		worldView_(outputTarget.getDefaultView()),
		textures_(),
		sceneGraph_(),
		sceneLayer_(),
		sounds_(sounds),
		worldBounds_(0.f, 0.f, worldView_.getSize().x, worldView_.getSize().y),
		playerSpawnPosition_(0.f, worldView_.getSize().y / 2.f),
		cherrySpawnPosition_(worldView_.getSize().x - 50.f, worldView_.getSize().y / 2.f),
		ghostSpawnPosition_(worldView_.getSize().x - 150.f, worldView_.getSize().y / 2.f),
		count_(0),
		player_(nullptr),
		ghost_(nullptr),
		animationPlaying_(false),
		score_(0)
	{
		sceneTexture_.create(target_.getSize().x, target_.getSize().y);

		loadTextures();
		buildScene();
		lives_ = player_->getLives();

		// prepare the view
		worldView_.setCenter(worldView_.getSize().x / 2.f, worldBounds_.height - worldView_.getSize().y / 2.f);
	}

	void World::update(sf::Time dt, CommandQueue& commands)
	{
		player_->setVelocity(0.f, 0.f);

		// reset player if they die
		if (player_->isDead())
			animationPlaying_ = true;
		if (!player_->isDead() && animationPlaying_ == true) 
		{
			animationPlaying_ = false;
			resetPlayer();
		}

		turnGhostAround();

		// run all the commands in the command queue
		while (!commandQueue_.isEmpty())
			sceneGraph_.onCommand(commandQueue_.pop(), dt);

		handleCollisions();
		sceneGraph_.removeWrecks();

		adaptPlayerVelocity();
		sceneGraph_.update(dt, commands);
		adaptPlayerPosition();

		updateTexts();

		//spawnEnemies();
		//updateSounds();
	}

	void World::updateSounds()
	{
		sounds_.setListenerPosition(player_->getWorldPosition());
		sounds_.removeStoppedSounds();
	}

	void World::adaptPlayerPosition()
	{
		const float BORDER_DISTANCE = 40.f;
		sf::FloatRect viewBounds(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());

		sf::Vector2f position = player_->getPosition();
		position.x = std::max(position.x, viewBounds.left + BORDER_DISTANCE);
		position.x = std::min(position.x, viewBounds.left + viewBounds.width - BORDER_DISTANCE);

		position.y = std::max(position.y, viewBounds.top + BORDER_DISTANCE);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - BORDER_DISTANCE);

		player_->setPosition(position);
	}

	void World::adaptPlayerVelocity()
	{
		sf::Vector2f velocity = player_->getVelocity();
		if (velocity.x != 0.f && velocity.y != 0.f)
			player_->setVelocity(velocity / std::sqrt(2.f));
	}

	sf::FloatRect World::getViewBounds() const
	{
		return sf::FloatRect(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());
	}

	sf::FloatRect World::getBattlefieldBounds() const
	{
		// change bounds a bit to make ghost bounce at edges of screens
		sf::FloatRect bounds = getViewBounds();
		bounds.top += 20.f;
		bounds.height -= 40.f;
		return bounds;
	}

	void World::draw()
	{
		if (PostEffect::isSupported())
		{
			// apply effects
			sceneTexture_.clear();
			sceneTexture_.setView(worldView_);
			sceneTexture_.draw(sceneGraph_);
			sceneTexture_.display();
			bloomEffect_.apply(sceneTexture_, target_);
		}
		else
		{
			target_.setView(worldView_);
			target_.draw(sceneGraph_);
		}
	}

	CommandQueue & World::getCommandQueue()
	{
		return commandQueue_;
	}

	bool World::hasAlivePlayer() const
	{
		// if player runs out of lives, game is over
		return !player_->getLives() == 0;//!player_->isDestroyed();
	}

	bool World::hasPlayerReachedEnd() const
	{
		return false; //!worldBounds_.contains(player_->getPosition());
	}

	void World::resetPlayer()
	{
		// resets player to spawn position
		player_->setPosition(playerSpawnPosition_);
	}

	void World::handleCollisions()
	{
		// build a list of colliding pairs of SceneNodes
		std::set<SceneNode::Pair> collisionPairs;
		sceneGraph_.checkSceneCollision(sceneGraph_, collisionPairs);

		for (SceneNode::Pair pair : collisionPairs)
		{
			// if player and ghost collide, player dies and loses a life
			if (matchesCategories(pair, Category::Type::Player, Category::Type::Ghost))
			{
				if (player_->getLives() > 0)
					player_->die();
			}

			// if player and cherry collides, player is reset and gains 200 score
			if (matchesCategories(pair, Category::Type::Player, Category::Type::Cherry))
			{
				player_->collectCherry();
				resetPlayer();
			}
		}
	}

	bool World::matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
	{
		const unsigned int category1 = colliders.first->getCategory();
		const unsigned int category2 = colliders.second->getCategory();

		if (type1 & category1 && type2 & category2)
		{
			return true;
		}
		else if (type1 & category2 && type2 & category1)
		{
			std::swap(colliders.first, colliders.second);
			return true;
		}
		else
			return false;
	}

	void World::turnGhostAround()
	{
		Command command;
		command.category = Category::Type::Ghost;
		command.action = derivedAction<Entity>([this](Entity& e, sf::Time dt)
		{
			if (!getBattlefieldBounds().intersects(e.getBoundingBox()))
			{
				if (ghost_->getDirection() == Ghost::State::Up)
				{
					ghost_->setVelocity(0.f, 100.f);
					ghost_->setDirection(Ghost::State::Down);
					return;
				}
				if (ghost_->getDirection() == Ghost::State::Down)
				{
					ghost_->setVelocity(0.f, -100.f);
					ghost_->setDirection(Ghost::State::Up);
					return;
				}
				std::cout << "X RANGE" << getBattlefieldBounds().left << " - " << getBattlefieldBounds().left + getBattlefieldBounds().width << "\n";
				std::cout << "Y RANGE" << getBattlefieldBounds().top << " - " << getBattlefieldBounds().top + getBattlefieldBounds().height << "\n";
				std::cout << "Entity Box X:" << e.getBoundingBox().left << " - " << e.getBoundingBox().left + e.getBoundingBox().width << "\n";
				std::cout << "Entity Box y:" << e.getBoundingBox().top << " - " << e.getBoundingBox().top + e.getBoundingBox().height << "\n";
				//e.remove();
			}
		});

		commandQueue_.push(command);
	}

	void World::updateTexts()
	{
		scoreText_->setText("Score: " + std::to_string(player_->getScore()));
		livesText_->setText("Lives: " + std::to_string(player_->getLives()));
	}

	void World::loadTextures()
	{
		textures_.load(TextureID::Ghost, "Media/Textures/Atlas.png");
		textures_.load(TextureID::Player, "Media/Textures/Atlas.png");
		textures_.load(TextureID::Background, "Media/Textures/background.png");
		textures_.load(TextureID::Cherry, "Media/Textures/Atlas.png");
	}

	void World::buildScene()
	{
		// Initalize layers
		for (int i = 0; i < LayerCount; i++)
		{
			auto category = (i == UpperAir) ? Category::Type::AirSceneLayer : Category::Type::None;
			SceneNode::Ptr layer(new SceneNode(category));
			sceneLayer_.push_back(layer.get());
			sceneGraph_.attachChild(std::move(layer));
		}


		// Particle Systems
		//std::unique_ptr<ParticleNode> smoke(new ParticleNode(Particle::Type::Smoke, textures_));
		//sceneLayer_[LowerAir]->attachChild(std::move(smoke));

		//std::unique_ptr<ParticleNode> fire(new ParticleNode(Particle::Type::Propellant, textures_));
		//sceneLayer_[LowerAir]->attachChild(std::move(fire));

		//// sound effects
		//std::unique_ptr<SoundNode> sNode(new SoundNode(sounds_));
		//sceneGraph_.attachChild(std::move(sNode));

		// background
		sf::Texture& texture = textures_.get(TextureID::Background);
		sf::IntRect textureRect(worldBounds_);
		texture.setRepeated(true);

		std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
		backgroundSprite->setPosition(worldBounds_.left, worldBounds_.top);
		sceneLayer_[Background]->attachChild(std::move(backgroundSprite));

		// cherry
		/*sf::Texture& cherryTexture = textures_.get(TextureID::Cherry);
		sf::IntRect cherryTextureRect(1, 1, 40, 40);

		std::unique_ptr<SpriteNode> cherrySprite(new SpriteNode(cherryTexture, cherryTextureRect));
		cherrySprite->setPosition(cherrySpawnPosition_);
		sceneLayer_[UpperAir]->attachChild(std::move(cherrySprite));*/

		std::unique_ptr<Cherry> cherrySprite(new Cherry(textures_));
		cherrySprite->setPosition(cherrySpawnPosition_);
		sceneLayer_[UpperAir]->attachChild(std::move(cherrySprite));

		// add player aircraft & game objects
		std::unique_ptr<Pacman> leader(new Pacman(textures_));
		leader->setPosition(playerSpawnPosition_);
		player_ = leader.get();
		sceneLayer_[UpperAir]->attachChild(std::move(leader));

		// add ghost
		std::unique_ptr<Ghost> ghost(new Ghost(textures_));
		ghost->setPosition(ghostSpawnPosition_);
		ghost_ = ghost.get();
		sceneLayer_[UpperAir]->attachChild(std::move(ghost));

		// score and lives text
		std::unique_ptr<TextNode> scoreTxt(new TextNode(""));
		scoreTxt->setText("Score: " + std::to_string(player_->getScore()));
		scoreTxt->setPosition(50, 10);
		scoreTxt->setSize(30);
		scoreText_ = scoreTxt.get();
		sceneLayer_[UpperAir]->attachChild((std::move(scoreTxt)));

		std::unique_ptr<TextNode> lifeTxt(new TextNode(""));
		lifeTxt->setText("Lives: " + std::to_string(player_->getLives()));
		lifeTxt->setPosition(50, 60);
		lifeTxt->setSize(30);
		livesText_ = lifeTxt.get();
		sceneLayer_[UpperAir]->attachChild((std::move(lifeTxt)));

		// add enemy aircraft
		//addEnemies();
	}


}
