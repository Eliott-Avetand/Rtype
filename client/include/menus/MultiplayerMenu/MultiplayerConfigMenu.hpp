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
//  File:            MultiplayerConfigMenu.hpp                                       //
//  Description:     Scene where player is asked to fill is pseudo and server ip     //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <mutex>
#include <memory>
#include <iostream>
#include <regex>

#include <SFML/Graphics.hpp>

#include "Engine.hpp"
#include "Errors.hpp"
#include "AGameState.hpp"
#include "CustomText.hpp"
#include "CustomButton.hpp"
#include "InputBox.hpp"
#include "BackgroundSpaceGenerator.hpp"

/**
 * @brief Button callback that redirect player to MainMenu
 *
 * @param engine GameEngine pointer that will be used to draw in the window
 * @param data Data that will be passed to callback
*/
void goToMainMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data);

/**
 * @brief Button callback that redirect player to MultiplayerRoomMenu
 *
 * @param engine GameEngine pointer that will be used to draw in the window
 * @param data Data that will be passed to callback
*/
void goToMultiplayerRoom(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data);

namespace rtype {
    namespace client {
        class MultiplayerConfigMenu : public rtype::scene::AGameState {
            private:
                static MultiplayerConfigMenu *_configMenuInstance;
                static std::mutex _mutex;

                rtype::client::utilities::BackgroundSpaceGenerator _background;
                std::unique_ptr<rtype::client::utilities::CustomText> _sceneTitle;
                std::unique_ptr<rtype::client::utilities::CustomButton> _goBackBtn;
                std::unique_ptr<rtype::client::utilities::CustomButton> _confirmBtn;

                std::unique_ptr<rtype::client::utilities::CustomText> _pseudoLabel;
                std::unique_ptr<rtype::client::utilities::InputBox> _pseudoInput;
                std::unique_ptr<rtype::client::utilities::CustomText> _ipLabel;
                std::unique_ptr<rtype::client::utilities::InputBox> _ipInput;

                int _actualFocusEventID;

            protected:
                MultiplayerConfigMenu() {}
                ~MultiplayerConfigMenu() {}

            public:
                MultiplayerConfigMenu(MultiplayerConfigMenu &) = delete;
                void operator=(MultiplayerConfigMenu const &) = delete;

                /**
                 * @brief Return the unique instance of the scene
                 *
                 * @details This function return the unique instance of the scene.
                 *          If the instance doesn't exist, the function create it.
                */
                static MultiplayerConfigMenu* getInstance()
                {
                    std::lock_guard<std::mutex> lock(_mutex);

                    if (_configMenuInstance == nullptr)
                        _configMenuInstance = new MultiplayerConfigMenu();
                    return _configMenuInstance;
                }

                static void releaseInstance()
                {
                    _configMenuInstance = nullptr;
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
                 * @brief Return a reference of _pseudoInput component stored in the scene
                */
                std::unique_ptr<rtype::client::utilities::InputBox> &getPseudoInput(void);

                /**
                 * @brief Return a reference of _ipInput component stored in the scene
                */
                std::unique_ptr<rtype::client::utilities::InputBox> &getIpInput(void);
        };
    }
}