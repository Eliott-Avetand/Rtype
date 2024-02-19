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

#include "ZigZagPattern.hpp"
#include "Monster.hpp"

rtype::action::ZigZagPattern::ZigZagPattern()
{
    this->_spawnDelay = 0.7;
    this->_speed = 5.0;
    this->_spawnPoint = sf::Vector2f(2000, 500);
    this->_xDestination = 1000;
    this->_dirUp = true;
}

void rtype::action::ZigZagPattern::perform(std::unique_ptr<rtype::actor::Actor>& actor)
{
    if (!actor)
        return;
    rtype::actor::Monster *monster = dynamic_cast<rtype::actor::Monster *>(actor.get());
    if (!monster)
        return;
    if (monster->getPosition().x < -1000)
        return;

    sf::Vector2f pos = monster->getPosition();

    if (pos.x > this->_xDestination) {
        pos.x -= this->_speed;
    } else if (pos.x <= this->_xDestination) {
        if (this->_dirUp == true) {
            if (pos.y > 200) {
                pos.y -= this->_speed;
            } else {
                this->_dirUp = false;
            }     
        } else {
            if (pos.y < 800) {
                pos.y += this->_speed;
            } else {
                this->_dirUp = true;
            }
        }
    }

    #ifdef CLIENT
    if (monster->canShoot())
        monster->shoot();
    #endif

    monster->setPosition(pos);
}