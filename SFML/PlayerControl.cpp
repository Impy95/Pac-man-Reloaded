#include "PlayerControl.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Category.h"
#include <functional>
#include "Pacman.h"

namespace GEX {
	struct PacmanMover
	{
	public:
		PacmanMover(float vx, float vy)
			: velocity(vx, vy)
			, positionOffset(vx, vy)
		{}
		void operator() (Pacman& pacman, sf::Time) const
		{
			pacman.accelerate(velocity);

			/*if (!pacman.isDead()) {
				if (positionOffset.y < 0 && positionOffset.x == 0) {
					pacman.setDirection(Pacman::Direction::Up);
				}
				else if (positionOffset.y > 0 && positionOffset.x == 0) {
					pacman.setDirection(Pacman::Direction::Down);
				}
				else if (positionOffset.x < 0 && positionOffset.y == 0) {
					pacman.setDirection(Pacman::Direction::Left);
				}
				else if (positionOffset.x > 0 && positionOffset.y == 0) {
					;
					pacman.setDirection(Pacman::Direction::Right);
				}
				pacman.updateLocation(positionOffset);
			}*/
		}
		sf::Vector2f positionOffset;

		sf::Vector2f velocity;
	};
	PlayerControl::PlayerControl()
		: currentMissionStatus_(MissionStatus::MissionRunning)
	{
		// set up bindings
		keyBindings_[sf::Keyboard::Left] = Action::MoveLeft;
		keyBindings_[sf::Keyboard::Right] = Action::MoveRight;
		keyBindings_[sf::Keyboard::Up] = Action::MoveUp;
		keyBindings_[sf::Keyboard::Down] = Action::MoveDown;


		// set up action bindings
		initalizeActions();

		for (auto& pair : actionBindings_)
			pair.second.category = Category::Player;

		//actionBindings_[Action::RR].category = Category::EnemyAircraft;
		//actionBindings_[Action::RL].category = Category::EnemyAircraft;


	}

	void PlayerControl::handleEvent(const sf::Event & event, CommandQueue & commands)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			auto found = keyBindings_.find(event.key.code);
			if (found != keyBindings_.end())
			{
				if (!isRealTimeAction(found->second))
					commands.push(actionBindings_[found->second]);
			}
		}
	}

	void PlayerControl::handleRealtimeInput(CommandQueue & commands)
	{
		// itraverse all assigned keys, look up action, generate command
		for (auto pair : keyBindings_)
		{
			if (sf::Keyboard::isKeyPressed(pair.first) && isRealTimeAction(pair.second))
				commands.push(actionBindings_[pair.second]);
		}
	}

	void PlayerControl::setMissionStatus(MissionStatus status)
	{
		currentMissionStatus_ = status;
	}

	MissionStatus PlayerControl::getMissionStatus() const
	{
		return currentMissionStatus_;
	}

	void PlayerControl::initalizeActions()
	{
		const float playerSpeed = 200.f;

		//actionBindings_[Action::MoveLeft].action = derivedAction<Pacman>(PacmanMover(-playerSpeed, 0.f));
		actionBindings_[Action::MoveLeft].action = derivedAction<Pacman>([](Pacman& node, sf::Time dt) 
		{ node.setDirection(Pacman::Direction::Left); });
		//actionBindings_[Action::MoveRight].action = derivedAction<Pacman>(PacmanMover(+playerSpeed, 0.f));
		actionBindings_[Action::MoveRight].action = derivedAction<Pacman>([](Pacman& node, sf::Time dt)
		{ node.setDirection(Pacman::Direction::Right); });
		//actionBindings_[Action::MoveUp].action = derivedAction<Pacman>(PacmanMover(0.f, -playerSpeed));
		actionBindings_[Action::MoveUp].action = derivedAction<Pacman>([](Pacman& node, sf::Time dt)
		{ node.setDirection(Pacman::Direction::Up); });
		//actionBindings_[Action::MoveDown].action = derivedAction<Pacman>(PacmanMover(0.f, +playerSpeed));
		actionBindings_[Action::MoveDown].action = derivedAction<Pacman>([](Pacman& node, sf::Time dt)
		{ node.setDirection(Pacman::Direction::Down); });
	}

	bool PlayerControl::isRealTimeAction(Action action)
	{
		switch (action)
		{
		case Action::MoveLeft:
		case Action::MoveRight:
		case Action::MoveDown:
		case Action::MoveUp:
			return true;
			break;
		default:
			return false;
			break;
		}
	}


}
