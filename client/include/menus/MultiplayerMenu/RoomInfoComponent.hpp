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
//  File:            RoomInfoComponent.hpp                                           //
//  Description:     Component: Room infos displayed in MultiplayerRoomMenu          //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "Engine.hpp"
#include "Errors.hpp"
#include "StaticSprite.hpp"
#include "CustomText.hpp"
#include "CustomButton.hpp"

#define ROOMINFO_BORDER "./client/assets/sprites/MultiplayerRoom/borderRoomInfoComponent.png"

/**
 * @brief Button callback that open the popup
 *
 * @param engine GameEngine pointer that will be used to draw in the window
 * @param data Data that will be passed to callback
*/
void openPopup(rtype::engine::Engine *engine, rtype::buttonutils::CallbackData &data);

namespace rtype {
	namespace client {
		namespace utilities {
			class RoomInfoComponent {
				private:
                    rtype::client::utilities::StaticSprite _borderSprite;
                    std::unique_ptr<rtype::client::utilities::CustomText> _roomName;
                    std::unique_ptr<rtype::client::utilities::CustomText> _numberOfPlayer;
                    std::unique_ptr<rtype::client::utilities::CustomButton> _joinRoomBtn;

					/**
					 * @brief Set position of _roomName, _numberOfPlayer and _joinRoomBtn relatively to the postion of _borderSprite
					*/
                    void setObjectsPosition(void);

				public:
					/**
					 * @brief Default constructor (not used)
					*/
                    RoomInfoComponent();

					/**
					 * @brief Create a component that contains informations about rooms created by other players
					 *
					 * @param roomName The name of the room
					 * @param numberOfPlayer The number of player already connected in the room
					 * @param position Position of the component in the window
					*/
					RoomInfoComponent(std::string roomName, int numberOfPlayer, sf::Vector2f position);

					/**
					 * @brief Set the position of the component
					 *
					 * @param position Position in the screen of the component
					*/
					void setPosition(sf::Vector2f position);

					/**
					 * @brief Move the component in the window
					 *
					 * @param offset Offset as a sf::Vector2f
					*/
					void move(sf::Vector2f offset);

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
					 * @brief Return a reference of _joinRoomBtn component
					*/
                    std::unique_ptr<rtype::client::utilities::CustomButton> &getJoinRoomBtn(void);

					/**
					 * @brief Draw the component in the window
					*/
					void draw(rtype::engine::Engine* engine);
			};
		};
	};
}