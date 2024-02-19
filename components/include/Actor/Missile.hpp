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
//  File:            Missile.hpp                                                     //
//  Description:     Missile class header                                            //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "StaticSprite.hpp"

#include <iostream>

#define MISSILE_BASIC_TEXTURE "./client/assets/sprites/players/missile_1.png"

#define PI 3.14159265

namespace rtype {
    namespace actor {
        class Missile {
            public:
                enum missileDirection {
                    RIGHT,
                    LEFT
                };

                Missile(sf::Vector2f position = sf::Vector2f(0,0), int speed = 40, int damage = 1, rtype::actor::Missile::missileDirection direction = rtype::actor::Missile::missileDirection::RIGHT, float _angle = 0.f);

                void draw(rtype::engine::Engine* engine);
                void update();

                int getSpeed(void);
                int getDamage(void);

                sf::FloatRect getBounds();
                sf::Sprite &getSprite(void);
                std::pair<int, int> getPosition(void);

                bool isDestroyed(void);

            private:
                bool _isDestroy;

                rtype::actor::Missile::missileDirection _direction;

                int _speed;
                int _damage;
                float _angle;

                std::pair<int, int> _position;
                sf::Clock _moveClock;

                sf::FloatRect _bounds;

                std::unique_ptr<rtype::client::utilities::StaticSprite> _sprite;
        };
    };
};