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
//  File:            GameDatas.cpp                                                   //
//  Description:     GameDatas Class Definition                                      //
//                                                                                   //
// ********************************************************************************* //
#include "GameDatas.hpp"

rtype::engine::GameDatas::GameDatas()
{
	this->_actualLevel = 1;
	this->_highScore = 0;
	this->_score = 0;
	this->_playerColor = sf::Color::White;
}

int rtype::engine::GameDatas::getScore(void)
{
	return this->_score;
}

void rtype::engine::GameDatas::setScore(int score)
{
	this->_score = score;
}

int rtype::engine::GameDatas::getHighScore(void)
{
	return this->_highScore;
}

void rtype::engine::GameDatas::setHighScore(int highScore)
{
	this->_highScore = highScore;
}

int rtype::engine::GameDatas::getActualLevel(void)
{
	return this->_actualLevel;
}

void rtype::engine::GameDatas::setActualLevel(int level)
{
	this->_actualLevel = level;
}

sf::Color rtype::engine::GameDatas::getPlayerColor(void)
{
	return this->_playerColor;
}

void rtype::engine::GameDatas::setPlayerColor(sf::Color color)
{
	this->_playerColor = color;
}
