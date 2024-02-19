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
//  File:            GlobalUtils.hpp                                                 //
//  Description:     Bunch of datas used by differents components                    //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <SFML/Graphics.hpp>

namespace rtype {
	namespace globalutils {

        /**
         * @brief Represent differents locations for the origin of a component
         */
        enum OriginPos {
            MIDDLE  =   0,
            LEFT    =   1,
            RIGHT   =   2
        };

        static sf::Vector2f returnOriginPos(OriginPos originPos, sf::FloatRect bounds)
        {
            if (originPos == OriginPos::LEFT)
                return sf::Vector2f(0, 0);
            else if (originPos == OriginPos::MIDDLE)
                return sf::Vector2f(bounds.width / 2, bounds.height / 2);
            else
                return sf::Vector2f(bounds.width, 0);
        }
    };
};