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
//  Description:     Asteroid Header										         //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "Engine.hpp"
#include "RandomNumbers.hpp"
#include "AnimatedSprite.hpp"
#include "Missile.hpp"

#define EXPLOSION_TEXTURE "./client/assets/sprites/planets/asteroids_explosion.png"

#define ASTEROID_TEXTURE "./client/assets/sprites/planets/asteroids_1.png"
#define ASTEROID_SOUND "./client/assets/musics/other_sounds/asteroidExplosionSound.ogg"

namespace rtype {
    namespace actor {
        class Asteroid {
            public:
                enum asteroidState {
                    ALIVE,
                    EXPLOSED,
                    DESTROYED,
                };

                Asteroid();
                Asteroid(sf::Vector2f positions, float size, float speed);

                void draw(rtype::engine::Engine *engine);
                void update(rtype::engine::Engine* engine);
                void destroy(void);
                rtype::actor::Asteroid::asteroidState getState(void);
                void setState(rtype::actor::Asteroid::asteroidState state);
                sf::Sprite& getSprite(void);
                sf::Vector2f getPositions(void);
                void setPosition(sf::Vector2f positions);
                bool isMissileCollision(rtype::actor::Missile *missile);
                sf::FloatRect getBounds(void);

                void setId(std::string id);
                std::string getId(void);

                float getSize(void);
                float getSpeed(void);
            private:
                sf::Vector2f _positions;
                float _size;
                float _speed;

                std::string _id;

                rtype::client::utilities::AnimatedSprite _sprite;
                rtype::client::utilities::AnimatedSprite _explosionSprite;
                sf::RectangleShape _collisionBounds;

                rtype::engine::utilities::CustomSound _sound;

                rtype::actor::Asteroid::asteroidState _state;

                sf::Clock _moveClock;

        };

    }
}
