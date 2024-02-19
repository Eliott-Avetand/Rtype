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
//  File:            Asteroid.hpp                                                    //
//  Description:     Asteroid Class Definition										 //
//                                                                                   //
// ********************************************************************************* //
#include "Asteroid.hpp"

rtype::actor::Asteroid::Asteroid(sf::Vector2f positions, float size, float speed) :
_sprite(ASTEROID_TEXTURE, sf::Vector2f(1000, 1000), sf::Vector2f(100, 100), rtype::client::utilities::AnimatedSprite::styleSheet::HORIZONTAL, sf::Vector2i(0, 0), sf::Vector2i(900, 0), 0.1),
_explosionSprite(EXPLOSION_TEXTURE, sf::Vector2f(1000, 1000), sf::Vector2f(125, 125), rtype::client::utilities::AnimatedSprite::styleSheet::HORIZONTAL, sf::Vector2i(0, 0), sf::Vector2i(125 * 7, 125 * 4), 0.009, false),
_sound(ASTEROID_SOUND)
{
	this->_positions = positions;

	this->_id = "";

	this->_size = size;
	this->_state = rtype::actor::Asteroid::ALIVE;
	this->_speed = speed;
	this->_moveClock.restart();
	this->_sprite.setScale(size, size);
	this->_explosionSprite.setScale(size, size);
}

rtype::actor::Asteroid::Asteroid() :
_sprite(ASTEROID_TEXTURE, sf::Vector2f(1000, 1000), sf::Vector2f(100, 100), rtype::client::utilities::AnimatedSprite::styleSheet::HORIZONTAL, sf::Vector2i(0, 0), sf::Vector2i(900, 0), 0.1),
_explosionSprite(EXPLOSION_TEXTURE, sf::Vector2f(1000, 1000), sf::Vector2f(125, 125), rtype::client::utilities::AnimatedSprite::styleSheet::HORIZONTAL, sf::Vector2i(0, 0), sf::Vector2i(125 * 7, 125 * 4), 0.009, false),
_sound(ASTEROID_SOUND)
{
	this->_positions = sf::Vector2f(2000, rtype::client::utilities::RandomNumbers::randomInteger(20, 1000));

	this->_id;

	this->_size = rtype::client::utilities::RandomNumbers::randomInteger(1, 3);
	this->_state = rtype::actor::Asteroid::ALIVE;
	this->_speed = rtype::client::utilities::RandomNumbers::randomFloat(1, 2) * this->_size;
	this->_moveClock.restart();
	this->_sprite.setScale(this->_size, this->_size);
	this->_explosionSprite.setScale(this->_size / 2, this->_size / 2);
	this->_collisionBounds.setSize(sf::Vector2f(_sprite.getSprite().getGlobalBounds().width / 4, _sprite.getSprite().getGlobalBounds().height / 4));
	this->_collisionBounds.setOrigin(sf::Vector2f(_sprite.getSprite().getGlobalBounds().width / 8, _sprite.getSprite().getGlobalBounds().height / 8));
}

void rtype::actor::Asteroid::draw(rtype::engine::Engine* engine)
{
	if (this->_state == rtype::actor::Asteroid::ALIVE) {
		this->_sprite.draw(engine);
	}
	if (this->_state == rtype::actor::Asteroid::EXPLOSED)
		this->_explosionSprite.draw(engine);
}

sf::FloatRect rtype::actor::Asteroid::getBounds(void)
{
	return this->_collisionBounds.getGlobalBounds();
}

void rtype::actor::Asteroid::update(rtype::engine::Engine* engine)
{
	_sprite.updateAnimation();

	#ifdef CLIENT
		if (!engine->getClient()->getIsOnline()) {
			if (this->_moveClock.getElapsedTime().asMilliseconds() > 1 && this->_state == rtype::actor::Asteroid::ALIVE) {
				this->_positions.x -= this->_speed;
				_moveClock.restart();
			}
		}
	#endif // CLIENT

	#ifdef SERVER
		if (this->_moveClock.getElapsedTime().asMilliseconds() > 1 && this->_state == rtype::actor::Asteroid::ALIVE) {
			this->_positions.x -= this->_speed;
			_moveClock.restart();
		}
	#endif // SERVER

	if (_state == rtype::actor::Asteroid::EXPLOSED) {
		_explosionSprite.updateAnimation();

		if (this->_moveClock.getElapsedTime().asMilliseconds() > 600)
			_state = rtype::actor::Asteroid::DESTROYED;
	}

	this->_sprite.setPosition(this->_positions);
	this->_explosionSprite.setPosition(this->_positions);
	this->_collisionBounds.setPosition(this->_positions);
}

void rtype::actor::Asteroid::destroy(void)
{
	this->_state = rtype::actor::Asteroid::EXPLOSED;
	#ifdef CLIENT
		this->_sound.playSound(engine::Engine::getInstance()->options.music.isSoundActive());
	#endif // CLIENT
	this->_moveClock.restart();
}

rtype::actor::Asteroid::asteroidState rtype::actor::Asteroid::getState(void)
{
	return this->_state;
}

void rtype::actor::Asteroid::setState(rtype::actor::Asteroid::asteroidState state)
{
	this->_state = state;
}

sf::Sprite& rtype::actor::Asteroid::getSprite(void)
{
	return this->_sprite.getSprite();
}

bool rtype::actor::Asteroid::isMissileCollision(rtype::actor::Missile *missile)
{
	if (this->_collisionBounds.getGlobalBounds().intersects(missile->getSprite().getGlobalBounds()) && this->_state == rtype::actor::Asteroid::ALIVE)
		return true;
	return false;
}

void rtype::actor::Asteroid::setPosition(sf::Vector2f positions)
{
	this->_positions = positions;
}

sf::Vector2f rtype::actor::Asteroid::getPositions(void)
{
	return this->_positions;
}

std::string rtype::actor::Asteroid::getId(void)
{
	return this->_id;
}

void rtype::actor::Asteroid::setId(std::string id)
{
	this->_id = id;
}

float rtype::actor::Asteroid::getSize(void)
{
	return this->_size;
}

float rtype::actor::Asteroid::getSpeed(void)
{
	return this->_speed;
}
