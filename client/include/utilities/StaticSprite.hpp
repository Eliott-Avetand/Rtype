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
//  File:            StaticSprite.hpp                                                //
//  Description:     File for the StaticSprite Header                                //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "ACustomSprite.hpp"
#include "GlobalUtils.hpp"
#include "Errors.hpp"

#define MISSING_TEXTURE "./client/assets/sprites/missing_texture.png"

namespace rtype {
	namespace client {
		namespace utilities {
			class StaticSprite : public ACustomSprite {
				private:
					sf::Sprite _sprite;
					std::shared_ptr<sf::Texture> _texture;

				public:

					/**
					 * @brief Create component which have all sprite'informations
					 * @see Constructor is called when this class is create
					 *
					 * @param std::string	Path of the sprite
					 * @param sf::Vector2f	Position of the sprite
					 * @param OriginPos		Relative position of the origin of the sprite
					 */
					StaticSprite(std::string spritePath = MISSING_TEXTURE , sf::Vector2f position = sf::Vector2f(0, 0), rtype::globalutils::OriginPos originPos = rtype::globalutils::OriginPos::MIDDLE);

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
					 * @brief Set the origin of the object
					 * 
					 * @param origin The coordinates of the new origin
					 */
					void setOrigin(sf::Vector2f origin);

					/**
					 * @brief Return the reference of the sprite
					 * @see This methods is called wherever you want
					 *
					 * @return sf::sprite	Reference of the sprite
					 */
					sf::Sprite &getSprite(void);

					/**
					 * @brief Return the position of the sprite
					 * @see This methods is called wherever you want
					 *
					 * @return sf::Vector2f	Return position of the current sprite
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
			};
		};
	};
}