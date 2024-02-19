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
//  File:            MoveRightCommand.hpp                                            //
//  Description:     MoveRightCommand header                                         //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "ACommand.hpp"
#include <iostream>
#include <memory>

namespace rtype {
    namespace action {
        class MoveRightCommand : public rtype::action::ACommand {
            public:
                void perform(std::unique_ptr<rtype::actor::Actor>& actor);
        };
    };
};