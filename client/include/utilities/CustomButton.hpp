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
//  File:            CustomButton.hpp                                                //
//  Description:     File for the CustomButton Header                                //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <iostream>
#include <memory>

#include "Engine.hpp"
#include "CustomText.hpp"
#include "InputHandler.hpp"

namespace rtype {
	namespace buttonutils {
        struct CallbackData {
            std::string roomName;
			sf::Color color;
        };

		enum joystickState {
			ARRIVED,
			STAY,
			CHANGE,
		};

        typedef void (*callback_t)(rtype::engine::Engine* engine, struct CallbackData &data);
    };

	namespace client {
		namespace utilities {
			class CustomButton {
				public:
					enum buttonState {
						NORMAL,
						HOVER,
						CLICKED,
						LOCKED
					};

					/**
					 * @brief Construct a CustomButton object
					 *
					 * @param text Text displayed in the button
					 * @param position Position of the button
					 * @param fontSize Font used to display the text
					 * @param callback A callback that will be called when the button is pressed
					 */
					CustomButton(std::string text, sf::Vector2f position, float fontSize, rtype::buttonutils::callback_t callback, int id);

					/**
					 * @brief Draw the component in the window
					 *
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void draw(rtype::engine::Engine* engine);

					/**
					 * @brief Define the behavior when the button is hovered
					 *
					 * @param isSound Boolean that represent if the hover sound should be played or not
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void hover(bool isSound, rtype::engine::Engine* engine);

					/**
					 * @brief Set the state of the button to 'pressed'
					 */
					void press(void);

					/**
					 * @brief Define the behavior when the button is pressed
					 *
					 * @param isSound Boolean that represent if the click sound should be played or not
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void click(bool isSound, rtype::engine::Engine* engine);

					/**
					 * @brief Set the state of the button to 'released'
					 */
					void released(void);

					/**
					 * @brief Check if the button is hovered or not
					 *
					 * @param mousePosition The position of the mouse as a sf::Vector2f
					 * @return True if button is hovered, otherwise false
					 */
					bool isHover(sf::Vector2f mousePosition);

					/**
					 * @brief Handle events related to the component
					 *
					 * @param event sf::Event contained in GameEngine, used to detect what event is fired by user
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					int eventUpdate(sf::Event event, rtype::engine::Engine* engine);

					/**
					 * @brief Handle events related to the component
					 *
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void update(rtype::engine::Engine* engine);

					/**
					 * @brief Handle events related to the component by the joystick
					 *
					 * @param event sf::Event contained in GameEngine, used to detect what event is fired by user
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					int eventUpdateJoystick(sf::Event event, rtype::engine::Engine* engine);

					/**
					 * @brief Update the callback of the button
					 *
					 * @see typedef callback_t
					 * @param callback A callback as a function pointer
					 */
					void setCallback(rtype::buttonutils::callback_t callback);

					/**
					 * @brief Set the color of the button
					 *
					 * @param color The color as a sf::Color
					 */
					void setColor(sf::Color color);

					/**
					 * @brief Set the position of the button in the window
					 *
					 * @param position Positon of the button in the window
					 */
					void setPosition(sf::Vector2f position);

					/**
					 * @brief Move button in the window
					 *
					 * @param offset Offset of the button in the window
					 */
					void move(sf::Vector2f offset);

					/**
					 * @brief Set the origin of the Rect exprimed in coordinates
					 *
					 * @param x Coordinate x of the origin
					 * @param y Coordinate y of the origin
					 */
					void setOrigin(float x, float y);

					/**
					 * @brief Return the width of the component
					*/
					float getWidth(void);

					/**
					 * @brief Return the height of the component
					*/
					float getHeight(void);

					/**
					 * @brief Set the data passed to callback function
					 * 
					 * @see CallbackData struct
					 * @param data Data that will be passed to callback as CallbackStat struct
					 */
					void setAdditionalData(rtype::buttonutils::CallbackData &data);

					/**
					 * @brief Get the text object contained in the button
					 *
					 * @return The text object as a std::unique_ptr<CustomText> &
					 */
					std::unique_ptr<CustomText> &getText();

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
					std::unique_ptr<CustomText> _text;
					float _fontSize;
					sf::Color _color;
					buttonState _state;
					sf::FloatRect _bounds;

					rtype::buttonutils::joystickState _joystickState;
					sf::Clock _joystickClock;

					int _id;
					int _rightElementsID;
					int _leftElementsID;
					int _upElementsID;
					int _downElementsID;

					rtype::buttonutils::callback_t _callback;
					rtype::buttonutils::CallbackData _data;
			};
		};
	};
};