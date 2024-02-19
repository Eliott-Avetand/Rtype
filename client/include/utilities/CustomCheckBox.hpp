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
//  File:            CustomCheckBox.hpp                                              //
//  Description:     File for the CustomCheckBox Header                              //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include "StaticSprite.hpp"
#include "CustomButton.hpp"
#include "Engine.hpp"

#define CHECK_BOX_TEXTURE "./client/assets/sprites/UI/EmptyCheckBox.png"
#define CHECKED_TEXTURE "./client/assets/sprites/UI/Checked.png"

namespace rtype {

	namespace checkboxutils {
		enum joystickState {
			ARRIVED,
			STAY,
			CHANGE,
		};
	}

	namespace client {
		namespace utilities {

			class CustomCheckBox {
				public:
					enum checkboxState {
						NORMAL,
						HOVER,
					};

					/**
					 * @brief Construct CustomCheckBox object
					 * 
					 * @param position Position of the checkbox in the window
					 * @param size Size of the checkbox in the window
					 * @param callback Set the callback of the button, called when it's pressed
					 * @param isEnable Set if the checkbox is enabled or not
					 */
					CustomCheckBox(sf::Vector2f position, float size, rtype::buttonutils::callback_t callback, bool isEnable = true, int id = -1);

					/**
					 * @brief Update the callback of the checkbox
					 *
					 * @see typedef callback_t
					 * @param callback A callback as a function pointer
					 */
					void setCallback(rtype::buttonutils::callback_t callback);

					/**
					 * @brief Set the position of the checkbox in the window
					 *
					 * @param position Positon of the checkbox in the window
					 */
					void setPosition(sf::Vector2f position);

					/**
					 * @brief Move the checkbox in the window
					 *
					 * @param offset Offset of move in the checkbox in the window
					 */
					void move(sf::Vector2f offset);

					/**
					 * @brief Check if the checkbox is enabled or not
					 *
					 * @return True if checkbox is enabled, otherwise false
					 */
					bool isEnable(void);

					/**
					 * @brief Draw the component in the window
					 *
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void draw(rtype::engine::Engine* engine);

					/**
					 * @brief Check if the checkbox is hovered or not
					 * 
					 * @param mousePosition The position of the mouse as a sf::Vector2f
					 * @return True if checkbox is hovered, otherwise false
					 */
					bool isHover(sf::Vector2f mousePosition);

					/**
					 * @brief Define the behavior when the checkbox is pressed
					 *
					 * @param isSound Boolean that represent if the click sound should be played or not
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void click(bool isSound, rtype::engine::Engine* engine);

					/**
					 * @brief Define the behavior when the button is hovered
					 *
					 * @param isSound Boolean that represent if the hover sound should be played or not
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void hover(bool isSound, rtype::engine::Engine* engine);

					/**
					 * @brief Set the state of the button to 'released'
					 */
					void released(void);

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
					rtype::client::utilities::StaticSprite _checkBoxSprite;
					rtype::client::utilities::StaticSprite _isEnableSprite;

					bool _isEnable;

					rtype::buttonutils::callback_t _callback;

					rtype::client::utilities::CustomCheckBox::checkboxState _checkboxState;
					rtype::checkboxutils::joystickState _joystickState;
					sf::Clock _joystickClock;

					int _id;
					int _rightElementsID;
					int _leftElementsID;
					int _upElementsID;
					int _downElementsID;

					float _size;

			};

		};
	};
};