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

#include "QuickLeftPattern.hpp"
#include "Monster.hpp"
#include "RandomNumbers.hpp"

rtype::action::QuickLeftPattern::QuickLeftPattern()
{
    this->_spawnDelay = 0.5;
    this->_speed = 1.0;

    float spawnX = rtype::client::utilities::RandomNumbers::randomFloat(1200, 1900);
    float spawnYTop = -100;
    float spawnYBottom = 1100;
    float randomTopOrBottom = rtype::client::utilities::RandomNumbers::randomFloat(0, 1);

    if (randomTopOrBottom < 0.5) {
        this->_spawnPoint = sf::Vector2f(spawnX, spawnYTop);
        this->_fromTop = true;
    } else {
        this->_spawnPoint = sf::Vector2f(spawnX, spawnYBottom);
        this->_fromTop = false;
    }

    if (this->_fromTop)
        this->_yDestination = rtype::client::utilities::RandomNumbers::randomFloat(0, 500);
    else
        this->_yDestination = rtype::client::utilities::RandomNumbers::randomFloat(500, 1000);
}

void rtype::action::QuickLeftPattern::perform(std::unique_ptr<rtype::actor::Actor>& actor)
{
    if (!actor)
        return;
    rtype::actor::Monster *monster = dynamic_cast<rtype::actor::Monster *>(actor.get());
    if (!monster)
        return;
    if (monster->getPosition().x < -1000)
        return;

    sf::Vector2f pos = monster->getPosition();

    if (pos.y <= this->_yDestination && this->_fromTop) {
        pos.y += this->_speed;
    } else if (pos.y >= this->_yDestination && !this->_fromTop) {
        pos.y -= this->_speed;
    } else {
        pos.x -= this->_speed * 20;
    }

    monster->setPosition(pos);
}
