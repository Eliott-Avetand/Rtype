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
//  File:            Breed.cpp                                                       //
//  Description:     Implement functions of the breed class                          //
//                                                                                   //
// ********************************************************************************* //
#include "Breed.hpp"
#include <memory>
#include <utility>
#include "Monster.hpp"
#include "RandomNumbers.hpp"

rtype::actor::Breed::Breed(int maxHealth, std::string name, int occurences, std::string sprite, int timeToSpawn, std::string patternName)
: _healthBar(std::make_pair(maxHealth, maxHealth)), _name(name), _occurences(occurences), _sprite(sprite), _timeToSpawn(timeToSpawn), _patternName(patternName)
{
    this->_spawnClock.restart();
}

std::unique_ptr<rtype::actor::Actor> rtype::actor::Breed::newMonster()
{
    //std::cout << _occurences << std::endl;
    if (_occurences <= 0)
        return nullptr;
    else {
        _occurences--;
        return std::make_unique<rtype::actor::Monster>(*this);
    }
}

int rtype::actor::Breed::getHealth()
{
    return _healthBar.first;
}

std::string rtype::actor::Breed::getName()
{
    return _name;
}

int rtype::actor::Breed::getOccurences()
{
    return _occurences;
}

std::string rtype::actor::Breed::getSpritePath(void)
{
    return this->_sprite;
}

int rtype::actor::Breed::getTimeToSpawn(void)
{
    return this->_timeToSpawn;
}

sf::Clock &rtype::actor::Breed::getClockSpawn(void)
{
    return this->_spawnClock;
}
std::string rtype::actor::Breed::getPatternName()
{
    return this->_patternName;
}
