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
//  File:            TripleShoot.hpp                                                 //
//  Description:     Triple shoot power up header                                    //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include "APowerUpItem.hpp"
#include "Missile.hpp"
#include "StaticSprite.hpp"

#include <SFML/Graphics.hpp>

#define TRIPLESHOOT_ITEM "./client/assets/sprites/powerup/TripleShootItem.png"

namespace rtype {
    namespace powerup {
        class TripleShootItem : public rtype::powerup::APowerUpItem {
            private:
                rtype::client::utilities::StaticSprite _sprite;

                const std::string _name;
                const float _speed;
                bool _hasSpawned;
                bool _isVisible;

                sf::Clock _rotationClock;

            public:
                /**
                 * @brief Represent the item of TripleShoot power up, that will be picked up by players
                 * 
                 */
                TripleShootItem(sf::Vector2f pos);

                bool hasSpawned(void);
                static float getSpawnChance(void);

                bool isVisible(void);
                void pickUp(void);

                std::string getPowerUpName(void);

                rtype::client::utilities::StaticSprite getSprite(void);

                void update(rtype::engine::Engine *engine);
                void render(rtype::engine::Engine *engine);
        };
    };
};