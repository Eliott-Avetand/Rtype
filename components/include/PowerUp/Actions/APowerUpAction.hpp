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
//  File:            APowerUpAction.hpp                                              //
//  Description:     Abstract for PowerUp actions                                    //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include "Player.hpp"

namespace rtype {
    namespace actor {
        class Player;
    };

    namespace powerup {
        class APowerUpAction {
            public:
                /**
                 * @brief Get the name of the PowerUp Action object
                 *
                 * @return A name as a std::string
                 */
                virtual std::string getActionName(void) const = 0;

                /**
                 * @brief Represent the effect of the PowerUp, that will be executed every frame of the game
                 * 
                 * @param player A pointer to a Player class
                 */
                virtual void perform(rtype::actor::Player *player) = 0;
        };
    };
};