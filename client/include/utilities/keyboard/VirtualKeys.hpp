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
//  File:            VirtualKeys.hpp                                                 //
//  Description:     VirtualKeys file headers                                        //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "Engine.hpp"
#include "CustomRect.hpp"
#include "CustomText.hpp"
#include "InputHandler.hpp"

namespace rtype {
	namespace virtualkeysutils {
		enum joystickState {
			ARRIVED,
			STAY,
			CHANGE,
		};
	};

	namespace client {
		namespace utilities {
			class VirtualKeys {
				public:
					enum virtualKeyState {
						NORMAL,
						HOVER,
						CLICKED,
					};

					VirtualKeys(sf::Vector2f position, int id = -1, std::string key = "");

					bool isHover(sf::Vector2f mousePos);
					void hover(bool isSound, rtype::engine::Engine* engine);
					void released(void);

					void update(rtype::engine::Engine* engine);
					int eventUpdate(sf::Event event, rtype::engine::Engine* engine);
					int eventJoystickUpdate(sf::Event event, rtype::engine::Engine* engine);

					void draw(rtype::engine::Engine* engine);

					void setPosition(sf::Vector2f position);
					sf::Vector2f getPosition(void);

					sf::FloatRect getBounds(void);

					void setSize(sf::Vector2f size);

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

					/**
					 * @brief Get Key of Key Touch
					 * @see This method is called wherever you want
					 *
					 * @return std::string Key of the Ke touch
					 */
					std::string getKey(void);
				private:
					sf::RectangleShape _keyShape;
					std::unique_ptr<rtype::client::utilities::CustomText> _keyText;

					sf::Vector2f _position;
					sf::FloatRect _bounds;

					rtype::client::utilities::VirtualKeys::virtualKeyState _virtualKeyState;
					rtype::virtualkeysutils::joystickState _joystickState;
					sf::Clock _joystickClock;

					int _id;
					int _rightElementsID;
					int _leftElementsID;
					int _upElementsID;
					int _downElementsID;
			};
		};

	};

};
