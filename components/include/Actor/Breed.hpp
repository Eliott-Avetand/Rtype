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
//  File:            Breed.hpp                                                       //
//  Description:     The breed class to make the Type Object pattern                 //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "AnimatedSprite.hpp"
#include "Actor.hpp"
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace rtype {
    namespace actor {
        class Monster;

        class Breed {
            friend class rtype::actor::Monster;

            private:
                std::pair<int, int> _healthBar;
                sf::Sound _shotSound;
                std::string _sprite;
                std::string _name;
                int _occurences;
                int _timeToSpawn;
                std::string _patternName;

                sf::Clock _spawnClock;

                std::vector<std::unique_ptr<rtype::actor::Actor>> _monster;

            public:
                Breed(int maxHealth, std::string name, int occurences, std::string sprite, int timeToSpawn, std::string patternName = "circular");
                std::unique_ptr<rtype::actor::Actor> newMonster();

                std::string getSpritePath(void);
                int getHealth();
                std::string getName();
                int getOccurences();
                int getTimeToSpawn(void);
                sf::Clock &getClockSpawn(void);
                std::string getPatternName();
        };
    }
}
