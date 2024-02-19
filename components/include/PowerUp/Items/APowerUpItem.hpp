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
//  File:            APowerUpItem.hpp                                                //
//  Description:     Abstract for player power ups                                   //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <memory>

#include "Engine.hpp"
#include "StaticSprite.hpp"

namespace rtype {
    namespace powerup {
        class APowerUpItem {
            public:
                /**
                 * @brief Get the spawn chance of the item (static function)
                 *
                 * @return The percentage of spawn chance as a float
                 */
                static float getSpawnChance(void);

                /**
                 * @brief Return if power up has spawned in the level or not
                 *
                 * @return true
                 * @return false
                 */
                virtual bool hasSpawned(void) = 0;

                /**
                 * @brief Return if the power up is visible (e.g if it was picked up or not)
                 *
                 * @return true
                 * @return false
                 */
                virtual bool isVisible(void) = 0;

                /**
                 * @brief Pick up the power up (for Actors)
                 *
                 */
                virtual void pickUp(void) = 0;

                /**
                 * @brief Get the name of the power up
                 *
                 * @return The name of the power up as a std::string
                 */
                virtual std::string getPowerUpName(void) = 0;

                /**
                 * @brief Get the sprite object of the power up
                 *
                 * @return The sprite object as a StaticSprite
                 */
                virtual rtype::client::utilities::StaticSprite getSprite(void) = 0;

                /**
                 * @brief Update the power up
                 *
                 * @param engine GameEngine pointer that will be used to draw in the window
                 * @param deltaTime Represent the time elapsed between two frames. Used to cap the framerate of each animations
                */
                virtual void update(rtype::engine::Engine *engine) = 0;

                /**
                 * @brief Render the power up in the window
                 *
                 * @param engine GameEngine pointer that will be used to draw in the window
                */
                virtual void render(rtype::engine::Engine *engine) = 0;
        };
    };
};