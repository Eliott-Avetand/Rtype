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
//  File:            InputBox.hpp                                                    //
//  Description:     Input box where player can type a text                          //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <iostream>
#include <memory>

#include "Engine.hpp"
#include "CustomText.hpp"
#include "CustomRect.hpp"
#include "VirtualKeyboard.hpp"
#include "InputHandler.hpp"

#define INPUT_MAX_CHAR 26

namespace rtype {
	namespace inputboxutils {
		enum joystickState {
			ARRIVED,
			STAY,
			FOCUS,
			CHANGE,
		};
	};

	namespace client {
		namespace utilities {
			class InputBox {
				public:
					enum inputboxState {
						NORMAL,
						HOVER,
					};

					/**
					 * @brief Construct a new Input Box object
					 *
					 * @param position Position of the component
					 * @param size Size of the component
					 * @param fontSize Size of the font in pc
					 * @param placeholder Content of the placeholder, displayed when nothing is enterred in the Input Box
					 */
					InputBox(sf::Vector2f position, sf::Vector2f size, float fontSize, std::string placeholder = "", int id = -1);

					/**
					 * @brief Draw the component in the window
					 *
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void draw(rtype::engine::Engine* engine);

					void drawKeyboard(rtype::engine::Engine* engine);

					/**
					 * @brief Handle events related to the component
					 *
					 * @param event sf::Event contained in GameEngine, used to detect what event is fired by user
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					int eventUpdate(sf::Event event, rtype::engine::Engine* engine);

					/**
					 * @brief Handle events related to the component by the joystick
					 *
					 * @param event sf::Event contained in GameEngine, used to detect what event is fired by user
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					int eventUpdateJoystick(sf::Event event, rtype::engine::Engine* engine);

					/**
					 * @brief Handle update
					 *
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void update(rtype::engine::Engine* engine);


					/**
					 * @brief Set the position of the component
					 *
					 * @param position Position in the screen of the component
					*/
					void setPosition(sf::Vector2f position);

					/**
					 * @brief Move the InputBox by offset
					 *
					 * @param offset The offset to move the InputBox as sf::Vector2f
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
					 * @brief Get the content of the InputBox
					 *
					 * @return The content of the input as a std::string
					 */
					const std::string getContent(void);

					/**
					 * @brief Update the content of the InputBox
					 *
					 * @details Used to fill the InputBox in case the data is already known
					 * @param content Content to set, as a std::string
					 */
					void setContent(std::string content);

					/**
					 * @brief Set ID of the button
					 * @see This method is called wherever you want
					 *
					 * @param ID	ID of the slider
					 */
					void setId(int Id);

					/**
					 * @brief Get ID of the button
					 * @see This method is called wherever you want
					 *
					 * @return ID	ID of the slider
					 */
					int getId(void);

					/**
					 * @brief Get ID of the right element
					 * @see This method is called wherever you want
					 *
					 * @return ID	ID of the right element
					 */
					int getRightElements(void);

					/**
					 * @brief Get ID of the left element
					 * @see This method is called wherever you want
					 *
					 * @return ID	ID of the left element
					 */
					int getLeftElements(void);

					/**
					 * @brief Get ID of the up element
					 * @see This method is called wherever you want
					 *
					 * @return ID	ID of the up element
					 */
					int getUpElements(void);

					/**
					 * @brief Get ID of the down element
					 * @see This method is called wherever you want
					 *
					 * @return ID	ID of the down element
					 */
					int getDownElements(void);

					/**
					 * @brief Set ID of the right element
					 * @see This method is called wherever you want
					 *
					 * @param ID	ID of the right element
					 */
					void setRightElements(int Id);

					/**
					 * @brief Set ID of the left element
					 * @see This method is called wherever you want
					 *
					 * @param ID	ID of the left element
					 */
					void setLeftElements(int Id);

					/**
					 * @brief Set ID of the up element
					 * @see This method is called wherever you want
					 *
					 * @param ID	ID of the up element
					 */
					void setUpElements(int Id);

					/**
					 * @brief Set ID of the down element
					 * @see This method is called wherever you want
					 *
					 * @param ID	ID of the down element
					 */
					void setDownElements(int Id);

				private:
					int _id;
					int _rightElementsID;
					int _leftElementsID;
					int _upElementsID;
					int _downElementsID;

					rtype::client::utilities::InputBox::inputboxState _state;
					rtype::inputboxutils::joystickState _joystickState;
					sf::Clock _joystickClock;

					std::unique_ptr<rtype::client::utilities::VirtualKeyboard> _keyboard;

					bool _isFocused;
					sf::String _rawInput;
					std::unique_ptr<rtype::client::utilities::CustomText> _displayedText;
					std::unique_ptr<rtype::client::utilities::CustomText> _placeholder;
					rtype::client::utilities::CustomRect _box;

					/**
					 * @brief Release focus of slider
					 * @see This method is called wherever you want
					 */
					void released(void);

					/**
					 * @brief Define the behavior when the button is hovered
					 *
					 * @param isSound Boolean that represent if the hover sound should be played or not
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void hover(bool isSound, rtype::engine::Engine* engine);
			};
		};
	};
};