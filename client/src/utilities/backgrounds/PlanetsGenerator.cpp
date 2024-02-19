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
//  File:            PlanetsGenerator.cpp                                            //
//  Description:     File for the PlanetsGenerator Class                             //
//                                                                                   //
// ********************************************************************************* //
#include "PlanetsGenerator.hpp"

Planet::Planet(std::string spriteSheet, int numberPlanet, sf::Vector2f position, float size, float speed, bool canMove) :
	_sprite(std::string(spriteSheet) + std::to_string(numberPlanet) + ".png", position, sf::Vector2f(size, size),
		rtype::client::utilities::AnimatedSprite::styleSheet::HORIZONTAL, sf::Vector2i(0, 0), sf::Vector2i(size * 19, 0))
{
	_size = size;
	_canMove = canMove;
	_speed = speed;

	std::vector<std::string> planetsTexture = { GAS_PLANET_TEXTURE, ICE_WORLD_TEXTURE, BLACK_HOLE_TEXTURE, GALAXY_TEXTURE, BIG_STAR_TEXTURE };
	std::vector<int> planetsSize = { GAS_PLANET_SIZE, ICE_WORLD_SIZE, BLACK_HOLE_SIZE, GALAXY_SIZE, BIG_STAR_SIZE };

	for (size_t i = 0; i < planetsTexture.size(); i++) {
		if (planetsTexture[i] == spriteSheet) {
			_sprite.changeTextureRect(sf::IntRect({ 0, 0 }, { planetsSize[i], planetsSize[i] }));
			_sprite.updateFrameAnimation(rtype::client::utilities::AnimatedSprite::styleSheet::HORIZONTAL, rtype::client::utilities::AnimatedSprite::direction::RIGHT, sf::Vector2i(0, 0), sf::Vector2i(planetsSize[i] * 19, 0), 0.3);
		}
	}

	_sprite.getSprite().setScale(size, size);
}

Planet::Planet(sf::Vector2f position, float size, float speed, bool canMove):
	_sprite(std::string(GAS_PLANET_TEXTURE) + "1.png", position, sf::Vector2f(GAS_PLANET_SIZE, GAS_PLANET_SIZE),
		rtype::client::utilities::AnimatedSprite::styleSheet::HORIZONTAL, sf::Vector2i(0, 0), sf::Vector2i(GAS_PLANET_SIZE * 19, 0))
{
	std::vector<int> planetsNumbers = { GAS_PLANETS_MAX , ICE_WORLD_MAX , BLACK_HOLE_MAX , GALAXY_MAX , BIG_STARS_MAX };
	std::vector<std::string> planetsTexture = { GAS_PLANET_TEXTURE, ICE_WORLD_TEXTURE, BLACK_HOLE_TEXTURE, GALAXY_TEXTURE, BIG_STAR_TEXTURE };
	std::vector<int> planetsSize = { GAS_PLANET_SIZE, ICE_WORLD_SIZE, BLACK_HOLE_SIZE, GALAXY_SIZE, BIG_STAR_SIZE };

	int randomPlanet = rtype::client::utilities::RandomNumbers::randomInteger(0, 4);
	int randomPlanetTexture = rtype::client::utilities::RandomNumbers::randomInteger(1, planetsNumbers[randomPlanet]);

	_size = size;
	_canMove = canMove;
	_speed = speed;

	_sprite.changeTexture(std::string(planetsTexture[randomPlanet]) + std::to_string(randomPlanetTexture) + ".png");
	_sprite.changeTextureRect(sf::IntRect({ 0, 0 }, { planetsSize[randomPlanet], planetsSize[randomPlanet] }));
	_sprite.updateFrameAnimation(rtype::client::utilities::AnimatedSprite::styleSheet::HORIZONTAL, rtype::client::utilities::AnimatedSprite::direction::RIGHT, sf::Vector2i(0, 0), sf::Vector2i(planetsSize[randomPlanet] * 19, 0), 0.3);
	_sprite.getSprite().setScale(size, size);
}

void Planet::draw(rtype::engine::Engine* engine)
{
	_sprite.draw(engine);
}

void Planet::update(rtype::engine::Engine* engine)
{
	sf::Time time = _moveClock.getElapsedTime();
	float milliseconds = time.asMilliseconds();

	if (milliseconds > 50) {
		if (_canMove) {
			_sprite.setPosition(sf::Vector2f(_sprite.getPosition().x - _speed * _size, _sprite.getPosition().y));
			_moveClock.restart();
		}
	}

	_sprite.updateAnimation();
}

bool Planet::isOutside(size_t windowWidth, size_t windowHeight)
{
	if (_sprite.getPosition().x < -200)
		return true;
	return false;
}

rtype::client::utilities::PlanetsGenerator::PlanetsGenerator()
{
	_allPlanets = {};

	_canSpawn = false;
	_minTimeSpawn = 45;
	_maxTimeSpawn = 60;
	_randomTimeSpawn = rtype::client::utilities::RandomNumbers::randomFloat(_minTimeSpawn, _maxTimeSpawn);
}

void rtype::client::utilities::PlanetsGenerator::update(rtype::engine::Engine* engine)
{
	sf::Time time = _spawnClock.getElapsedTime();
	float seconds = time.asSeconds();

	if (seconds > _randomTimeSpawn && _canSpawn) {
		_allPlanets.push_back(Planet(sf::Vector2f(engine->options.getWindowWidth() + 500, rtype::client::utilities::RandomNumbers::randomFloat(0, engine->options.getWindowHeight())), rtype::client::utilities::RandomNumbers::randomFloat(1, 3)));

		_randomTimeSpawn = rtype::client::utilities::RandomNumbers::randomFloat(_minTimeSpawn, _maxTimeSpawn);
		_spawnClock.restart();
	}

	for (unsigned int i = 0; i < _allPlanets.size(); i++) {
		_allPlanets[i].update(engine);

		if (_allPlanets[i].isOutside(engine->options.getWindowWidth(), engine->options.getWindowHeight())) {
			_allPlanets.erase(_allPlanets.begin() + i);
		}
	}
}

void rtype::client::utilities::PlanetsGenerator::drawPlanets(rtype::engine::Engine* engine)
{
	for (unsigned int i = 0; i < _allPlanets.size(); i++) {
		_allPlanets[i].draw(engine);
	}
}

void rtype::client::utilities::PlanetsGenerator::togglePlanetsSpawn(bool canSpawn)
{
	_canSpawn = canSpawn;
}

void rtype::client::utilities::PlanetsGenerator::managePlanetsSpawnTime(float minTime, float maxTime)
{
	_maxTimeSpawn = maxTime;
	_minTimeSpawn = minTime;
}

void rtype::client::utilities::PlanetsGenerator::forceSpawnPlanets(std::string spriteSheet, int numberPlanet, sf::Vector2f position, float size, float speed, bool canMove)
{
	_allPlanets.push_back(Planet(spriteSheet, numberPlanet, position, size, speed, canMove));

	_randomTimeSpawn = rtype::client::utilities::RandomNumbers::randomFloat(_minTimeSpawn, _maxTimeSpawn);
	_spawnClock.restart();
}