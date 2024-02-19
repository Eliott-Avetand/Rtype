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
//  File:            GameOverScreen.hpp                                              //
//  Description:     GameOverScreen Header File                                      //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <mutex>
#include "AGameState.hpp"
#include "Engine.hpp"
#include <SFML/Graphics.hpp>

#include "CustomText.hpp"
#include "CustomButton.hpp"

#include "PauseMenu.hpp"

#include "Level_1.hpp"

namespace rtype {
    namespace client {
        class GameOverScreen : public rtype::scene::AGameState {
        private:
            static GameOverScreen* _instance;
            static std::mutex _mutex;

            rtype::client::Level_1* _levelInstance;

            std::unique_ptr<rtype::client::utilities::CustomText> _gameOverText;
            sf::RectangleShape _gameOverBar;

            std::unique_ptr<rtype::client::utilities::CustomText> _scoreSectionText;
            std::unique_ptr<rtype::client::utilities::CustomText> _scoreText;

            sf::RectangleShape _background;
            std::unique_ptr<rtype::client::utilities::CustomButton> _goToMenu;

            int _actualFocusEventID;

            void createBackground(rtype::engine::Engine* engine);
            void createButtons(rtype::engine::Engine* engine);
            void createTexts(rtype::engine::Engine* engine);

        protected:
            GameOverScreen() {}
            ~GameOverScreen() {}

        public:
            GameOverScreen(GameOverScreen&) = delete;
            void operator=(GameOverScreen const&) = delete;

            static GameOverScreen* getInstance()
            {
                std::lock_guard<std::mutex> lock(_mutex);

                if (_instance == nullptr)
                    _instance = new GameOverScreen();
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