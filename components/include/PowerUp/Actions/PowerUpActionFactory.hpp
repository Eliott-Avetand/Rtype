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
//  File:            PowerUpActionFactory.hpp                                        //
//  Description:     Factory that create PowerUp actions                             //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <memory>

#include "Errors.hpp"
#include "APowerUpAction.hpp"
#include "TripleShootAction.hpp"

namespace rtype {
    namespace powerup {
        class APowerUpAction;
    };

    namespace powerup {
        class PowerUpActionFactory {
            public:

                /**
                 * @brief Factory-like function. Map a PowerUp name to a PowerUp Action class
                 *
                 * @param powerUpName The name of a power up as std::string
                 * @return A PowerUp Action class as std::unique_ptr<rtype::powerup::APowerUpAction>
                 */
                static std::unique_ptr<rtype::powerup::APowerUpAction> getPowerUpAction(std::string powerUpName);
        };
    };
};