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
//  File:            CustomRect.cpp                                                  //
//  Description:     Encapsulation of sf::RectangleShape                             //
//                                                                                   //
// ********************************************************************************* //

#include "CustomRect.hpp"

rtype::client::utilities::CustomRect::CustomRect()
{
	// Void
}

rtype::client::utilities::CustomRect::CustomRect(sf::Vector2f size, sf::Color color, sf::Vector2f position, rtype::globalutils::OriginPos originPos, sf::Color outlineColor, int outlineSize)
{
	this->_rect = sf::RectangleShape(size);
    this->_rect.setFillColor(color);
	if (outlineSize != 0) {
		this->_rect.setOutlineColor(outlineColor);
		this->_rect.setOutlineThickness(outlineSize);
	}
	sf::Vector2f origin = rtype::globalutils::returnOriginPos(originPos, this->_rect.getLocalBounds());
	this->_rect.setOrigin(origin.x, origin.y);
	this->_rect.setPosition(position);
}

void rtype::client::utilities::CustomRect::setPosition(sf::Vector2f position)
{
	this->_rect.setPosition(position);
}

sf::Vector2f rtype::client::utilities::CustomRect::getPosition(void)
{
	return this->_rect.getPosition();
}

float rtype::client::utilities::CustomRect::getWidth(void)
{
	return this->_rect.getGlobalBounds().width;
}

float rtype::client::utilities::CustomRect::getHeight(void)
{
	return this->_rect.getGlobalBounds().height;
}

sf::FloatRect rtype::client::utilities::CustomRect::getLocalBounds(void)
{
	return this->_rect.getLocalBounds();
}

sf::FloatRect rtype::client::utilities::CustomRect::getGlobalBounds(void)
{
	return this->_rect.getGlobalBounds();
}

void rtype::client::utilities::CustomRect::draw(rtype::engine::Engine* engine)
{
	engine->window.draw(this->_rect);
}

void rtype::client::utilities::CustomRect::setColor(sf::Color color)
{
	this->_rect.setFillColor(color);
}

void rtype::client::utilities::CustomRect::setOutlineColor(sf::Color color)
{
	this->_rect.setOutlineColor(color);
}

void rtype::client::utilities::CustomRect::setOutlineThickness(int thickness)
{
	this->_rect.setOutlineThickness(thickness);
}

void rtype::client::utilities::CustomRect::setOrigin(float x, float y)
{
	this->_rect.setOrigin(x, y);
}

sf::Color rtype::client::utilities::CustomRect::getColor(void)
{
	return this->_rect.getFillColor();
}

sf::RectangleShape &rtype::client::utilities::CustomRect::getRect(void)
{
	return this->_rect;
}