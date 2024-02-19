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
//  File:            PowerUpAction.cpp                                               //
//  Description:     Map power up item with an action (function)                     //
//                                                                                   //
// ********************************************************************************* //

#include "TripleShootAction.hpp"

rtype::powerup::TripleShootAction::TripleShootAction() :
_name("TripleShoot")
{
    // Void
}

void rtype::powerup::TripleShootAction::perform(rtype::actor::Player *player)
{
    const short angles[3] = {-TRIPLESHOOT_ANGLE, 0, TRIPLESHOOT_ANGLE};
    for (uint8_t i = 0; i < 3; i++)
        player->getMissiles().push_back(std::make_unique<rtype::actor::Missile>(sf::Vector2f(player->getPosition().x + 10, player->getPosition().y), 40, 1, rtype::actor::Missile::missileDirection::RIGHT, angles[i]));
}

std::string rtype::powerup::TripleShootAction::getActionName(void) const
{
    return this->_name;
}