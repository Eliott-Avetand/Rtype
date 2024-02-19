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
//  File:            PlanetsGenerator.hpp                                            //
//  Description:     File for the PlanetsGenerator Header                            //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <iostream>
#include <string>

#include "Engine.hpp"
#include "AnimatedSprite.hpp"
#include "RandomNumbers.hpp"

#define DEFAULT_MAX 0

#define GAS_PLANETS_MAX 1
#define GAS_PLANET_TEXTURE "./client/assets/sprites/planets/gas_planet_"
#define GAS_PLANET_SIZE 300

#define ICE_WORLD_MAX 1
#define ICE_WORLD_TEXTURE "./client/assets/sprites/planets/ice_planet_"
#define ICE_WORLD_SIZE 100

#define BLACK_HOLE_MAX 1
#define BLACK_HOLE_TEXTURE "./client/assets/sprites/planets/black_hole_"
#define BLACK_HOLE_SIZE 200

#define GALAXY_MAX 1
#define GALAXY_TEXTURE "./client/assets/sprites/planets/galaxy_"
#define GALAXY_SIZE 100

#define BIG_STARS_MAX 1
#define BIG_STAR_TEXTURE "./client/assets/sprites/planets/big_star_"
#define BIG_STAR_SIZE 200

class Planet {
	private:
		rtype::client::utilities::AnimatedSprite _sprite;

		sf::Clock _moveClock;
		bool _isOutside;

		float _speed;
		float _size;

		bool _canMove;

	public:

		/**
		 * @brief Create planet component
		 * @see Constructor is called when this class is create
		 *
		 * @param std::string		Path of the spriteSheet > You can see theses informations in PlanetsGenerator.hpp
		 * @param int		        Number of type of planet you want > You can see theses informations in PlanetsGenerator.hpp
		 * @param sf::Vector2f      Position where the planet will spawn
		 * @param float             Size of the planet
		 * @param float             Speed of the planet
		 * @param bool              True if the planet can move and false otherwise
		 */
		Planet(sf::Vector2f position = sf::Vector2f(0, 0), float size = 1, float speed = 2, bool canMove = true);
		
		/**
		 * @brief Create basic planet component
		 * @see Constructor is called when this class is create
		 *
		 * @param sf::Vector2f      Position where the planet will spawn
		 * @param float             Size of the planet
		 * @param float             Speed of the planet
		 * @param bool              True if the planet can move and false otherwise
		 */
		Planet(std::string spriteSheet = MISSING_TEXTURE, int numberPlanet = 1, sf::Vector2f position = sf::Vector2f(0, 0), float size = 1, float speed = 0.5, bool canMove = true);


		/**
		 * @brief Display planet on the screen
		 * @see This method is called when PlanetsGenerator draw method is called
		 *
		 * @param engine    A reference of the engine given by the engine himself
		 */
		void draw(rtype::engine::Engine* engine);
		
		/**
		 * @brief Update planet on the screen
		 * @see This method is called when PlanetsGenerator update method is called
		 *
		 * @param engine    A reference of the engine given by the engine himself
		 */
		void update(rtype::engine::Engine* engine);

		/**
		 * @brief Check if a planet is outside of the screen
		 * @see This method is called in update of PlanetsGenerator
		 *
		 * @param windowWidth	Width of the window
		 * @param windowHeight	Height of the window
		 *
		 * @return bool			true if the asteroid is outside of the screen and false otherwise
		 */
		bool isOutside(size_t windowWidth, size_t windowHeight);
};

namespace rtype {
	namespace client {
		namespace utilities {
			class PlanetsGenerator {
				private:
					std::vector<Planet> _allPlanets;

					float _minTimeSpawn;
					float _maxTimeSpawn;

					bool _canSpawn;

					sf::Clock _spawnClock;
					float _randomTimeSpawn;

				public:

					/**
					 * @brief Create component which have and manage all planets's informations
					 * @see Constructor is called when this class is create
					 */
					PlanetsGenerator();


					/**
					 * @brief Update all planets on the screen
					 * @see This method is called when BackgroundGenerator's update method is called
					 *
					 * @param engine    A reference of the engine given by the engine himself
					 */
					void update(rtype::engine::Engine* engine);

					/**
					 * @brief Display all planets on the screen
					 * @see This method is called when BackgroundGenerator's draw method is called
					 *
					 * @param engine    A reference of the engine given by the engine himself
					 */
					void drawPlanets(rtype::engine::Engine* engine);


					/**
					 * @brief This method enable and disable planet's spawn
					 * @see This method is called wherever you want
					 */
					void togglePlanetsSpawn(bool canSpawn);

					/**
					 * @brief Manage the minimum and the maximum time that a planet can spawn
					 * @see This methods is called from BackgroundGenerator
					 *
					 * @param float		Minimum time of planets can spawn
					 * @param float		Maximum time of planets can spawn
					 */
					void managePlanetsSpawnTime(float minTime, float maxTime);

					/**
					 * @brief Add a planet when called
					 * @see This methods is called wherever you want
					 *
					 * @param std::string		Path of the spriteSheet > You can see theses informations in PlanetsGenerator.hpp
					 * @param int		        Number of type of planet you want > You can see theses informations in PlanetsGenerator.hpp
					 * @param sf::Vector2f      Position where the planet will spawn
					 * @param float             Size of the planet
					 * @param float             Speed of the planet
					 * @param bool              True if the planet can move and false otherwise
					 */
					void forceSpawnPlanets(std::string spriteSheet, int numberPlanet, sf::Vector2f position, float size, float speed, bool canMove);
			};
		}
	}
}