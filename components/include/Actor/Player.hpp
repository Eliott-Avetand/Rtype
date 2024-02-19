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
//  File:            Player.hpp                                                      //
//  Description:     Player class                                                    //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <iostream>
#include <memory>

#include "Actor.hpp"
#include "AnimatedSprite.hpp"
#include "Asteroid.hpp"
#include "CustomText.hpp"

#include "APowerUpAction.hpp"
#include "PowerUpActionFactory.hpp"

#define PLAYER_SPRITE_TOP "./client/assets/sprites/players/spaceship_top.png"
#define PLAYER_SPRITE_BOTTOM "./client/assets/sprites/players/spaceship_bottom.png"

#define PLAYER_DAMAGE_SOUND "./client/assets/musics/other_sounds/player_damage.ogg"

#define PLAYER_SHOOT_SOUND_1 "./client/assets/musics/other_sounds/player_shoot_1.wav"

namespace rtype {
    namespace powerup {
        class APowerUpAction;
    };

    namespace actor {
        class Player : public rtype::actor::Actor {
            public:
                enum playerState {
                    NORMAL,
                    INVINCIBLE,
                };

                Player(std::string name, int id);
                ~Player();

                int getID(void);
                std::string getName(void);

                float getReloadedPercentage(void);

                int getHealth(void);
                void setHealth(int healthNumber);
                void setDamage(int damage);
                int getDamage(void);
                void takeDamage(int damage);

                sf::Sprite &getSprite(void);

                sf::Vector2f getPosition(void);
                void setPosition(sf::Vector2f position);

                int getScore(void);
                void addScore(int scoreAdded);
                void subScore(int scoreSubstracted);
                void setScore(int score);

                void setPlayerName(std::string name);

                void update(rtype::engine::Engine* engine);
                void updateMissiles(rtype::engine::Engine* engine);
                void draw(rtype::engine::Engine* engine);

                MoveDirection getDirection(void);
                void setDirection(MoveDirection direction);

                void moveUp();
                void moveDown();
                void moveLeft();
                void moveRight();
                void shoot();

                rtype::actor::Player::playerState getStatus(void);

                bool isMissileCollision(rtype::actor::Missile *missile);
                bool isActorCollision(rtype::actor::Actor* actor);
                bool isAsteroidCollision(rtype::actor::Asteroid* asteroid);
                bool isPowerUpCollision(std::unique_ptr<rtype::powerup::APowerUpItem> &powerup);

                std::vector<std::unique_ptr<rtype::actor::Missile>>& getMissiles(void);
                rtype::actor::Actor::actorState getState(void);

                std::string getEntityID(void);
                void setEntityID(std::string ID);

                void setPlayerColor(sf::Color color);

                void setPowerUpAction(std::string powerUpName);
                void drawUsername(rtype::engine::Engine* engine);

            private:
                int _id;
                std::string _name;

                rtype::actor::Player::playerState _playerState;

                sf::Clock _standClock;
                sf::Clock _reloadClock;
                sf::Clock _shootWaitClock;

                sf::Clock _invincibleClock;
                sf::Clock _invincibleAnimationClock;

                std::pair<int, int> _healthBar;
                std::pair<int, int> _attackBar;

                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _usernameText;

                std::unique_ptr<rtype::client::utilities::AnimatedSprite> _bottomSprite;

                rtype::engine::utilities::CustomSound _shootSound;
                rtype::engine::utilities::CustomSound _damageSound;

                std::unique_ptr<rtype::powerup::APowerUpAction> _powerup;

                void updatePlayerState(void);
        };
    }
}