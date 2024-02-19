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
//  Description:     File for the AnimatedSprite Header                              //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "ACustomSprite.hpp"
#include "Errors.hpp"

#define MISSING_TEXTURE "./client/assets/sprites/missing_texture.png"

namespace rtype {
	namespace client {
		namespace utilities {
			class AnimatedSprite : public ACustomSprite {
				public:
					enum styleSheet {
						HORIZONTAL,
						VERTICAL
					};

					enum direction {
						LEFT,
						RIGHT
					};

					/**
					 * @brief Create component which have all sprite'informations
					 * @see Constructor is called when this class is create
					 *
					 * @param std::string	Path of the sprite
					 * @param sf::Vector2f	Position of the sprite
					 * @param sf::Vector2f	Size Rectangle of the sprite
					 * @param styleSheet	Is the styleSheet is horizontal or vertical
					 * @param sf::Vector2i	Born of the begin animation
					 * @param sf::Vector2i	Born of the ending animation
					 * @param float			Animation speed of the sprite's movement
					 * @param bool			True if animation can loop and false otherwise
					 * @param direction		Is the spriteSheet is reading left or right
					 */
					AnimatedSprite(std::string spritePath = MISSING_TEXTURE, sf::Vector2f position = sf::Vector2f(0, 0), sf::Vector2f rectSize = sf::Vector2f(0, 0), rtype::client::utilities::AnimatedSprite::styleSheet styleSheet = rtype::client::utilities::AnimatedSprite::HORIZONTAL, sf::Vector2i beginAnimation = sf::Vector2i(0, 0), sf::Vector2i endAnimation = sf::Vector2i(0, 0), float animationSpeed = 0.2, bool isLoop = true, rtype::client::utilities::AnimatedSprite::direction direction = rtype::client::utilities::AnimatedSprite::direction::RIGHT);


					/**
					 * @brief Set the position of the sprite
					 * @see This methods is called wherever you want
					 *
					 * @param sf::Vector2	Position of the sprite
					 */
					void setPosition(sf::Vector2f position);

					/**
					 * @brief Manage opacity of ther sprite
					 * @see This methods is called wherever you want
					 */
					void setOpacity(int opacity);

					/**
					 * @brief Scale the sprite
					 * @see This methods is called wherever you want
					 */
					void setScale(float scaleX, float scaleY);

					/**
					 * @brief Update the animation of the sprite
					 * @see This methods is called in update of the scene
					 */
					void updateAnimation(void);

					/**
					 * @brief Update and change the animation of the sprite
					 * @see This methods is called in update of the scene
					 */
					void updateFrameAnimation(rtype::client::utilities::AnimatedSprite::styleSheet styleSheet, rtype::client::utilities::AnimatedSprite::direction direction, sf::Vector2i beginAnimation, sf::Vector2i endAnimation, float animationSpeed);


					/**
					 * @brief Return the position of the sprite
					 * @see This methods is called wherever you want
					 *
					 * @return sf::Vector2f	Return position of the current sprite
					 */
					sf::Sprite& getSprite(void);

					/**
					 * @brief Change the texture's sprite
					 * @see This methods is called wherever you want
					 *
					 * @param std::string Return opacity of the sprite
					 */
					void changeTexture(std::string newFile);

					/**
					 * @brief Change the rectangle of the sprite's texture
					 * @see This methods is called wherever you want
					 *
					 * @param sf::IntRect	Size of the new rect
					 */
					void changeTextureRect(sf::IntRect rect);

					/**
					 * @brief Return the reference of the sprite
					 * @see This methods is called wherever you want
					 *
					 * @return sf::sprite	Reference of the sprite
					 */
					sf::Vector2f getPosition(void);

					/**
					 * @brief Return the width of the sprite
					 * @see This methods is called wherever you want
					 *
					 * @return flaot	Return width of the current sprite
					 */
					float getWidth(void);

					/**
					 * @brief Return the width of the sprite
					 * @see This methods is called wherever you want
					 *
					 * @param percentage	percentage of width
					 * 
					 * @return float	Return width of the current sprite by a percentage
					 */
					float getWidth(float percentage);

					/**
					 * @brief Return the height of the sprite
					 * @see This methods is called wherever you want
					 *
					 * @return flaot	Return height of the current sprite
					 */
					float getHeight(void);

					/**
					 * @brief Return the height of the sprite
					 * @see This methods is called wherever you want
					 *
					 * @param percentage	percentage of width
					 *
					 * @return float	Return height of the current sprite by a percentage
					 */
					float getHeight(float percentage);

					/**
					 * @brief Return opacity of the sprite
					 * @see This methods is called wherever you want
					 *
					 * @return int		Return opacity of the sprite
					 */
					int getOpacity(void);

					/**
					 * @brief Draw the sprite
					 * @see This methods is called in render of the scene
					 */
					void draw(rtype::engine::Engine* engine);

					/**
					 * @brief Set the color of the sprite
					 * @see This methods is called wherever you want
					 *
					 * @param sf:Color		Color of the sprite
					 */
					void setColor(sf::Color color);

					/**
					 * @brief Get the color of the sprite
					 * @see This methods is called wherever you want
					 *
					 * @return sf:Color		Color of the sprite
					 */
					sf::Color getColor(void);

					/**
					 * @brief Get the animation state
					 * @see This methods is called wherever you want
					 *
					 * @return bool		True if animation is finish, false otherwise
					 */
					bool isAnimationFinish(void);
				private:

					sf::Sprite _sprite;
					std::shared_ptr<sf::Texture> _texture;

					sf::FloatRect _spriteSize;

					bool _isLoop;
					bool _isFinish;

					float _animationSpeed;
					sf::IntRect _spriteRect;
					sf::Clock _clock;
					rtype::client::utilities::AnimatedSprite::styleSheet _styleSheet;
					rtype::client::utilities::AnimatedSprite::direction _direction;
					sf::Vector2i _beginAnimation;
					sf::Vector2i _endAnimation;

			};
		};
	};
}