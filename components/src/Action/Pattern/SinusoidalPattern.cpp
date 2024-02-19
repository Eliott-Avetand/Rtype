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
//  File:            MoveDownCommand.cpp                                             //
//  Description:     MoveDownCommand class                                           //
//                                                                                   //
// ********************************************************************************* //

#include "SinusoidalPattern.hpp"
#include "Monster.hpp"

rtype::action::SinusoidalPattern::SinusoidalPattern()
{
    this->_spawnDelay = 0.7;
    this->_speed = 2.0;
    this->_spawnPoint = sf::Vector2f(2000, 500);
    this->amplitude = 400;
}

void rtype::action::SinusoidalPattern::perform(std::unique_ptr<rtype::actor::Actor>& actor)
{
    if (!actor)
        return;
    rtype::actor::Monster *monster = dynamic_cast<rtype::actor::Monster *>(actor.get());
    if (!monster)
        return;
    if (monster->getPosition().x < -1000)
        return;

    sf::Vector2f pos = monster->getPosition();

    float angle = (1920 - pos.x) / (this->amplitude / 3);

    pos.x -= this->_speed;
    pos.y = this->amplitude * sin(angle) + this->_spawnPoint.y;

    #ifdef CLIENT
    if (monster->canShoot())
        monster->shoot();
    #endif

    monster->setPosition(pos);
}