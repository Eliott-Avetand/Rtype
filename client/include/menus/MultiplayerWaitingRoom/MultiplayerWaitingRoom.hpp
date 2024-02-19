
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
//  File:            MultiplayerWaitingRoom.hpp                                      //
//  Description:     The waiting room before the game launch                         //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <mutex>
#include <memory>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Engine.hpp"
#include "Errors.hpp"
#include "AGameState.hpp"
#include "BackgroundSpaceGenerator.hpp"
#include "CustomText.hpp"
#include "CustomButton.hpp"
#include "SlotInfoComponent.hpp"

/**
 * @brief Button callback that redirect player to MultiplayerRoomMenu
 *
 * @param engine GameEngine pointer that will be used to draw in the window
 * @param data Data that will be passed to callback
*/
void goBackToMultiplayerRoomMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data);

/**
 * @brief Button callback that redirect player to the loading screen
 *
 * @param engine GameEngine pointer that will be used to draw in the window
 * @param data Data that will be passed to callback
*/
void launchGame(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data);

namespace rtype {
    namespace client {
        class MultiplayerWaitingRoom : public rtype::scene::AGameState {
            private:
                static MultiplayerWaitingRoom *_waitingRoomInstance;
                static std::mutex _mutex;

                rtype::client::utilities::BackgroundSpaceGenerator _background;
                std::unique_ptr<rtype::client::utilities::CustomText> _sceneTitle;
                std::vector<rtype::client::utilities::SlotInfoComponent> _slots;
                std::unique_ptr<rtype::client::utilities::CustomButton> _exitRoom;
                std::unique_ptr<rtype::client::utilities::CustomButton> _launchGame;
                std::unique_ptr<rtype::client::utilities::CustomText> _roomPassword;

                int _actualFocusEventID;

                /**
                 * @brief Create the slots used to display the pseudo of players in the room
                 *
                 * @param engine GameEngine pointer that will be used to draw in the window
                */
                void createMultiplayerSlots(rtype::engine::Engine* engine);

                /**
                 * @brief Update the display of the room password in the menu
                 *
                 * @param engine GameEngine pointer that will be used to draw in the window
                 * @param password The room password
                */
                void updateDisplayOfRoomPassword(rtype::engine::Engine* engine, std::string password);

            protected:
                MultiplayerWaitingRoom() {}
                ~MultiplayerWaitingRoom() {}

            public:
                MultiplayerWaitingRoom(MultiplayerWaitingRoom &) = delete;
                void operator=(MultiplayerWaitingRoom const &) = delete;

                /**
                 * @brief Return the unique instance of the scene
                 *
                 * @details This function return the unique instance of the scene.
                 *          If the instance doesn't exist, the function create it.
                */
                static MultiplayerWaitingRoom *getInstance()
                {
                    std::lock_guard<std::mutex> lock(_mutex);

                    if (_waitingRoomInstance == nullptr)
                        _waitingRoomInstance = new MultiplayerWaitingRoom();
                    return _waitingRoomInstance;
                }

                static void releaseInstance()
                {
                    _waitingRoomInstance = nullptr;
                }

                void init(rtype::engine::Engine *engine);
                void cleanup();

                void pause();
                void resume();

                void networkSender(rtype::engine::Engine *engine);
                void networkReceiver(rtype::engine::Engine *engine);

                void processInput(sf::Event event, rtype::engine::Engine *engine);
                void update(rtype::engine::Engine *engine);
                void render(rtype::engine::Engine *engine);

                void updateObjectsPosition(sf::Vector2f deltaSize);

                std::vector<rtype::client::utilities::SlotInfoComponent> &getSlots(void);
        };
    }
}