//
// The game World
//
#pragma once

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "SceneNode.h"
#include "SpriteNode.h"
#include "TextureManager.h"
#include "Pacman.h"
#include "CommandQueue.h"

#include <vector>
#include "BloomEffect.h"
#include "SoundPlayer.h"

// forward declaration
namespace sf
{
	class RenderTarget;
}
namespace GEX {
	class Ghost;

	class World
	{
	public:
		explicit					World(sf::RenderTarget& outputTarget, SoundPlayer& sounds);

		void						update(sf::Time dt, CommandQueue& commands);
		void						updateSounds();
		void						draw();

		CommandQueue&				getCommandQueue();
		bool						hasAlivePlayer() const;
		bool						hasPlayerReachedEnd() const;
		void						resetPlayer();
	private:
		void						loadTextures();
		void						buildScene();
		void						adaptPlayerPosition();
		void						adaptPlayerVelocity();

		sf::FloatRect				getViewBounds() const;
		sf::FloatRect				getBattlefieldBounds() const;

		void						handleCollisions();
		bool						matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);

		void						turnGhostAround();

		void						updateTexts();

	private:
		enum Layer 
		{
			Background = 0,
			LowerAir,
			UpperAir,
			LayerCount
		};

	private:
		sf::RenderTarget&			target_;
		sf::RenderTexture			sceneTexture_;
		sf::View					worldView_;
		TextureManager				textures_;
		SoundPlayer&				sounds_;

		SceneNode					sceneGraph_;
		std::vector<SceneNode*>		sceneLayer_;

		CommandQueue				commandQueue_;

		sf::FloatRect				worldBounds_;
		sf::Vector2f				playerSpawnPosition_;
		sf::Vector2f				cherrySpawnPosition_;
		sf::Vector2f				ghostSpawnPosition_;

		int							count_;
		Pacman*						player_;
		Ghost*						ghost_;

		//std::vector<SpawnPoint>		enemySpawnPoints_;

		BloomEffect					bloomEffect_;
		bool						animationPlaying_;
		int							score_;
		int							lives_;
		TextNode*					scoreText_;
		TextNode*					livesText_;
	};
}


