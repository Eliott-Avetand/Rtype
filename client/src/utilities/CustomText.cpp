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
//  File:            CustomText.cpp                                                  //
//  Description:     Encapsulation of sf::Text                                       //
//                                                                                   //
// ********************************************************************************* //

#include "CustomText.hpp"

rtype::client::utilities::CustomText::CustomText(std::string content, std::string fontPath, int fontSize, sf::Color color, sf::Vector2f position)
{
	sf::FloatRect localBounds;

	if (!this->_font.loadFromFile(fontPath))
		throw rtype::settings::Errors("Cannot load file " + fontPath, "FileError");
	this->_text.setFont(this->_font);
	this->_text.setString(content);
	this->_text.setCharacterSize(fontSize);
	this->_text.setFillColor(color);
	localBounds = this->_text.getLocalBounds();
	this->_text.setOrigin(localBounds.left + localBounds.width / 2.0f, localBounds.top + localBounds.height / 2.0f);
	this->_text.setPosition(position);
}

void rtype::client::utilities::CustomText::setPosition(sf::Vector2f position)
{
	this->_text.setPosition(position);
}

void rtype::client::utilities::CustomText::move(sf::Vector2f offset)
{
	this->_text.move(offset);
}


sf::Vector2f rtype::client::utilities::CustomText::getPosition(void)
{
	return this->_text.getPosition();
}

float rtype::client::utilities::CustomText::getWidth(void)
{
	return this->_text.getGlobalBounds().width;
}

float rtype::client::utilities::CustomText::getHeight(void)
{
	return this->_text.getGlobalBounds().height;
}

sf::FloatRect rtype::client::utilities::CustomText::getLocalBounds(void)
{
	return this->_text.getLocalBounds();
}

sf::FloatRect rtype::client::utilities::CustomText::getGlobalBounds(void)
{
	return this->_text.getGlobalBounds();
}

void rtype::client::utilities::CustomText::draw(rtype::engine::Engine* engine)
{
	engine->window.draw(this->_text);
}

void rtype::client::utilities::CustomText::setColor(sf::Color color)
{
	this->_text.setFillColor(color);
}

void rtype::client::utilities::CustomText::setOrigin(float x, float y)
{
	this->_text.setOrigin(x, y);
}

void rtype::client::utilities::CustomText::setText(std::string text)
{
	this->_text.setString(text);

	sf::FloatRect localBounds = this->_text.getLocalBounds();
	this->_text.setOrigin(localBounds.left + localBounds.width / 2.0f, localBounds.top + localBounds.height / 2.0f);
}

std::string rtype::client::utilities::CustomText::getText(void)
{
	return this->_text.getString();
}

sf::Text &rtype::client::utilities::CustomText::getTextObject(void)
{
	return this->_text;
}

sf::Color rtype::client::utilities::CustomText::getColor(void)
{
	return this->_text.getFillColor();
}

void rtype::client::utilities::CustomText::setCharacterSize(int size)
{
	this->_text.setCharacterSize(size);

	sf::FloatRect localBounds = this->_text.getLocalBounds();
	this->_text.setOrigin(localBounds.left + localBounds.width / 2.0f, localBounds.top + localBounds.height / 2.0f);
}
