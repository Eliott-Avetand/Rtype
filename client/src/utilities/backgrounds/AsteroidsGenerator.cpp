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
//  File:            AsteroidsGenerator.cpp                                          //
//  Description:     File for the AsteroidsGenerator Class                           //
//                                                                                   //
// ********************************************************************************* //
#include "AsteroidsGenerator.hpp"

Asteroids::Asteroids(sf::Vector2f position, float moveSpeed, Asteroids::asteroidsSpawn asteroidSpawn):
	_sprite(ASTEROID_TEXTURE, position, sf::Vector2f(100, 100), rtype::client::utilities::AnimatedSprite::styleSheet::HORIZONTAL, sf::Vector2i(0, 0), sf::Vector2i(900, 0), 0.1), _isOutside(false), _moveSpeed(moveSpeed),
	_explosionSprite(EXPLOSION_TEXTURE, position, sf::Vector2f(125, 125), rtype::client::utilities::AnimatedSprite::styleSheet::HORIZONTAL, sf::Vector2i(0, 0), sf::Vector2i(125 * 7, 125 * 4), 0.009, false)
{
	_state = Asteroids::asteroidState::ALIVE;
	this->_spawnPosition = asteroidSpawn;

	this->_moveMap = {
		{Asteroids::UP, {1, 1} },
		{Asteroids::DOWN, {-1, -1} },
		{Asteroids::LEFT, {1, 0} },
		{Asteroids::RIGHT, {-1, 0} }
	};
}

void Asteroids::draw(rtype::engine::Engine* engine)
{
	if (_state == Asteroids::asteroidState::ALIVE)
		_sprite.draw(engine);
	if (_state == Asteroids::asteroidState::EXPLOSED)
		_explosionSprite.draw(engine);
}

void Asteroids::update(rtype::engine::Engine* engine)
{
	sf::Time time = _moveClock.getElapsedTime();
	float milliseconds = time.asMilliseconds();
	sf::Vector2f position = _sprite.getPosition();
	float windowHeight = engine->options.getWindowHeight();
	float windowWidth = (float)engine->options.getWindowWidth();

	_sprite.updateAnimation();

	if (milliseconds > 1 && _state == Asteroids::asteroidState::ALIVE) {
		if (this->_moveMap.contains(this->_spawnPosition)) {
			_sprite.setPosition(sf::Vector2f(position.x + _moveSpeed, position.y + _moveSpeed));
			_explosionSprite.setPosition(sf::Vector2f(position.x + _moveSpeed, position.y + _moveSpeed));
			//_sprite.getSprite().move(sf::Vector2f(windowWidth * (this->_moveMap.find(this->_spawnPosition)->second.first * _moveSpeed) / 100, windowHeight * (this->_moveMap.find(this->_spawnPosition)->second.second * _moveSpeed) / 100));
			//_explosionSprite.getSprite().move(sf::Vector2f(windowWidth * (this->_moveMap.find(this->_spawnPosition)->second.first * _moveSpeed) / 100, windowHeight * (this->_moveMap.find(this->_spawnPosition)->second.second * _moveSpeed) / 100));
			_moveClock.restart();
		}
	}
	if (_state == Asteroids::asteroidState::EXPLOSED) {
		_explosionSprite.updateAnimation();

		if (milliseconds > 600)
			_state = Asteroids::asteroidState::DESTROYED;
	}
}


void Asteroids::destroy(void)
{
	_state = Asteroids::asteroidState::EXPLOSED;
	_moveClock.restart();
}

Asteroids::asteroidState Asteroids::getState(void)
{
	return _state;
}

bool Asteroids::isOutside(size_t windowWidth, size_t windowHeight)
{
	sf::Vector2f position = _sprite.getPosition();

	if (position.x > windowWidth || position.y > windowHeight)
		return true;
	return false;
}

bool Asteroids::isHover(sf::Vector2f mousePosition)
{
	return _sprite.getSprite().getGlobalBounds().contains(mousePosition);
}

std::vector<Asteroids> &rtype::client::utilities::AsteroidsGenerator::getAllAsteroids(void)
{
	return this->_allAsteroids;
}

