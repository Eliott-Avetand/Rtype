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
//  File:            SlotInfoComponent.hpp                                           //
//  Description:     Component: Slot infos displayed in MultiplayerWaitingRoom       //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "Engine.hpp"
#include "Errors.hpp"
#include "StaticSprite.hpp"
#include "CustomText.hpp"
#include "CustomCheckBox.hpp"
#include "CustomButton.hpp"

#define SLOTINFO_BORDER "./client/assets/sprites/MultiplayerRoom/borderSlotInfoComponent.png"

/**
 * @brief Callback that set if the player is ready to launch the game, or not
 *
 * @param engine GameEngine pointer that will be used to draw in the window
 * @param data Data that will be passed to callback
 */
void toggleReadyState(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data);

namespace rtype {
	namespace client {
		namespace utilities {
			class SlotInfoComponent {
				private:
                    rtype::client::utilities::StaticSprite _borderSprite;
                    std::unique_ptr<rtype::client::utilities::CustomText> _playerName;
                    std::unique_ptr<rtype::client::utilities::CustomCheckBox> _readyCheckbox;

				public:
					/**
					 * @brief Create a component that contains the pseudo of a player in the room
					 *
					 * @param name Pseudo of the player
					 * @param position Position of the component in the window
					*/
                    SlotInfoComponent(std::string name, sf::Vector2f position);

					/**
					 * @brief Set the position of the component
					 *
					 * @param position Position in the screen of the component
					*/
					void setPosition(sf::Vector2f position);

					/**
					 * @brief Move the component in the window
					 *
					 * @param offset Offset of the component
					*/
					void move(sf::Vector2f offset);

					/**
					 * @brief Update the pseudo of the player displayed in the component
					 *
					 * @param name The new pseudo of the player
					*/
					void setPlayerName(std::string name);

					/**
					 * @brief Get the position of the component
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
					 * @brief Update position of _playerName relatively to the postion of _borderSprite
					*/
                    void updatePlayerNamePosition(void);

                    std::unique_ptr<rtype::client::utilities::CustomCheckBox> &getReadyCheckbox(void);
					std::string getPlayerNameInSlot(void);

					/**
					 * @brief Draw the component in the window
					*/
					void draw(rtype::engine::Engine* engine);
			};
		};
	};
}