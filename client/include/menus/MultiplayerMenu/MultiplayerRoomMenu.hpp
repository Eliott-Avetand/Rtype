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
//  File:            MultiplayerRoomMenu.hpp                                         //
//  Description:     Scene that list all room created in the server                  //
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
#include "CustomText.hpp"
#include "RoomInfoComponent.hpp"
#include "ConnectionPopup.hpp"
#include "BackgroundSpaceGenerator.hpp"

/**
 * @brief Button callback that redirect player to MultiplayerConfigMenu
 *
 * @param engine GameEngine pointer that will be used to draw in the window
 * @param data Data that will be passed to callback
*/
void goToMultiplayerConfig(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data);

/**
 * @brief Button callback that allow player to create a room
 *
 * @param engine GameEngine pointer that will be used to draw in the window
 * @param data Data that will be passed to callback
*/
void createRoom(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data);

/**
 * @brief Button callback that refresh the room list
 * 
 * @param engine GameEngine pointer that will be used to draw in the window
 * @param data Data that will be passed to callback
 */
void refreshRoom(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data);

namespace rtype {
    namespace client {
        class MultiplayerRoomMenu : public rtype::scene::AGameState {
            private:
                static MultiplayerRoomMenu *_roomMenuInstance;
                static std::mutex _mutex;

                rtype::client::utilities::BackgroundSpaceGenerator _background;
                std::unique_ptr<rtype::client::utilities::CustomText> _sceneTitle;
                std::vector<rtype::client::utilities::RoomInfoComponent> _rooms;
                std::unique_ptr<rtype::client::utilities::CustomButton> _goBackBtn;
                std::unique_ptr<rtype::client::utilities::CustomButton> _createRoomBtn;
                std::unique_ptr<rtype::client::utilities::CustomButton> _refreshRoomBtn;
                std::unique_ptr<rtype::client::utilities::ConnectionPopup> _popup;

                int _actualFocusEventID;

                bool _isUpdate;

                /**
                 * @brief Create a bunch of RoomInfoComponent and push them in a vector
                 *
                 * @param engine GameEngine pointer that will be used to draw in the window
                */
                void createMultiplayerRooms(rtype::engine::Engine* engine);

                void linkMultiplayerButtons(void);

            protected:
                MultiplayerRoomMenu() {}
                ~MultiplayerRoomMenu() {}

            public:
                MultiplayerRoomMenu(MultiplayerRoomMenu &) = delete;
                void operator=(MultiplayerRoomMenu const &) = delete;

                /**
                 * @brief Return the unique instance of the scene
                 *
                 * @details This function return the unique instance of the scene.
                 *          If the instance doesn't exist, the function create it.
                */
                static MultiplayerRoomMenu *getInstance()
                {
                    std::lock_guard<std::mutex> lock(_mutex);

                    if (_roomMenuInstance == nullptr)
                        _roomMenuInstance = new MultiplayerRoomMenu();
                    return _roomMenuInstance;
                }

                static void releaseInstance()
                {
                    _roomMenuInstance = nullptr;
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

                /**
                 * @brief Return a reference of _popup component stored in the scene
                */
                std::unique_ptr<rtype::client::utilities::ConnectionPopup> &getPopup(void);

                /**
                 * @brief Set is the scene is Updated or not
                 * 
                 * @param bool  True if the scene is updated, false otherwise
                 */
                void setUpdate(bool isUpdated);
        };
    }
}