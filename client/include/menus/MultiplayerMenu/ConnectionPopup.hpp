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
//  File:            ConnectionPopup.hpp                                             //
//  Description:     Popup displayed when user want to join a room         			 //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "Engine.hpp"
#include "CustomText.hpp"
#include "CustomButton.hpp"
#include "CustomRect.hpp"
#include "InputBox.hpp"

/**
 * @brief Button callback that close the popup
 *
 * @param engine GameEngine pointer that will be used to draw in the window
 * @param data Data that will be passed to callback
*/
void closePopup(rtype::engine::Engine *engine, rtype::buttonutils::CallbackData &data);

/**
 * @brief Button callback that redirect player to the waiting zone of the room if given psw is OK
 *
 * @param engine GameEngine pointer that will be used to draw in the window
 * @param data Data that will be passed to callback
*/
void connectToRoom(rtype::engine::Engine *engine, rtype::buttonutils::CallbackData &data);

namespace rtype {
	namespace client {
		namespace utilities {
			class ConnectionPopup {
				private:
                    bool _isOpen;

                    rtype::client::utilities::CustomRect _background;
                    rtype::client::utilities::CustomRect _popupContent;
                    std::unique_ptr<rtype::client::utilities::InputBox> _inputBox;
					std::unique_ptr<rtype::client::utilities::CustomText> _title;
					std::unique_ptr<rtype::client::utilities::CustomButton> _connectBtn;
					std::unique_ptr<rtype::client::utilities::CustomButton> _closeBtn;

					int _actualFocusEventID;

				public:

					/**
					 * @brief Create the popup that will be used to connect to a room
					 *
					 * @param pos Position in the screen of the popup
					 * @param engine GameEngine pointer that will be used to draw in the window
					*/
                    ConnectionPopup(sf::Vector2f pos, rtype::engine::Engine *engine);

					/**
					 * @brief Set the position of the popup
					 *
					 * @param position Position in the screen of the popup
					*/
					void setPosition(sf::Vector2f position);

					/**
					 * @brief Move the popup in the window
					 *
					 * @param offset Offset as a sf::Vector2f
					*/
					void move(sf::Vector2f offset);

					/**
					 * @brief Return the position of the popup
					*/
					sf::Vector2f getPosition(void);

					/**
					 * @brief Return the width of the popup
					*/
					float getWidth(void);

					/**
					 * @brief Return the height of the popup
					*/
					float getHeight(void);

					/**
					 * @brief Display popup in the screen
					 *
					 * @param roomName The name of the room you try to connect
					*/
                    void open(std::string roomName);

					/**
					 * @brief Remove popup from the screen
					*/
                    void close(void);

					/**
					 * @brief Return if the popup is displayed or not
					*/
                    bool isOpen(void);

					/**
					 * @brief Handle events related to the popup and its components
					 *
					 * @param event sf::Event contained in GameEngine, used to detect what event is fired by user
                 	 * @param engine GameEngine pointer that will be used to draw in the window
					*/
					void handleEvents(sf::Event event, rtype::engine::Engine *engine);

					/**
					 * @brief Return the content of _inputBox component
					 *
					 * @return The content of _inputBox as a std::string
					*/
					const std::string getPasswordInputContent(void);

					/**
					 * @brief Draw the popup in the window
					 *
                 	 * @param engine GameEngine pointer that will be used to draw in the window
					*/
					void draw(rtype::engine::Engine* engine);

					/**
					 * @brief Update the popup in the window
					 *
					 * @param engine GameEngine pointer that will be used to draw in the window
					*/
					void update(rtype::engine::Engine* engine);
			};
		};
	};
}