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

#include "AnimatedSprite.hpp"

#include <iostream>

#define MISSILE_BOSS_TEXTURE "./client/assets/sprites/monsters/boss.png"

namespace rtype {
    namespace actor {
        class MissileBoss {
            public:
                MissileBoss(sf::Vector2f position = sf::Vector2f(0,0), int speed = 20, int damage = 1, std::string sprite = MISSILE_BOSS_TEXTURE    );

                void draw(rtype::engine::Engine* engine);
                void update(void);

                int getSpeed(void);
                int getDamage(void);
                
                std::string getId(void);
                void setId(std::string);

                sf::Vector2f getPosition(void);
                void setPosition(sf::Vector2f position);

                sf::FloatRect getBounds();
                sf::Sprite &getSprite(void);

                bool isDestroyed(void);
                void destroy(void);

            private:
                bool _isDestroy;

                std::string _id;

                int _speed;
                int _damage;

                std::pair<int, int> _position;
                sf::Clock _moveClock;

                sf::FloatRect _bounds;

                std::unique_ptr<rtype::client::utilities::AnimatedSprite> _sprite;
        };
    };
};