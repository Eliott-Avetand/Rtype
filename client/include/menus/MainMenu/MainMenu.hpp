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
//  File:            MainMenu.hpp                                                    //
//  Description:     MainMenu Class Header                                           //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include "AGameState.hpp"
#include "Engine.hpp"
#include "StaticSprite.hpp"
#include "CustomButton.hpp"
#include "BackgroundSpaceGenerator.hpp"
#include "RandomNumbers.hpp"

#include <SFML/Graphics.hpp>

#include <mutex>
#include <vector>
#include <utility>
#include <string>

#define MAINMENU_MUSIC_PATH "./client/assets/musics/main-menu_theme.ogg"

#define LOGO_TEXTURE "./client/assets/sprites/game_logo.png"

/**
 * @brief This is a callback, it quit game when call
 * @see This method is call by quit game button
 * 
 * @param engine    A reference of the engine given by the engine himself
 * @param data      All datas needed for the functions
 */
void quitGame(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

/**
 * @brief This is a callback, it launch game when call
 * @see This method is call by play button
 * 
 * @param engine    A reference of the engine given by the engine himself
 * @param data      All datas needed for the functions
 */
void playGame(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

/**
 * @brief This is a callback, it display the options menu when call
 * @see This method is call by options button
 * 
 * @param engine    A reference of the engine given by the engine himself
 * @param data      All datas needed for the functions
 */
void goToOptionsMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

/**
 * @brief This is a callback, it display the multiplayer menu when call
 * @see This method is call by multiplayer button
 * 
 * @param engine    A reference of the engine given by the engine himself
 * @param data      All datas needed for the functions
 */
void goToMultiplayer(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

namespace rtype {
    namespace client {
        class MainMenu : public rtype::scene::AGameState {
            private:
                static MainMenu *_mainMenuInstance;
                static std::mutex _mutex;

                rtype::client::utilities::StaticSprite _titleLogo;
                std::vector<rtype::client::utilities::CustomButton> _allButtons;
                rtype::client::utilities::BackgroundSpaceGenerator _background;

                bool _hasDestroyedAsteroids;
                int _actualFocusEventID;

                /**
                 * @brief This is a method which initialize title
                 * @see This method is automatically called when scene is init
                 * 
                 * @param engine    A reference of the engine given by the engine himself
                 */
                void createTitleLogo(rtype::engine::Engine* engine);

                /**
                 * @brief This is a method which display title in the main menu
                 * @see This method is automatically called when scene is render
                 * 
                 * @param engine    A reference of the engine given by the engine himself
                 */
                void displayTitleLogo(rtype::engine::Engine* engine);

                /**
                 * @brief This is a method which initialize all buttons for the scene
                 * @see This method is automatically called when scene is init
                 * 
                 * @param engine    A reference of the engine given by the engine himself
                 */
                void createButtons(rtype::engine::Engine* engine);

                /**
                 * @brief Display buttons of the scene
                 *
                 * @param engine    A reference of the engine given by the engine himself
                 */
                void displayButtons(rtype::engine::Engine* engine);

                /**
                 * @brief Load music of the main menu
                 *
                 * @param engine A reference of the engine given by the engine himself
                 */
                void loadMainMenuMusic(rtype::engine::Engine* engine);

            protected:
                MainMenu() : _titleLogo(LOGO_TEXTURE, sf::Vector2f(0.0, 0.0)) {}
                ~MainMenu() {}

            public:
                MainMenu(MainMenu &) = delete;
                void operator=(MainMenu const &) = delete;

                static MainMenu* getInstance()
                {
                    std::lock_guard<std::mutex> lock(_mutex);

                    if (_mainMenuInstance == nullptr)
                        _mainMenuInstance = new MainMenu();
                    return _mainMenuInstance;
                }

                static void releaseInstance()
                {
                    _mainMenuInstance = nullptr;
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
                void renderLevel(rtype::engine::Engine* engine);
                void updateObjectsPosition(sf::Vector2f deltaSize);
        };
    }
}