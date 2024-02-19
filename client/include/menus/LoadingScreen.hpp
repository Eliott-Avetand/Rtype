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
//  File:            LoadingScreen.hpp                                               //
//  Description:     LoadingScreen Class Header                                      //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "Engine.hpp"
#include "AGameState.hpp"
#include "Level_1.hpp"
#include "CustomText.hpp"
#include "Errors.hpp"

#include <iostream>

namespace rtype {
    namespace client {
        class LoadingScreen : public rtype::scene::AGameState {
            private:
                static LoadingScreen *_loadingInstance;
                static std::mutex _mutex;

                sf::Clock _sceneClock;

                sf::RectangleShape _rectangleShape;
                std::unique_ptr<rtype::client::utilities::CustomText> _levelTitle;
                std::unique_ptr<rtype::client::utilities::CustomText> _loadingText;
                sf::Clock _loadingTextClock;


                /**
                 * @brief This is a method which initialize loading text
                 * @see This method is automatically called when scene is init
                 *
                 * @param engine    A reference of the engine given by the engine himself
                 */
                void createLoadingText(rtype::engine::Engine* engine);

                /**
                 * @brief This is a method which display loading text
                 * @see This method is automatically called when scene is render
                 *
                 * @param engine    A reference of the engine given by the engine himself
                 */
                void displayLoadingText(rtype::engine::Engine* engine);

                /**
                 * @brief This is a method which initialize title
                 * @see This method is automatically called when scene is init
                 *
                 * @param engine    A reference of the engine given by the engine himself
                 */
                void createTitleLevel(rtype::engine::Engine* engine);

                /**
                 * @brief This is a method which display title
                 * @see This method is automatically called when scene is render
                 *
                 * @param engine    A reference of the engine given by the engine himself
                 */
                void displayTitleLevel(rtype::engine::Engine* engine);

                /**
                 * @brief This is a method will check if the time is up to 5 seconds
                 * @see This method is automatically called when scene is update
                 *
                 * @returns true if is load, false if is not
                 */
                bool minimalLoading(void);

            protected:
                LoadingScreen() {};
                ~LoadingScreen() {};

            public:
                LoadingScreen(LoadingScreen&) = delete;
                void operator=(LoadingScreen const&) = delete;

                static LoadingScreen *getInstance()
                {
                    std::lock_guard<std::mutex> lock(_mutex);

                    if (_loadingInstance == nullptr)
                        _loadingInstance = new LoadingScreen();
                    return _loadingInstance;
                }

                static void releaseInstance()
                {
                    _loadingInstance = nullptr;
                }

                void init(rtype::engine::Engine* engine);
                void cleanup();

                void pause();
                void resume();

                void networkSender(rtype::engine::Engine *engine);
                void networkReceiver(rtype::engine::Engine *engine);

                void processInput(sf::Event event, rtype::engine::Engine* engine);
                void update(rtype::engine::Engine* engine);
                void render(rtype::engine::Engine* engine);

                void updateObjectsPosition(sf::Vector2f deltaSize);
        };
    };
};