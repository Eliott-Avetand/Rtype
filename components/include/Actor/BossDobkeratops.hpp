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
//  File:            BossDobkeratops.hpp                                             //
//  Description:     First boss of the rtype                                         //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <vector>
#include "AnimatedSprite.hpp"
#include "MissileBoss.hpp"
#include "Actor.hpp"
#include "CustomSound.hpp"
#include "Player.hpp"

#define BOSS_OST "./client/assets/musics/levels_musics/boss.ogg"
#define BOSS_ROAR "./client/assets/musics/other_sounds/boss_roar.ogg"

#ifdef SERVER

class Room;

#endif // SERVER


namespace rtype {
    namespace actor {
        class DobkeratopsHeart {

            public:
                DobkeratopsHeart(sf::Vector2f position, sf::Vector2f react, sf::Vector2i animationBegin, sf::Vector2i animationEnd);

                enum state {
                    SPAWN,
                    STOPPED,
                    ALIVE,
                    DYING,
                    DEAD,
                    INVINCIBLE
                };
                std::unique_ptr<rtype::client::utilities::AnimatedSprite> _sprite;
                std::vector<std::unique_ptr<rtype::client::utilities::AnimatedSprite>> _spriteExplosion;
                rtype::engine::utilities::CustomSound _deathSound;
                std::vector<std::unique_ptr<rtype::actor::MissileBoss>> _allMissiles;
                sf::Vector2f _position;
                sf::Clock _shootClock;
                sf::Clock _dyingClock;
                sf::Clock _explosionClock;
                sf::Clock _missileClock;
                sf::Clock _animationClock;
                sf::Clock _invincibleClock;

                std::string _id;

                int _health;
                int _score;
                int _damage;
                state _state;

                int getDamage();
                sf::Vector2f getPosition();
                sf::Sprite &getSprite();
                state getState();

                void setPosition(sf::Vector2f);
                void setAlive();

                bool isDying();
                bool isDead();
                bool isPlayerCollision(std::unique_ptr<rtype::actor::Actor> &actor);

                #ifdef SERVER
                void updateMissiles(Room* room);
                void update(Room* room);
                #endif

                std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& getMissile();

                void takeDamage(rtype::engine::Engine *engine, bool isLastHeart, int damage);
                void updateMissiles(rtype::engine::Engine* engine);
                void updateInvincible();
                void update(rtype::engine::Engine *engine);
                void shoot();
                void die(rtype::engine::Engine *engine);
                void draw(rtype::engine::Engine* engine);
        };

        class BossDobkeratops {
            enum tailSprites {
                SHORT,
                MEDIUM,
                BIG,
                END
            };

            enum state {
                SPAWN,
                STOPPED,
                ALIVE,
                DYING,
                DEAD
            };

            private:
                std::unique_ptr<rtype::client::utilities::AnimatedSprite> _sprite;
                std::vector<std::unique_ptr<rtype::client::utilities::AnimatedSprite>> _spriteHeart;
                std::vector<std::unique_ptr<rtype::client::utilities::AnimatedSprite>> _spriteExplosion;
                std::vector<std::unique_ptr<rtype::client::utilities::AnimatedSprite>> _tail;
                std::map<rtype::actor::BossDobkeratops::tailSprites, std::vector<sf::Vector2f>> _balls;
                std::vector<std::unique_ptr<rtype::actor::MissileBoss>> _allMissiles;
                rtype::engine::utilities::CustomSound _soundRoar;
                std::vector<std::unique_ptr<DobkeratopsHeart>> _dobkeratopsHeart;
                int _damage;
                int _tailSpeed;
                int _tailCounter;
                int _score;
                state _state;
                std::vector<sf::Clock> _tailCreationTime;
                sf::Vector2f _position;
                sf::Clock _tailClock;
                sf::Clock _tailCreationClock;
                sf::Clock _moveClock;
                sf::Clock _shootClock;
                sf::Clock _missileClock;
                sf::Clock _dyingClock;
                sf::Clock _explosionClock;

            public:
                BossDobkeratops(rtype::engine::Engine *engine);

                int getDamage(void);

                bool isDead();
                bool isDying();
                int nbrHeartsAlive();
                bool isMissileCollision(rtype::actor::Missile *missile, rtype::engine::Engine *engine);
                bool isTailCollision(rtype::actor::Missile *missile);
                bool isPlayerCollision(std::unique_ptr<rtype::actor::Actor> &actor);
                bool isSpawned(void);

                std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& getMissile();
                std::vector<std::unique_ptr<rtype::client::utilities::AnimatedSprite>>& getTails();
                std::vector<std::unique_ptr<DobkeratopsHeart>> &getHearts();

                void setDamage(int damage);
                int getScore();
                void setPosition(sf::Vector2f position);
                sf::Vector2f getPosition(void);
                #ifdef SERVER
                bool isMissileCollision(rtype::actor::Missile* missile, Room* room, int playerID);
                void updateMissiles(Room* room);
                void update(Room *room);
                #endif

                void shoot(void);
                void spawn(rtype::engine::Engine *engine);
                void die(rtype::engine::Engine *engine);
                void updateMissiles(rtype::engine::Engine* engine);
                void update(rtype::engine::Engine* engine);
                void draw(rtype::engine::Engine* engine);
        };
    }
}