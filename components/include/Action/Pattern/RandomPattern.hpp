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
//  File:            AIBasicCommand.hpp                                              //
//  Description:     AIBasicCommand header                                           //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "ActorPattern.hpp"
#include <iostream>
#include <memory>

namespace rtype {
    namespace action {
        
        class RandomPattern : public ActorPattern {
            public:
                RandomPattern();
                
                /**
                 * @brief Performs the action of the command. It will move all enemies with a specific pattern
                 * 
                 * @param actor The actor to perform the action on
                 */
                void perform(std::unique_ptr<rtype::actor::Actor>& actor);

            private:
                sf::Vector2f _destination;
                bool _hasChangedDirection;
        };
    };
};
