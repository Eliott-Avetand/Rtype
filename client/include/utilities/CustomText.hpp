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
//  File:            CustomText.hpp                                                  //
//  Description:     Encapsulation of sf::Text                                       //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <SFML/Graphics.hpp>

#include "Errors.hpp"
#include "Engine.hpp"

#define FONT_PATH "./client/assets/fonts/font.ttf"

namespace rtype {
	namespace client {
		namespace utilities {
			class CustomText {
				private:
					sf::Text _text;
					sf::Font _font;

				public:

					/**
					 * @brief Construct the encapsulation of the sf::Text
					 *
					 * @param content The text that will be displayed by the sf::Text
					 * @param fontPath The font used to display the text
					 * @param fontSize Font size of the text
					 * @param color Color of the text
					 * @param position Positon of the text in the window
					 */
					CustomText(std::string content = "", std::string fontPath = FONT_PATH, int fontSize = 0, sf::Color color = sf::Color(0, 0, 0), sf::Vector2f position = sf::Vector2f(0, 0));

					/**
					 * @brief Update the content of the text
					 *
					 * @param text The conent as a std::string
					 */
					void setText(std::string text);

					/**
					 * @brief Get the content of the text
					 *
					 * @return The content of the text as std::string
					 */
					std::string getText(void);

					/**
					 * @brief Get the sf::Text stored in the object
					 *
					 * @return The sf::Text object
					 */
					sf::Text &getTextObject(void);

					/**
					 * @brief Set the origin of the Rect exprimed in coordinates
					 *
					 * @param x Coordinate x of the origin
					 * @param y Coordinate y of the origin
					 */
					void setOrigin(float x, float y);

					/**
					 * @brief Set the size of the font
					 *
					 * @param size Size of font
					 */
					void setCharacterSize(int size);

					/**
					 * @brief Get the position of the Rect in the windox
					 */
					sf::Vector2f getPosition(void);

					/**
					 * @brief Set the position of the text in the window
					 *
					 * @param position Positon of the text in the window
					 */
					void setPosition(sf::Vector2f position);

					/**
					 * @brief Move the text in the window
					 *
					 * @param offset Offset of the text in the window
					 */
					void move(sf::Vector2f offset);

					/**
					 * @brief Return the width of the component
					*/
					float getWidth(void);

					/**
					 * @brief Return the height of the component
					*/
					float getHeight(void);

					/**
					 * @brief Get the color of the text
					 *
					 * @return The color of the text as a sf::Color
					 */
					sf::Color getColor(void);

					/**
					 * @brief Set the color of the text
					 *
					 * @param color The color as a sf::Color
					 */
					void setColor(sf::Color color);

					/**
					 * @brief Get the local bounds of the text
					 *
					 * @return The text bounds as a sf::FloatRect
					 */
					sf::FloatRect getLocalBounds(void);

					/**
					 * @brief Get the global bounds of the text
					 *
					 * @return The text bounds as a sf::FloatRect
					 */
					sf::FloatRect getGlobalBounds(void);

					/**
					 * @brief Draw the component in the window
					 *
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void draw(rtype::engine::Engine* engine);
			};
		};
	};
}