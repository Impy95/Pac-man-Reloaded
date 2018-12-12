#include "Entity.h"
#include <cassert>


namespace GEX {
	Entity::Entity()
	{
		isAlive_ = true;
	}

	void Entity::setVelocity(sf::Vector2f velocity)
	{
		velocity_ = velocity;
	}

	void Entity::setVelocity(float vx, float vy)
	{
		velocity_.x = vx;
		velocity_.y = vy;
	}

	sf::Vector2f Entity::getVelocity() const
	{
		return velocity_;
	}

	void Entity::accelerate(sf::Vector2f velocity)
	{
		velocity_ += velocity;
	}

	void Entity::accelerate(float vx, float vy)
	{
		velocity_.x += vx;
		velocity_.y += vy;
	}

	void Entity::updateLocation(sf::Vector2f location)
	{
		this->setPosition(this->getPosition() + location);
	}

	void Entity::destroy()
	{
		isAlive_ = false;
	}

	bool Entity::isDestroyed() const
	{
		return !isAlive_;
	}

	void Entity::remove()
	{
		destroy();
	}

	void Entity::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		move(velocity_ * dt.asSeconds());
	}

}
