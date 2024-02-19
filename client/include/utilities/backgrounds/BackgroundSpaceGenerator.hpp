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
//  File:            BackgroundSpaceGenerator.cpp                                    //
//  Description:     BackgroundSpaceGenerator File Header                            //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "StaticSprite.hpp"
#include "RandomNumbers.hpp"
#include "AsteroidsGenerator.hpp"
#include "PlanetsGenerator.hpp"

#define SPACE_TEXTURE "./client/assets/sprites/spaces/backgrounds/"
#define STARS_TEXTURE "./client/assets/sprites/spaces/stars/"
#define DUST_TEXTURE "./client/assets/sprites/spaces/dusts/"

#define STARS_1_INDEX 0
#define STARS_2_INDEX 1

#define MAX_SPACE_TEXTURES 1
#define MAX_STARS_TEXTURES 2
#define MAX_DUST_TEXTURES 1

#define ASTEROID_TEXTURE "./client/assets/sprites/planets/asteroids_1.png"

namespace rtype {
	namespace client {
		namespace utilities {
			class BackgroundSpaceGenerator {
			private:
				enum starsState {
					STATIC,
					CHANGE
				};

				sf::Clock _starsClock;
				float _speed;
				float _starsTimeDisplay;
				int _starsDisplay;
				rtype::client::utilities::BackgroundSpaceGenerator::starsState _starsState;

				bool _isBackgroundMoving;
				sf::Clock _backgroundMoveClock;

				std::unique_ptr<rtype::client::utilities::StaticSprite> _backgroundImage;
				std::vector<rtype::client::utilities::StaticSprite> _backgroundDefile;
				std::vector<rtype::client::utilities::StaticSprite> _starsImage;
				std::vector<rtype::client::utilities::StaticSprite> _dustImage;
				std::vector<std::unique_ptr<rtype::client::utilities::AnimatedSprite>> _backgroundPlanets;

				rtype::client::utilities::AsteroidsGenerator _asteroidsGenerator;
				rtype::client::utilities::PlanetsGenerator _planetsGenerator;

				/**
				 * @brief Create and initialize background image
				 * @see This method is called from init of the scene
				 *
				 * @param engine    A reference of the engine given by the engine himself
				 * @param backgroundName    Name of the background
				 */
				void createBackground(rtype::engine::Engine* engine, std::string backgroundName);

				/**
				 * @brief Draw all elements which in the background
				 * @see This method is called from render of the scene
				 *
				 * @param engine    A reference of the engine given by the engine himself
				 */
				void displayBackground(rtype::engine::Engine* engine);


				/**
				 * @brief Update all elements which in the background
				 * @see This method is called from update of the scene
				 *
				 * @param engine    A reference of the engine given by the engine himself
				 */
				void updateBackground(rtype::engine::Engine* engine);


				/**
				 * @brief Update clock of stars and there opacity
				 * @see This method is called from update of the scene
				 */
				void updateStars(void);

				/**
				 * @brief Swipe opacity of all 2 layers of stars
				 * @see This method is called from updateStars
				 */
				void swipeStars(void);
			public:

				/**
				 * @brief Create component which have and manage all backgorund's informations
				 * @see Constructor is called when this class is create
				 */
				BackgroundSpaceGenerator();
				BackgroundSpaceGenerator(std::string backgroundName);

				/**
				 * @brief Update and handle all informations of the background
				 * @see This method is called every events update
				 *
				 * @param sf::event All events of the program
				 * @param engine    A reference of the engine given by the engine himself
				 */
				void eventUpdate(sf::Event event, rtype::engine::Engine* engine);

				/**
				 * @brief Update all informations of the background
				 * @see This method is called when update
				 *
				 * @param float     delta Time of the program
				 * @param engine    A reference of the engine given by the engine himself
				 */
				void update(rtype::engine::Engine* engine);

				/**
				 * @brief Draw all elements of the background
				 * @see This method is called every render
				 *
				 * @param engine    A reference of the engine given by the engine himself
				 */
				void display(rtype::engine::Engine* engine);


				/**
				 * @brief Toggle asteroid to enable or disable there spawn
				 * @see This method is called wherever you want
				 *
				 * @param bool      true if asteroids can spawn and false otherwise
				 */
				void toggleAsteroidsSpawn(bool canSpawn);

				/**
				 * @brief This method enable and disable background's moving state
				 * @see This method is called wherever you want
				 *
				 * @param bool      true if the background can move and false otherwise
				 */
				void toggleBackgroundMove(bool canMove);

				void setBackgroundSpeed(float speedMultiplicator);

				/**
				 * @brief Destroy all asteroids remained
				 * @see This method is called wherever you want
				 */
				void destroyAllAsteroids(void);

				/**
				 * @brief Check if asteroid is remain
				 * @see This method is called wherever you want
				 *
				 * @return bool     Return true if there is asteroid remains and false otherwise
				 */
				bool isAsteroids(void);

				/**
				 * @brief Return number of destroyed asteroids
				 * @see This method is called wherever you want
				 *
				 * @return int      return number of destroyed asteroids
				 */
				int getDestroyedAsteroids(void);

				/**
				 * @brief manage the minimum and the maximum time that an asteroid can spawn
				 * @see This methods is called wherever you want
				 *
				 * @param float		Minimum time of asteroids can spawn
				 * @param float		Maximum time of asteroids can spawn
				 */
				void manageAsteroidsSpawnTime(float minTime, float maxTime);

				/**
				 * @brief Update all informations of all asteroids
				 * @see This method is called in every update
				 *
				 * @return All asteroids in a vector
				 */
				std::vector<Asteroids>& getAllAsteroids(void);

				/**
				 * @brief Enable and disable the spawn of planets
				 * @see This methods is called wherever you want
				 *
				 * @param bool		true if an planet can spawn and false otherwise
				 */
				void togglePlanetsSpawn(bool canSpawn);

				/**
				 * @brief manage the minimum and the maximum time that an planets can spawn
				 * @see This methods is called wherever you want
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
				void addPlanet(std::string spriteSheet, int numberPlanet, sf::Vector2f position, float size, float speed, bool canMove);

				/**
				 * @brief Draw all asteroids of the game
				 * @see This methods is called from BackgroundGenerator
				 *
				 * @param spawnPosition	position of spawn of an enemy
				 */
				void setAsteroidsPositionSpawn(Asteroids::asteroidsSpawn spawnPosition);
			};
		};
	};
};