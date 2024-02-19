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
//  File:            CustomRect.hpp                                                  //
//  Description:     Encapsulation of sf::RectangleShape                             //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <SFML/Graphics.hpp>

#include "Errors.hpp"
#include "Engine.hpp"
#include "GlobalUtils.hpp"

namespace rtype {
	namespace client {
		namespace utilities {
			class CustomRect {
				private:
					sf::RectangleShape _rect;

				public:
					/**
					 * @brief Default constructor (not used)
					 *
					 */
					CustomRect();

					/**
					 * @brief Encapsulation of the construction of a sf::RectangleShape
					 *
					 * @param size Size of the Rect
					 * @param color Background color of the Rect
					 * @param position Position of the Rect in the window
					 * @param originPos The position of the origin, exprimed in OriginPos enum
					 * @param outlineColor Outline color of the Rect
					 * @param outlineSize Outline size of the Rect
					 */
					CustomRect(sf::Vector2f size, sf::Color color, sf::Vector2f position, rtype::globalutils::OriginPos originPos = rtype::globalutils::OriginPos::LEFT, sf::Color outlineColor = sf::Color::Transparent, int outlineSize = 0);

					/**
					 * @brief Set the position of the rect in the window
					 *
					 * @param position Position in the screen of the rect
					*/
					void setPosition(sf::Vector2f position);

					/**
					 * @brief Set the background color of the Rect
					 *
					 * @param color Color as a sf::Color
					 */
					void setColor(sf::Color color);

					/**
					 * @brief Set the outline color of the Rect
					 *
					 * @param color Color as a sf::Color
					 */
					void setOutlineColor(sf::Color color);

					/**
					 * @brief Set the outline size of the Rect
					 *
					 * @param thickness The size of the outline (0 to disable outline)
					 */
					void setOutlineThickness(int thickness);

					/**
					 * @brief Set the origin of the Rect exprimed in coordinates
					 *
					 * @param x Coordinate x of the origin
					 * @param y Coordinate y of the origin
					 */
					void setOrigin(float x, float y);

					/**
					 * @brief Get the position of the Rect in the windox
					 */
					sf::Vector2f getPosition(void);

					/**
					 * @brief Return the width of the component
					*/
					float getWidth(void);

					/**
					 * @brief Return the height of the component
					*/
					float getHeight(void);

					/**
					 * @brief Get the background color of the Rect
					 * 
					 * @return The color of the background as a sf::Color
					 */
					sf::Color getColor(void);

					/**
					 * @brief Get the local bounds of the Rect
					 * 
					 * @return The Rect bounds as a sf::FloatRect
					 */
					sf::FloatRect getLocalBounds(void);

					/**
					 * @brief Get the global bounds of the Rect
					 * 
					 * @return The Rect bounds as a sf::FloatRect
					 */
					sf::FloatRect getGlobalBounds(void);

					/**
					 * @brief Draw the component in the window
					 *
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void draw(rtype::engine::Engine* engine);

					/**
					 * @brief Get the sf::RectangleShape contained in CustomRect
					 * 
					 * @return The rectangle as a sf::RectangleShape &
					 */
					sf::RectangleShape &getRect(void);
			};
		};
	};
}