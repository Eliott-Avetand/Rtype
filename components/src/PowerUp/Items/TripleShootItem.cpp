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
//  File:            TripleShootItem.cpp                                             //
//  Description:     Triple shoot power up implementation                            //
//                                                                                   //
// ********************************************************************************* //

#include "TripleShootItem.hpp"

rtype::powerup::TripleShootItem::TripleShootItem(sf::Vector2f pos) :
_sprite(TRIPLESHOOT_ITEM, pos),
_speed(2.f),
_name("TripleShoot")
{
    this->_hasSpawned = true;
    this->_isVisible = true;
    this->_sprite.setScale(3.f, 3.f);
    this->_rotationClock.restart();
}

float rtype::powerup::TripleShootItem::getSpawnChance(void)
{
    return 15.f;
}

bool rtype::powerup::TripleShootItem::hasSpawned(void)
{
    return this->_hasSpawned;
}

bool rtype::powerup::TripleShootItem::isVisible(void)
{
    return this->_isVisible;
}

void rtype::powerup::TripleShootItem::pickUp(void)
{
    this->_isVisible = false;
}

std::string rtype::powerup::TripleShootItem::getPowerUpName(void)
{
    return this->_name;
}

rtype::client::utilities::StaticSprite rtype::powerup::TripleShootItem::getSprite(void)
{
    return this->_sprite;
}

void rtype::powerup::TripleShootItem::update(rtype::engine::Engine *engine)
{
    if (!this->_hasSpawned || !this->_isVisible)
        return;

    if (this->_sprite.getPosition().y - (this->_sprite.getHeight() / 2) > engine->options.getWindowHeight()) {
        this->_isVisible = false;
        return;
    }

    this->_sprite.getSprite().move(0, this->_speed);
    if (this->_rotationClock.getElapsedTime().asMilliseconds() > (1/60)) {
        this->_sprite.getSprite().rotate(360/60);
        this->_rotationClock.restart();
    }
}

void rtype::powerup::TripleShootItem::render(rtype::engine::Engine *engine)
{
    if (!this->_hasSpawned || !this->_isVisible)
        return;
    this->_sprite.draw(engine);
}
