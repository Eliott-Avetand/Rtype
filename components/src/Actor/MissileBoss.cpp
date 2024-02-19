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
//  File:            MissileBoss.cpp                                                 //
//  Description:     Implement functions of the Missile class                        //
//                                                                                   //
// ********************************************************************************* //
#include "MissileBoss.hpp"

rtype::actor::MissileBoss::MissileBoss(sf::Vector2f position, int speed, int damage, std::string sprite)
{
	_isDestroy = false;
	_position = { (int)position.x, (int)position.y };

	this->_id = "";

	_speed = speed;
	_damage = damage;
    _sprite = std::make_unique<rtype::client::utilities::AnimatedSprite>(sprite, sf::Vector2f(1500.0f, 550.0f), sf::Vector2f(24.0f, 24.0f), rtype::client::utilities::AnimatedSprite::HORIZONTAL, sf::Vector2i(static_cast<int>(550), static_cast<int>(2061)), sf::Vector2i(static_cast<int>(550 + 24 * 4), static_cast<int>(2061)), 0.05f, true);

	if (!_sprite)
		throw rtype::settings::Errors("Not enough memories", "MemoryError:");

	_sprite->getSprite().scale(4, 4);

	this->_bounds = this->_sprite->getSprite().getGlobalBounds();
}

void rtype::actor::MissileBoss::draw(rtype::engine::Engine* engine)
{
	_sprite->draw(engine);
}

void rtype::actor::MissileBoss::update(void)
{
	sf::Time time = _moveClock.getElapsedTime();
	float milliseconds = time.asMilliseconds();

	if (milliseconds > 10) {
		_position.first -= _speed;
		_sprite->setPosition(sf::Vector2f(_position.first, _position.second));
		_moveClock.restart();
	}

	#ifdef SERVER
		if (_position.first >= 3000 || _position.first <= -500)
			_isDestroy = true;
	#endif // SERVER


	#ifdef CLIENT
	if (rtype::engine::Engine::getInstance()->getClient()) {
		if (!rtype::engine::Engine::getInstance()->getClient()->getIsOnline()) {
			if (_position.first >= 3000 || _position.first <= -500)
				_isDestroy = true;
		}
	}
	#endif // CLIENT

    _sprite->updateAnimation();
}

int rtype::actor::MissileBoss::getSpeed(void)
{
	return _speed;
}

int rtype::actor::MissileBoss::getDamage(void)
{
	return _damage;
}

bool rtype::actor::MissileBoss::isDestroyed(void)
{
	return _isDestroy;
}

sf::Sprite &rtype::actor::MissileBoss::getSprite(void)
{
	return this->_sprite->getSprite();
}

sf::FloatRect rtype::actor::MissileBoss::getBounds(void)
{
	return this->_bounds;
}

std::string rtype::actor::MissileBoss::getId(void)
{
	return this->_id;
}

void rtype::actor::MissileBoss::setId(std::string id)
{
	this->_id = id;
}

void rtype::actor::MissileBoss::destroy()
{
	this->_isDestroy = true;
}

sf::Vector2f rtype::actor::MissileBoss::getPosition(void)
{
	return sf::Vector2f(this->_position.first, this->_position.second);
}

void rtype::actor::MissileBoss::setPosition(sf::Vector2f position)
{
	this->_position = { static_cast<int>(position.x), static_cast<int>(position.y) };
}
