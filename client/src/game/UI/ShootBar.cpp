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
//  File:            ShootBar.cpp                                                    //
//  Description:     ShootBar Class definition                                       //
//                                                                                   //
// ********************************************************************************* //
#include "ShootBar.hpp"

rtype::client::ShootBar::ShootBar(sf::Vector2f position) :
	_shootBarSprite(SHOOT_BAR_TEXTURE, position)
{
	this->_position = position;

	this->_shootBarSprite.setScale(4, 3);

	this->_loadingShootRect.setFillColor(sf::Color::Blue);
	this->_loadingShootRect.setSize(sf::Vector2f(this->_shootBarSprite.getWidth() - this->_shootBarSprite.getWidth(4.5), this->_shootBarSprite.getHeight() - this->_shootBarSprite.getHeight(29)));
	this->_loadingShootRect.setOrigin(sf::Vector2f(0, this->_loadingShootRect.getGlobalBounds().height / 2));
	this->_loadingShootRect.setPosition(sf::Vector2f((position.x - this->_shootBarSprite.getWidth() / 2) + this->_shootBarSprite.getWidth(2.5), position.y));
}

void rtype::client::ShootBar::draw(rtype::engine::Engine* engine)
{
	_shootBarSprite.draw(engine);
	engine->window.draw(this->_loadingShootRect);
}

void rtype::client::ShootBar::update(rtype::engine::Engine* engine, rtype::actor::Actor* player)
{
	float playerReloadPercentage = player->getReloadedPercentage();

	this->_loadingShootRect.setSize(sf::Vector2f(((this->_shootBarSprite.getWidth() - this->_shootBarSprite.getWidth(4.5))  * playerReloadPercentage / 100),
		this->_shootBarSprite.getHeight() - this->_shootBarSprite.getHeight(29)));
}

void rtype::client::ShootBar::setPosition(const sf::Vector2f position)
{
	this->_position = position;

	this->_shootBarSprite.setPosition(this->_position);
	this->_loadingShootRect.setPosition(sf::Vector2f((position.x - this->_shootBarSprite.getWidth() / 2) + this->_shootBarSprite.getWidth(2.5), position.y));
}

sf::Vector2f rtype::client::ShootBar::getPosition(void) const
{
	return this->_position;
}

float rtype::client::ShootBar::getWidth(void)
{
	return this->_shootBarSprite.getWidth();
}

float rtype::client::ShootBar::getWidth(const float percentage)
{
	return this->_shootBarSprite.getWidth(percentage);
}

float rtype::client::ShootBar::getHeight(void)
{
	return this->_shootBarSprite.getHeight();
}

float rtype::client::ShootBar::getHeight(const float percentage)
{
	return this->_shootBarSprite.getHeight(percentage);
}