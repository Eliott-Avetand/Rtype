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
//  File:            Actor.hpp                                                       //
//  Description:     The Actor class, parent of each entities                        //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Missile.hpp"
#include "AnimatedSprite.hpp"
#include "CustomSound.hpp"
#include "Asteroid.hpp"
#include "APowerUpItem.hpp"

#define EXPLOSION_SPRITE "./client/assets/sprites/monsters/simple_explosion.png"
#define EXPLOSION_SOUND "./client/assets/musics/other_sounds/asteroidExplosionSound.ogg"

namespace rtype {
    namespace actor {
        class Actor {

            public:
                enum actorState {
                    ALIVE,
                    EXPLOSION,
                    DEAD
                };

                /**
                 * @brief get the actor's name
                 * @see This methods is called wherever you want
                 * @param sf::string	Name of the actor
                 */
                virtual std::string getName() = 0;

                /**
                 * @brief get the actor's health
                 * @see This methods is called wherever you want
                 * @param int	Health of the actor
                 */
                virtual int getHealth(void) = 0;

                /**
                  * @brief get the actor's reloaded percentage
                  * @see This methods is called wherever you want
                  * @return float	percentage reloaded of the actor
                  */
                virtual float getReloadedPercentage(void) = 0;

                /**
                 * @brief get the actor's score
                 * @see This methods is called wherever you want
                 * @return int	    score of the actor
                 */
                virtual int getScore(void) = 0;

                /**
                 * @brief add actor score
                 * @see This methods is called wherever you want
                 * @param int	    score added to the actor
                 */
                virtual void addScore(int scoreAdded) = 0;

                /**
                 * @brief add actor score
                 * @see This methods is called wherever you want
                 * @param int	    score added to the actor
                 */
                virtual void setScore(int score) = 0;

                /**
                 * @brief remove actor score
                 * @see This methods is called wherever you want
                 * @param int	    score substracted to the actor
                 */
                virtual void subScore(int scoreSubstracted) = 0;

                /**
                 * @brief get the actor's sprite
                 * @see This methods is called wherever you want
                 * @return sf::Sprite	    sprite of the actor
                 */
                virtual sf::Sprite &getSprite(void) = 0;

                /**
                 * @brief get the actor's missiles
                 * @see This methods is called wherever you want
                 * @param std::vector<std::unique_ptr<rtype::actor::Missile>>	Missiles of the actor
                 */
                virtual std::vector<std::unique_ptr<rtype::actor::Missile>> &getMissiles(void) = 0;

                /**
                 * @brief set a number of damge to the actor
                 * @see This methods is called wherever you want
                 * @param int	Number of damage give to the actor
                 */
                virtual void setDamage(int damage) = 0;

                /**
                 * @brief get a number of damge to the actor
                 * @see This methods is called wherever you want
                 * @result int	Number of damage give to the actor
                 */
                virtual int getDamage(void) = 0;

                /**
                 * @brief get a number of damge to the actor
                 * @see This methods is called wherever you want
                 * @param int	Number of damage give to the actor
                 */
                virtual void takeDamage(int damage) = 0;

                /**
                 * @brief set a number of health to the actor
                 * @see This methods is called wherever you want
                 * @param int	Number of health give to the actor
                 */
                virtual void setHealth(int healthNumber) = 0;

                /**
                 * @brief set the actor's position
                 * @see This methods is called wherever you want
                 * @param sf::Vector2f	Position of the actor
                 */
                virtual void setPosition(sf::Vector2f position) = 0;

                /**
                 * @brief get the actor's position
                 * @see This methods is called wherever you want
                 * @return sf::Vector2f	Position of the actor
                 */
                virtual sf::Vector2f getPosition(void) = 0;

                /**
                 * @brief get the actor's direction
                 * @see This methods is called wherever you want
                 * @return MoveDiection	Direction of the actor
                 */
                virtual MoveDirection getDirection(void) = 0;

                /**
                 * @brief get the actor's position
                 * @see This methods is called wherever you want
                 * @param MoveDiection	Direction of the actor
                 */
                virtual void setDirection(MoveDirection direction) = 0;

                /**
                 * @brief Draw actor in the scene
                 * @see This methods is called in the render method in the scene
                 * * @param engine		A reference of the engine given by the engine himself
                 */
                virtual void draw(rtype::engine::Engine* engine) = 0;

                /**
                 * @brief Update actor in the scene
                 * @see This methods is called in the update method in the scene
                 * @param engine		A reference of the engine given by the engine himself
                 */
                virtual void update(rtype::engine::Engine* engine) = 0;

                /**
                 * @brief Method which move up the actor
                 * @see This methods is called in the processInput method in the scene
                 */
                virtual void moveUp() = 0;

                /**
                 * @brief Method which move down the actor
                 * @see This methods is called in the processInput method in the scene
                 */
                virtual void moveDown() = 0;

                /**
                 * @brief Method which move left the actor
                 * @see This methods is called in the processInput method in the scene
                 */
                virtual void moveLeft() = 0;

                /**
                 * @brief Method which move right the actor
                 * @see This methods is called in the processInput method in the scene
                 */
                virtual void moveRight() = 0;

                /**
                 * @brief Method which allow the actor to shoot
                 * @see This methods is called in the processInput method in the scene
                 */
                virtual void shoot() = 0;

                /**
                 * @brief Check if actor is collided by a missile
                 * @see This methods is called wherever you want
                 * 
                 * @return bool     True is missile is collied, false otherwise
                 */
                virtual bool isMissileCollision(rtype::actor::Missile *missile) = 0;

                /**
                 * @brief Check if actor is collided by a missile
                 * @see This methods is called in the processInput method in the scene
                 *
                 * @return bool     True is actor is collied, false otherwise
                 */
                virtual bool isActorCollision(rtype::actor::Actor* actor) = 0;

                virtual bool isAsteroidCollision(rtype::actor::Asteroid* asteroid) = 0;

                virtual bool isPowerUpCollision(std::unique_ptr<rtype::powerup::APowerUpItem> &powerup) = 0;

                virtual void setEntityID(std::string ID) = 0;

                virtual std::string getEntityID(void) = 0;

                virtual rtype::actor::Actor::actorState getState(void) = 0;
            protected:
                std::string _name;
                std::string _entityID;

                std::pair<float, float> _position;
                MoveDirection _direction;

                std::vector<std::unique_ptr<rtype::actor::Missile>> _allMissiles;
                std::unique_ptr<rtype::client::utilities::AnimatedSprite> _sprite;
                std::unique_ptr<rtype::client::utilities::AnimatedSprite> _deathSprite;
                std ::unique_ptr<rtype::engine::utilities::CustomSound> _deathSound;
                rtype::actor::Actor::actorState _state;

                bool _canShoot;

                int _health;
                float _shootReloaded;
                int _strength;
                int _score;
        };
    }
}