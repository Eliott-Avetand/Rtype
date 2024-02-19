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

#include "RandomPattern.hpp"
#include "Monster.hpp"
#include "RandomNumbers.hpp"

rtype::action::RandomPattern::RandomPattern()
{
    this->_spawnDelay = 0.5;
    this->_speed = 3.0;

    float spawnY = rtype::client::utilities::RandomNumbers::randomFloat(200, 800);
    this->_spawnPoint = sf::Vector2f(2000, spawnY);

    float destinationY = rtype::client::utilities::RandomNumbers::randomFloat(-100, 1150);
    this->_destination = sf::Vector2f(-1000, destinationY);

    this->_hasChangedDirection = false;
}

float mod(float a, float b)
{
    return a - b * floor(a / b);
}

void rtype::action::RandomPattern::perform(std::unique_ptr<rtype::actor::Actor>& actor)
{
    if (!actor)
        return;
    rtype::actor::Monster *monster = dynamic_cast<rtype::actor::Monster *>(actor.get());
    if (!monster)
        return;
    if (monster->getPosition().x < -1000)
        return;

    sf::Vector2f pos = monster->getPosition();

    if (mod(pos.x, 100) < 2 && !this->_hasChangedDirection) {
        this->_destination = sf::Vector2f(-1000, rtype::client::utilities::RandomNumbers::randomFloat(-100, 1150));
        this->_hasChangedDirection = true;
    } else if (mod(pos.x, 100) >= 2) {
        this->_hasChangedDirection = false;
    }

    if (pos.x > this->_destination.x)
        pos.x -= this->_speed;
    if (pos.y > this->_destination.y)
        pos.y -= this->_speed;
    if (pos.x < this->_destination.x)
        pos.x += this->_speed;
    if (pos.y < this->_destination.y)
        pos.y += this->_speed;

    #ifdef CLIENT
    if (monster->canShoot())
        monster->shoot();
    #endif

    monster->setPosition(pos);
}
