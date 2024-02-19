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
//  File:            Options.cpp                                                     //
//  Description:     Options class file                                              //
//                                                                                   //
// ********************************************************************************* //

#include <iostream>
#include <vector>
#include <string>

#include <SFML/Window.hpp>

#include "Options.hpp"

rtype::settings::Options::Options()
{
    // this->_width = 1920.f;
    // this->_height = 1080.f;
    this->_width = (float)sf::VideoMode::getFullscreenModes()[0].width;
    this->_height =  (float)sf::VideoMode::getFullscreenModes()[0].height;
    this->_fps = 60;
    this->_nb_players = 1;
    this->_title = "R-Type";
    this->_keybinds = {
        {rtype::settings::UP, sf::Keyboard::Up},
        {rtype::settings::DOWN, sf::Keyboard::Down},
        {rtype::settings::RIGHT, sf::Keyboard::Right},
        {rtype::settings::LEFT, sf::Keyboard::Left},
        {rtype::settings::SHOT, sf::Keyboard::Space}
    };
}

void rtype::settings::Options::setWindowSize(sf::Vector2f newSize)
{
    this->_width = newSize.x;
    this->_height = newSize.y;
}

float rtype::settings::Options::getWindowWidth()
{
    return this->_width;
}

float rtype::settings::Options::getWindowHeight()
{
    return this->_height;
}

size_t rtype::settings::Options::getFps()
{
    return _fps;
}

double rtype::settings::Options::getFramePerUpdate()
{
    return (1.0f / _fps) * 1000.0f;
}

char const *rtype::settings::Options::getTitle()
{
    return _title.c_str();
}

std::map<rtype::settings::Actions, sf::Keyboard::Key> rtype::settings::Options::getActions()
{
    return this->_keybinds;
}

void rtype::settings::Options::increasePlayers()
{
    _nb_players = _nb_players + 1 > 4 ? 1 : _nb_players + 1;
}

void rtype::settings::Options::decreasePlayers()
{
    _nb_players = _nb_players - 1 < 1 ? 4 : _nb_players - 1;
}

int rtype::settings::Options::getNbPlayers()
{
    return _nb_players;
}