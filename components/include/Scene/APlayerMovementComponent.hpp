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
//  File:            APlayerMovementComponent.hpp                                     //
//  Description:     Represent the user input layer dedicated to player movement     //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include "Engine.hpp"

namespace rtype {
    namespace scene {

        /**
        ** @brief Represent the user input layer dedicated to player movement
        */
        class APlayerMovementComponent {
            public:
                /**
                 * @brief Process the user input to move the player
                 *
                 * @param engine GameEngine pointer that will be used to draw in the window
                 */
                virtual void processPlayerInput(rtype::engine::Engine *engine) = 0;
        };
    }
}