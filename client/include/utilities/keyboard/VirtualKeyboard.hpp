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
//  File:            VirtualKeyboard.hpp                                             //
//  Description:     VirtualKeyboard file headers                                    //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "Engine.hpp"
#include "CustomRect.hpp"
#include "VirtualKeys.hpp"

namespace rtype {
	namespace client {
		namespace utilities {
			class VirtualKeyboard {
				public:
					enum keyboardPosition {
						TOP,
						BOTTOM,
					};

					enum keyboardState {
						CLOSE,
						CLOSING,
						OPENING,
						OPEN,
					};

					VirtualKeyboard(rtype::client::utilities::VirtualKeyboard::keyboardPosition keyboardPos = rtype::client::utilities::VirtualKeyboard::keyboardPosition::BOTTOM);

					void updateJoystickEvent(sf::Event event, rtype::engine::Engine* engine);
					void updateEvent(sf::Event event, rtype::engine::Engine* engine);
					void update(rtype::engine::Engine* engine);

					void draw(rtype::engine::Engine* engine);
					rtype::client::utilities::VirtualKeyboard::keyboardState getState(void);

					void setPosition(sf::Vector2f position);

					void openKeyboard(void);
					void closeKeyboard(void);

					bool isHover(sf::Vector2f mousePos);

					std::string getLastKeyPressed(void);

					rtype::client::utilities::VirtualKeyboard::keyboardState getKeyboardState(void);

				private:
					sf::RectangleShape _keyboardBackground;
					rtype::client::utilities::VirtualKeyboard::keyboardState _keyboardState;
					rtype::client::utilities::VirtualKeyboard::keyboardPosition _keyboardPosition;
					std::vector<rtype::client::utilities::VirtualKeys> _allKeys;

					sf::Clock _moveClock;
					sf::Vector2f _position;

					int _actualFocusEventID;

					void updateState(rtype::engine::Engine* engine);

					bool isOpened(rtype::engine::Engine* engine);
					bool isClosed(rtype::engine::Engine* engine);

					void createKeys(rtype::engine::Engine* engine);

			};

		};
	};
};
