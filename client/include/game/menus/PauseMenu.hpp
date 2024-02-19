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
//  File:            Exemple.hpp                                                     //
//  Description:     A template to copy / paste for the other scene                  //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <mutex>
#include "APlayerMovementComponent.hpp"
#include "AGameState.hpp"
#include "ACommand.hpp"
#include "Engine.hpp"
#include "Monster.hpp"
#include <SFML/Graphics.hpp>

#include "CustomText.hpp"
#include "CustomButton.hpp"

#include "Level_1.hpp"
#include "PauseMenuOptions.hpp"

/**
 * @brief This is a callback, it go to Main menu
 * @see This method is call by quit game button
 *
 * @param engine    A reference of the engine given by the engine himself
 * @param data      All datas needed for the functions
 */
void returnToMainMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

/**
 * @brief This is a callback, it resume game
 * @see This method is call by resume button
 *
 * @param engine    A reference of the engine given by the engine himself
 * @param data      All datas needed for the functions
 */
void resumeGame(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

/**
 * @brief This is a callback, it go to pause options menu
 * @see This method is call by back button
 *
 * @param engine    A reference of the engine given by the engine himself
 * @param data      All datas needed for the functions
 */
void goToOptionsGameMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

namespace rtype {
    namespace client {
        class PauseMenu : public rtype::scene::AGameState {
            private:
                static PauseMenu* _instance;
                static std::mutex _mutex;

                rtype::client::Level_1 *_levelInstance;

                sf::RectangleShape _background;
                rtype::client::utilities::CustomText _titleText;
                std::vector<rtype::client::utilities::CustomButton> _allButtons;

                int _actualFocusEventID;

                void createBackground(rtype::engine::Engine* engine);
                void createTitle(rtype::engine::Engine* engine);
                void createButtons(rtype::engine::Engine* engine);

            protected:
                PauseMenu() {}
                ~PauseMenu() {}

            public:
                PauseMenu(PauseMenu &) = delete;
                void operator=(PauseMenu const &) = delete;

                static PauseMenu* getInstance()
                {
                    std::lock_guard<std::mutex> lock(_mutex);

                    if (_instance == nullptr)
                        _instance = new PauseMenu();
                    return _instance;
                }

                static void releaseInstance()
                {
                    if (_instance != nullptr) {
                        _instance = nullptr;
                    }
                }

                void init(rtype::engine::Engine* engine);
                void initInput();
                void loadEnemies();
                void cleanup();

                void pause();
                void resume();

                void networkSender(rtype::engine::Engine* engine);
                void networkReceiver(rtype::engine::Engine* engine);

                void processInput(sf::Event event, rtype::engine::Engine* engine);
                void update(rtype::engine::Engine* engine);
                void render(rtype::engine::Engine* engine);

                void updateObjectsPosition(sf::Vector2f deltaSize);
        };
    }
}