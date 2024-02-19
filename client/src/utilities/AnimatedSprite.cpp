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
//  File:            AnimatedSprite.hpp                                              //
//  Description:     File for the AnimatedSprite Class                               //
//                                                                                   //
// ********************************************************************************* //
#include "AnimatedSprite.hpp"
#include <iostream>

rtype::client::utilities::AnimatedSprite::AnimatedSprite(std::string spritePath, sf::Vector2f position, sf::Vector2f rectSize, rtype::client::utilities::AnimatedSprite::styleSheet styleSheet, sf::Vector2i beginAnimation, sf::Vector2i endAnimation, float animationSpeed, bool isLoop, rtype::client::utilities::AnimatedSprite::direction direction)
{
	_texture = std::make_shared<sf::Texture>();

	if (!_texture)
		throw rtype::settings::Errors("Not enough memories", "MemoryError:");
	if (!_texture->loadFromFile(spritePath)) {
		if (!_texture->loadFromFile(MISSING_TEXTURE))
			throw rtype::settings::Errors("File not found", "TextureError:");
	}

	_direction = direction;

	_beginAnimation = beginAnimation;
	_endAnimation = endAnimation;

	_animationSpeed = animationSpeed;

	_styleSheet = styleSheet;

	_isLoop = isLoop;
	_isFinish = false;

	_spriteRect.width = rectSize.x;
	_spriteRect.height = rectSize.y;
	_spriteRect.left = beginAnimation.x;
	_spriteRect.top = beginAnimation.y;

	_sprite.setTexture(*_texture);

	_sprite.setOrigin(sf::Vector2(rectSize.x / 2, rectSize.y / 2));

	_spriteSize = _sprite.getLocalBounds();

	_sprite.setTextureRect(_spriteRect);
	_sprite.setPosition(position);

}

void rtype::client::utilities::AnimatedSprite::setPosition(sf::Vector2f position)
{
	_sprite.setPosition(position);
}

sf::Sprite& rtype::client::utilities::AnimatedSprite::getSprite(void)
{
	return _sprite;
}

sf::Vector2f rtype::client::utilities::AnimatedSprite::getPosition(void)
{
	return _sprite.getPosition();
}

float rtype::client::utilities::AnimatedSprite::getWidth(void)
{
	return _sprite.getGlobalBounds().width;
}

float rtype::client::utilities::AnimatedSprite::getWidth(float percentage)
{
	return this->getWidth() * percentage / 100;
}

float rtype::client::utilities::AnimatedSprite::getHeight(void)
{
	return _sprite.getGlobalBounds().height;
}

float rtype::client::utilities::AnimatedSprite::getHeight(float percentage)
{
	return this->getHeight() * percentage / 100;
}

void rtype::client::utilities::AnimatedSprite::updateAnimation(void)
{
	sf::Time time = _clock.getElapsedTime();
	float seconds = time.asSeconds();

	if (seconds > _animationSpeed && ((!_isLoop && !_isFinish) || (_isLoop))) {
		if (_styleSheet == rtype::client::utilities::AnimatedSprite::styleSheet::HORIZONTAL) {
			if (_direction == rtype::client::utilities::AnimatedSprite::direction::RIGHT)
				_spriteRect.left += _spriteRect.width;
			else
				_spriteRect.left -= _spriteRect.width;
			if (_spriteRect.left > _spriteSize.width || (_spriteRect.left > _endAnimation.x && _direction == rtype::client::utilities::AnimatedSprite::direction::RIGHT) || (_spriteRect.left < 0 && _direction == rtype::client::utilities::AnimatedSprite::direction::LEFT)) {
				_spriteRect.left = _beginAnimation.x;
				_spriteRect.top += _spriteRect.height;
				if (_spriteRect.height > _spriteSize.height || _spriteRect.top > _endAnimation.y) {
					_spriteRect.top = _beginAnimation.y;
					if (!_isLoop) {
						_isFinish = true;
						return;
					}
				}
			}
		}
		else {
			_spriteRect.top += _spriteRect.height;
			if (_spriteRect.top > _spriteSize.height || _spriteRect.top > _endAnimation.y) {
				_spriteRect.top = _beginAnimation.y;
				if (_direction == rtype::client::utilities::AnimatedSprite::direction::RIGHT)
					_spriteRect.left += _spriteRect.width;
				else
					_spriteRect.left -= _spriteRect.width;
				if (_spriteRect.left > _spriteSize.width || (_spriteRect.left > _endAnimation.x && _direction == rtype::client::utilities::AnimatedSprite::direction::RIGHT) || (_spriteRect.left < 0 && _direction == rtype::client::utilities::AnimatedSprite::direction::LEFT)) {
					_spriteRect.left = _beginAnimation.x;
					if (!_isLoop) {
						_isFinish = true;
						return;
					}
				}
			}
		}
		_sprite.setTextureRect(_spriteRect);
		_clock.restart();
	}
}

void rtype::client::utilities::AnimatedSprite::updateFrameAnimation(rtype::client::utilities::AnimatedSprite::styleSheet styleSheet, rtype::client::utilities::AnimatedSprite::direction direction, sf::Vector2i beginAnimation, sf::Vector2i endAnimation, float animationSpeed)
{
	_beginAnimation = beginAnimation;
	_isFinish = false;
	_endAnimation = endAnimation;
	_styleSheet = styleSheet;
	_spriteRect.left = beginAnimation.x;
	_spriteRect.top = beginAnimation.y;
	_direction = direction;
	_sprite.setTextureRect(_spriteRect);
	_clock.restart();
}

void rtype::client::utilities::AnimatedSprite::setScale(float scaleX, float scaleY)
{
	_sprite.setScale(sf::Vector2f(scaleX, scaleY));
}

void rtype::client::utilities::AnimatedSprite::draw(rtype::engine::Engine* engine)
{
	engine->window.draw(_sprite);
}

void rtype::client::utilities::AnimatedSprite::setOpacity(int opacity)
{
	sf::Color oldColor = _sprite.getColor();

	_sprite.setColor(sf::Color(oldColor.r, oldColor.g, oldColor.b, opacity));
}

int rtype::client::utilities::AnimatedSprite::getOpacity(void)
{
	return _sprite.getColor().a;
}

void rtype::client::utilities::AnimatedSprite::changeTexture(std::string newFile)
{
	if (!_texture->loadFromFile(newFile)) {
		if (!_texture->loadFromFile(MISSING_TEXTURE))
			throw rtype::settings::Errors("File not found", "TextureError:");
	}
	_sprite.setTexture(*_texture);
}

void rtype::client::utilities::AnimatedSprite::changeTextureRect(sf::IntRect newRect)
{
	_spriteRect = newRect;
	_sprite.setTextureRect(_spriteRect);
	_sprite.setOrigin(sf::Vector2f(_spriteRect.width / 2, _spriteRect.height / 2));
	_clock.restart();
}

sf::Color rtype::client::utilities::AnimatedSprite::getColor(void)
{
	return this->_sprite.getColor();
}

void rtype::client::utilities::AnimatedSprite::setColor(sf::Color color)
{
	this->_sprite.setColor(color);
}

bool rtype::client::utilities::AnimatedSprite::isAnimationFinish(void)
{
	return this->_isFinish;
}
