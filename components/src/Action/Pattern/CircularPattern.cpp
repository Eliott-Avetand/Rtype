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

#include "CircularPattern.hpp"
#include "Monster.hpp"

rtype::action::CircularPattern::CircularPattern()
{
    this->_spawnDelay = 0.5;
    this->_speed = 3.0;
    this->_spawnPoint = sf::Vector2f(2000, 500);
    this->_angle = 0;
    this->_radius = 300;
    this->_centerOrigins = sf::Vector2f(1920.0f, 400.0f);
}

void rtype::action::CircularPattern::perform(std::unique_ptr<rtype::actor::Actor>& actor)
{
    if (!actor)
        return;
    rtype::actor::Monster* monster = dynamic_cast<rtype::actor::Monster*>(actor.get());
    if (!monster)
        return;
    if (monster->getPosition().x < -1000)
        return;

    sf::Vector2f pos = monster->getPosition();

    pos.x = this->_centerOrigins.x + this->_radius * cos(this->_angle);
    pos.y = this->_centerOrigins.y + this->_radius * sin(this->_angle);

    this->_angle -= 0.01;
    monster->setAngle(this->_angle);

    this->_centerOrigins.x -= 0.5;
    monster->setCenterOrigins(this->_centerOrigins);

    #ifdef CLIENT
    if (monster->canShoot())
        monster->shoot();
    #endif

    monster->setPosition(pos);
}
