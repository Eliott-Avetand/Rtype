// ********************************************************************************* //
//      __                        __   _  __      _____  __              __ _        //
//     / /   ____   __  __ _____ / /_ (_)/ /__   / ___/ / /_ __  __ ____/ /(_)____   //
//    / /   / __ \ / / / // ___// __// // //_/   \__ \ / __// / / // __  // // __ \  //
//   / /___/ /_/ // /_/ /(__  )/ /_ / // ,<     ___/ // /_ / /_/ // /_/ // // /_/ /  //
//  /_____/\____/ \__,_//____/ \__//_//_/|_|   /____/ \__/ \__,_/ \__,_//_/ \____/   //
//                                                                                   //
// ********************************************************************************* //
//                                                                                   //
//  Project:         R-Type                                                          //
//  File:            Missile.cpp                                                     //
//  Description:     Implement functions of the Missile class                        //
//                                                                                   //
// ********************************************************************************* //

#include "Missile.hpp"

rtype::actor::Missile::Missile(sf::Vector2f position, int speed, int damage, rtype::actor::Missile::missileDirection direction, float angle)
{
	this->_isDestroy = false;
	this->_position = { (int)position.x, (int)position.y };
	this->_direction = direction;
	this->_speed = speed;
	this->_damage = damage;
	this->_angle = angle;

	this->_sprite = std::make_unique<rtype::client::utilities::StaticSprite>(MISSILE_BASIC_TEXTURE, position);
	if (!_sprite)
		throw rtype::settings::Errors("Not enough memories", "MemoryError:");

	this->_sprite->getSprite().scale(3, 3);
	this->_bounds = this->_sprite->getSprite().getGlobalBounds();
	if (_direction == rtype::actor::Missile::missileDirection::LEFT)
		_sprite->getSprite().setRotation(180);
	this->_sprite->getSprite().rotate(angle);
}

void rtype::actor::Missile::draw(rtype::engine::Engine* engine)
{
	_sprite->draw(engine);
}

void rtype::actor::Missile::update()
{
	sf::Time time = _moveClock.getElapsedTime();
	float milliseconds = time.asMilliseconds();

	if (milliseconds > 10) {
		if (_direction == rtype::actor::Missile::missileDirection::RIGHT)
			_position.first += _speed;
		else
			_position.first -= _speed;
		if (this->_angle != 0)
			_position.second += ((this->_angle > 0) ? _speed : -_speed) * std::tan(abs(this->_angle) * (PI / 180.0));
		_sprite->setPosition(sf::Vector2f(_position.first, _position.second));
		_moveClock.restart();
	}

	if (_position.first >= 2500 || _position.first <= -100)
		_isDestroy = true;
}

int rtype::actor::Missile::getSpeed(void)
{
	return _speed;
}

int rtype::actor::Missile::getDamage(void)
{
	return _damage;
}

bool rtype::actor::Missile::isDestroyed(void)
{
	return _isDestroy;
}

sf::Sprite &rtype::actor::Missile::getSprite(void)
{
	return this->_sprite->getSprite();
}

sf::FloatRect rtype::actor::Missile::getBounds(void)
{
	return this->_bounds;
}

std::pair<int,int> rtype::actor::Missile::getPosition(void)
{
	return this->_position;
}
