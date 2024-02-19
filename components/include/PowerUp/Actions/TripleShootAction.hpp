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
//  File:            TripleShootAction.hpp                                           //
//  Description:     Action for PowerUp 'TripleShoot'                                //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include "APowerUpAction.hpp"

#define TRIPLESHOOT_ANGLE 25

namespace rtype {
    namespace powerup {
        class TripleShootAction : public rtype::powerup::APowerUpAction {
            private:
                const std::string _name;

            public:
                /**
                 * @brief Contain the name and the effect of the TripleShoot power up
                 * 
                 */
                TripleShootAction();

                std::string getActionName(void) const;
                void perform(rtype::actor::Player *player);
        };
    };
};