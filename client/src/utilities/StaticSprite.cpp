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
//  File:            StaticSprite.cpp                                                //
//  Description:     File for the StaticSprite Class                                 //
//                                                                                   //
// ********************************************************************************* //

#include "StaticSprite.hpp"

rtype::client::utilities::StaticSprite::StaticSprite(std::string spritePath, sf::Vector2f position, rtype::globalutils::OriginPos originPos)
{
	_texture = std::make_shared<sf::Texture>();

	if (!_texture)
		throw rtype::settings::Errors("Not enough memories", "MemoryError:");
	if (!_texture->loadFromFile(spritePath)) {
		if (!_texture->loadFromFile(MISSING_TEXTURE))
			throw rtype::settings::Errors("File not found", "TextureError:");
	}
	_sprite.setTexture(*_texture);
	sf::Vector2f origin = rtype::globalutils::returnOriginPos(originPos, _sprite.getGlobalBounds());
	_sprite.setOrigin(origin.x, origin.y);
	_sprite.setPosition(position);
}

void rtype::client::utilities::StaticSprite::setPosition(sf::Vector2f position)
{
	_sprite.setPosition(position);
}

void rtype::client::utilities::StaticSprite::setOrigin(sf::Vector2f origin)
{
	_sprite.setOrigin(origin);
}


sf::Sprite &rtype::client::utilities::StaticSprite::getSprite(void)
{
	return _sprite;
}

sf::Vector2f rtype::client::utilities::StaticSprite::getPosition(void)
{
	return _sprite.getPosition();
}

float rtype::client::utilities::StaticSprite::getWidth(void)
{
	return _sprite.getGlobalBounds().width;
}

float rtype::client::utilities::StaticSprite::getWidth(float percentage)
{
	return this->getWidth() * percentage / 100;
}

float rtype::client::utilities::StaticSprite::getHeight(void)
{
	return _sprite.getGlobalBounds().height;
}

float rtype::client::utilities::StaticSprite::getHeight(float percentage)
{
	return this->getHeight() * percentage / 100;
}

void rtype::client::utilities::StaticSprite::setScale(float scaleX, float scaleY)
{
	_sprite.setScale(sf::Vector2f(scaleX, scaleY));
}

void rtype::client::utilities::StaticSprite::draw(rtype::engine::Engine* engine)
{
	engine->window.draw(_sprite);
}

void rtype::client::utilities::StaticSprite::setOpacity(int opacity)
{
	sf::Color oldColor = _sprite.getColor();

	_sprite.setColor(sf::Color(oldColor.r, oldColor.g, oldColor.b, opacity));
}

int rtype::client::utilities::StaticSprite::getOpacity(void)
{
	return _sprite.getColor().a;
}

sf::Color rtype::client::utilities::StaticSprite::getColor(void)
{
	return this->_sprite.getColor();
}

void rtype::client::utilities::StaticSprite::setColor(sf::Color color)
{
	this->_sprite.setColor(color);
}
