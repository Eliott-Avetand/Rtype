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
//  File:            Monster.hpp                                                     //
//  Description:     Monster class from which every enemies inherit                  //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <vector>
#include <memory>

#include "Breed.hpp"
#include "Actor.hpp"
#include "Missile.hpp"
#include "RandomNumbers.hpp"
#include "ActorPattern.hpp"

namespace rtype {
    namespace actor {
        class Monster : public rtype::actor::Actor {
            public:
                Monster(rtype::actor::Breed &breed);

                std::string getName();
                int getHealth(void);
                void setHealth(int healthNumber);
                void setDamage(int damage);
                int getDamage(void);
                void takeDamage(int damage);
                float getReloadedPercentage(void);

                std::vector<std::unique_ptr<rtype::actor::Missile>>& getMissiles(void);
                void setPosition(sf::Vector2f position);
                sf::Vector2f getPosition(void);

                int getScore(void);
                void setScore(int score);
                void addScore(int scoreAdded);
                void subScore(int scoreSubstracted);

                MoveDirection getDirection(void);
                void setDirection(MoveDirection direction);

                float getSpeed(void);
                sf::Clock& getClock(void);
                float getAngleSpeed(void);
                float getRadius(void);
                float getAngle(void);
                sf::Vector2f getCenterOrigins(void);

                void setAngleSpeed(float angleSpeed);
                void setRadius(float radius);
                void setAngle(float angle);
                void setSpeed(float speed);
                void setClock(sf::Clock clock);
                void setCenterOrigins(sf::Vector2f centerOrigins);
                sf::Sprite& getSprite(void);

                void update(rtype::engine::Engine* engine);
                void draw(rtype::engine::Engine* engine);

                void moveUp();
                void moveDown();
                void moveLeft();
                void moveRight();

                void moveUp(float speed);
                void moveDown(float speed);
                void moveLeft(float speed);
                void moveRight(float speed);

                void shoot();

                bool canShoot();

                bool isMissileCollision(rtype::actor::Missile *missile);
                bool isActorCollision(rtype::actor::Actor* actor);
                bool isAsteroidCollision(rtype::actor::Asteroid* asteroid);
                bool isPowerUpCollision(std::unique_ptr<rtype::powerup::APowerUpItem> &powerup);

                rtype::actor::Actor::actorState getState(void);

                void setEntityID(std::string ID);
                std::string getEntityID(void);

                void setBreedIndex(int index);
                int getBreedIndex(void);

                bool getDirUp(void);
                void setDirUp(bool dirUp);
                std::unique_ptr<rtype::action::ActorPattern> &getPattern(void);

            private:
                rtype::actor::Breed &_breed;
                std::unique_ptr<rtype::action::ActorPattern> _pattern;

                sf::Clock _clock;

                sf::Clock _reloadClock;
                float _reloadTime;

                float _speed;

                float _angleSpeed;
                float _radius;
                float _angle;
                bool _dirUp;

                int _breedIndex;
                sf::Vector2f _centerOrigins;
        };
    }
}