rtype::client::utilities::AsteroidsGenerator::AsteroidsGenerator(void) :
	_sound(ASTEROID_SOUND)
{
	_allAsteroids = {};

	_spawnPosition = Asteroids::UP;
	_canSpawn = false;
	_destroyAll = false;
	_destroyedAsteroids = 0;
	_minTimeSpawn = 0.1;
	_maxTimeSpawn = 1.5;
	_randomTimeSpawn = rtype::client::utilities::RandomNumbers::randomFloat(_minTimeSpawn, _maxTimeSpawn);
}

void rtype::client::utilities::AsteroidsGenerator::update(rtype::engine::Engine* engine)
{
	sf::Time time = _spawnClock.getElapsedTime();
	float seconds = time.asSeconds();

	if (seconds > _randomTimeSpawn && !_destroyAll && _canSpawn) {
		_allAsteroids.push_back(Asteroids(sf::Vector2f(rtype::client::utilities::RandomNumbers::randomFloat(-700, (float)engine->options.getWindowWidth()), -50), rtype::client::utilities::RandomNumbers::randomFloat(5, 20), this->_spawnPosition));

		_randomTimeSpawn = rtype::client::utilities::RandomNumbers::randomFloat(_minTimeSpawn, _maxTimeSpawn);
		_spawnClock.restart();
	}

	if (seconds > 0.1 && _destroyAll && isAsteroids()) {
		for (unsigned int i = 0; i < _allAsteroids.size(); i++) {

			if (_allAsteroids[i].getState() == Asteroids::asteroidState::ALIVE) {
				_sound.playSound(engine->options.music.isSoundActive());
				_allAsteroids[i].destroy();
				_spawnClock.restart();
				break;
			}
		}
	}

	for (unsigned int i = 0; i < _allAsteroids.size(); i++) {
		_allAsteroids[i].update(engine);

		if (_allAsteroids[i].isOutside(engine->options.getWindowWidth(), engine->options.getWindowHeight()) || _allAsteroids[i].getState() == Asteroids::asteroidState::DESTROYED) {
			_allAsteroids.erase(_allAsteroids.begin() + i);
		}
	}
}

void rtype::client::utilities::AsteroidsGenerator::eventUpdate(sf::Event event, rtype::engine::Engine* engine)
{
	for (unsigned int i = 0; i < _allAsteroids.size(); i++) {

		if (_allAsteroids[i].isHover(engine->window.mapPixelToCoords(sf::Mouse::getPosition())) && event.type == sf::Event::MouseButtonPressed && _allAsteroids[i].getState() == Asteroids::asteroidState::ALIVE) {
			_sound.playSound(engine->options.music.isSoundActive());
			_allAsteroids[i].destroy();
			_destroyedAsteroids++;
		}
	}
}

void rtype::client::utilities::AsteroidsGenerator::drawAsteroids(rtype::engine::Engine* engine)
{
	for (unsigned int i = 0; i < _allAsteroids.size(); i++)
		_allAsteroids[i].draw(engine);
}


bool rtype::client::utilities::AsteroidsGenerator::isAsteroids(void)
{
	if (_allAsteroids.size() > 0)
		return true;
	return false;
}

void  rtype::client::utilities::AsteroidsGenerator::toggleAsteroidsSpawn(bool canSpawn)
{
	_canSpawn = canSpawn;
}

void rtype::client::utilities::AsteroidsGenerator::destroyAllAsteroids(void)
{
	_destroyAll = true;
	_spawnClock.restart();
}

int rtype::client::utilities::AsteroidsGenerator::getDestroyedAsteroids(void)
{
	return _destroyedAsteroids;
}

void rtype::client::utilities::AsteroidsGenerator::manageAsteroidsSpawnTime(float minTime, float maxTime)
{
	_minTimeSpawn = minTime;
	_maxTimeSpawn = maxTime;
}

void rtype::client::utilities::AsteroidsGenerator::setAsteroidsPositionSpawn(Asteroids::asteroidsSpawn spawnPosition)
{
	this->_spawnPosition = spawnPosition;
}
