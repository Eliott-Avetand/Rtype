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
//  File:            Time.cpp                                                        //
//  Description:     Time class file                                                 //
//                                                                                   //
// ********************************************************************************* //
#include "Time.hpp"
#include <SFML/System.hpp>

rtype::engine::Time::Time(double time = 0.0f)
{
    _time = time;
    _lastTime = 0.0f;
    _deltaTime = 0.0f;
}

rtype::engine::Time::operator float() const
{
    return _time;
}

double rtype::engine::Time::getSeconds() const
{
    return _time;
}

double rtype::engine::Time::getMilliseconds() const
{
    return _time * 1000;
}