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
//  File:            AsteroidsGenerator.hpp                                          //
//  Description:     File for the AsteroidsGenerator Header                          //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "RandomNumbers.hpp"
#include <memory>
#include "AnimatedSprite.hpp"

#define EXPLOSION_TEXTURE "./client/assets/sprites/planets/asteroids_explosion.png"

#define ASTEROID_TEXTURE "./client/assets/sprites/planets/asteroids_1.png"
#define ASTEROID_SOUND "./client/assets/musics/other_sounds/asteroidExplosionSound.ogg"

class Asteroids {
	public:
		enum asteroidState {
			ALIVE,
			EXPLOSED,
			DESTROYED,
		};

		enum asteroidsSpawn {
			UP,
			RIGHT,
			LEFT,
			DOWN,
		};

		/**
		 * @brief Create component which have informations of asteroids
		 * @see Constructor is called when this class is create
		 * 
		 * @param sf::Vector2f	position of the spawn
		 * @param moveSpeed		speed of the asteroid's movement
		 */
		Asteroids(sf::Vector2f position = sf::Vector2f(0, 0), float moveSpeed = 100, Asteroids::asteroidsSpawn asteroidSpawn = Asteroids::UP);


		/**
		 * @brief Display asteroid on the screen
		 * @see This method is called when AsteroidGenerator draw method is called
		 *
		 * @param engine    A reference of the engine given by the engine himself
		 */
		void draw(rtype::engine::Engine* engine);

		/**
		 * @brief Update asteroid on the screen
		 * @see This method is called when AsteroidGenerator update method is called
		 *
		 * @param engine    A reference of the engine given by the engine himself
		 */
		void destroy(void);

		/**
		 * @brief Change asteroids state to explosed and restard clock
		 * @see This method is called when an asteroid is destroyed
		 */
		void update(rtype::engine::Engine* engine);
		
		/**
		 * @brief Change asteroids state to explosed and restard clock
		 * @see This method is called when an asteroid is destroyed
		 *
		 * @return Asteroids::asteroidState		state of the asteroid
		 */
		bool isOutside(size_t windowWidth, size_t windowHeight);
		
		/**
		 * @brief Check if an asteroid is outside of the screen
		 * @see This method is called in update
		 *
		 * @param windowWidth	Width of the window
		 * @param windowHeight	Height of the window
		 *
		 * @return bool			true if the asteroid is outside of the screen and false otherwise
		 */
		bool isHover(sf::Vector2f mousePosition);

		/**
		 * @brief Check if mouse is hover the asteroid's sprite
		 * @see This method is called in every events
		 *
		 * @param mousePosition	2D vector which contains mouse position
		 *
		 * @return bool			true if mouse is on the sprite and false otherwise
		 */
		Asteroids::asteroidState getState(void);
	private:
		rtype::client::utilities::AnimatedSprite _sprite;
		rtype::client::utilities::AnimatedSprite _explosionSprite;

		Asteroids::asteroidState _state;
		Asteroids::asteroidsSpawn _spawnPosition;

		std::map<Asteroids::asteroidsSpawn, std::pair<int, int>> _moveMap;

		sf::Clock _moveClock;
		bool _isOutside;

		float _moveSpeed;
};

namespace rtype {
	namespace client {
		namespace utilities {
			class AsteroidsGenerator {
				private:
					std::vector<Asteroids> _allAsteroids;
					rtype::engine::utilities::CustomSound _sound;

					int _destroyedAsteroids;
					bool _destroyAll;

					float _minTimeSpawn;
					float _maxTimeSpawn;

					bool _canSpawn;

					sf::Clock _spawnClock;
					float _randomTimeSpawn;

					Asteroids::asteroidsSpawn _spawnPosition;
				public:
					/**
					 * @brief Create component which can handler all informations of asteroids
					 * @see Constructor is called when this class is create
					 */
					AsteroidsGenerator(void);

					/**
					 * @brief Update all informations of all asteroids
					 * @see This method is called in every update
					 *
					 * @return All asteroids in a vector
					 */
					std::vector<Asteroids>& getAllAsteroids(void);

					/**
					 * @brief Update all informations of all asteroids
					 * @see This method is called in every update
					 *
					 * @param deltaTime		delta Time
					 * @param engine		A reference of the engine given by the engine himself
					 */
					void update(rtype::engine::Engine* engine);
					
					/**
					 * @brief Update and handle all events of the screen
					 * @see This method is called in every events
					 *
					 * @param event			All events of the loop
					 * @param engine		A reference of the engine given by the engine himself
					 */
					void eventUpdate(sf::Event event, rtype::engine::Engine* engine);
					
					/**
					 * @brief Draw all asteroids of the game
					 * @see This methods is called in draw of BackgroundGenerator
					 *
					 * @param engine		A reference of the engine given by the engine himself
					 */
					void drawAsteroids(rtype::engine::Engine* engine);

					/**
					 * @brief Draw all asteroids of the game
					 * @see This methods is called in draw of BackgroundGenerator
					 *
					 * @param bool		true if asteroids can spawn and false otherwise
					 */
					void toggleAsteroidsSpawn(bool canSpawn);
					
					/**
					 * @brief Check if asteroids remains or not
					 * @see This methods is called in update of BackgroundGenerator
					 *
					 * @return bool			true if there is asteroids and false otherwise
					 */
					bool isAsteroids(void);

					/**
					 * @brief Destroy all asteroids remaining
					 * @see This methods is called in BackgroundGenerator
					 */
					void destroyAllAsteroids(void);

					/**
					 * @brief Get numbers of all asteroids of the game
					 * @see This methods is called wherever you want
					 *
					 * @return int		Number of destroyed asteroids
					 */
					int getDestroyedAsteroids(void);

					/**
					 * @brief Draw all asteroids of the game
					 * @see This methods is called from BackgroundGenerator
					 *
					 * @param float		Minimum time of asteroids can spawn
					 * @param float		Maximum time of asteroids can spawn
					 */
					void manageAsteroidsSpawnTime(float minTime, float maxTime);

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