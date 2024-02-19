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
//  File:            Options.hpp                                                     //
//  Description:     Options class' header                                           //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <cstddef>
#include <string>
#include <map>
#include <vector>
#include "Music.hpp"
#include "SFML/Window.hpp"

namespace rtype {
    namespace settings {
        enum Actions {
            UP,
            DOWN,
            RIGHT,
            LEFT,
            SHOT
        };

        class Options {
            private:
                int _nb_players;
                float _width;
                float _height;
                size_t _fps;
                std::string _title;
                std::map <rtype::settings::Actions, sf::Keyboard::Key> _keybinds;

            public:
                Options();

                rtype::settings::Music music;

                void setWindowSize(sf::Vector2f newSize);
                float getWindowWidth();
                float getWindowHeight();

                size_t getFps();
                double getFramePerUpdate();

                char const *getTitle();

                int getNbPlayers();
                void increasePlayers();
                void decreasePlayers();

                std::map<rtype::settings::Actions, sf::Keyboard::Key> getActions();

        };
    }
}