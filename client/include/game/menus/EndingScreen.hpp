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
//  File:            EndingScreen.hpp                                                //
//  Description:     EndingScreen Header Class                                   //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "Engine.hpp"
#include "MainMenu.hpp"
#include "Level_1.hpp"
#include <cmath>

#include "CustomText.hpp"
#include "CustomButton.hpp"

#define CONGRATULATION_MUSIC "./client/assets/musics/levels_musics/endingMusic.ogg"

namespace rtype {
	namespace client {
		class EndingScreen : public rtype::scene::AGameState {
        private:
            static EndingScreen* _instance;
            static std::mutex _mutex;

            rtype::client::Level_1* _levelInstance;

            std::unique_ptr<rtype::actor::Actor> _player;

            std::unique_ptr<rtype::client::utilities::CustomText> _congratulationText;
            sf::RectangleShape _congratulationBar;

            std::unique_ptr<rtype::client::utilities::CustomText> _scoreSectionText;
            std::unique_ptr<rtype::client::utilities::CustomText> _scoreText;

            std::unique_ptr<rtype::client::utilities::CustomButton> _goToMenu;

            sf::RectangleShape _background;

            sf::Clock _startedTime;

            bool _isTextDisplayed;

            bool _isBackgroundDisplayed;
            sf::Vector2f _savePlayerPos;

            int _actualFocusEventID;

            void createButtons(rtype::engine::Engine* engine);
            void createTexts(rtype::engine::Engine* engine);
           
        protected:
            EndingScreen() {}
            ~EndingScreen() {}

        public:
            EndingScreen(EndingScreen&) = delete;
            void operator=(EndingScreen const&) = delete;

            static EndingScreen* getInstance()
            {
                std::lock_guard<std::mutex> lock(_mutex);

                if (_instance == nullptr)
                    _instance = new EndingScreen();
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

            void initPlayerPosition(sf::Vector2f position);

            void pause();
            void resume();

            void networkSender(rtype::engine::Engine* engine);
            void networkReceiver(rtype::engine::Engine* engine);

            void processInput(sf::Event event, rtype::engine::Engine* engine);
            void update(rtype::engine::Engine* engine);
            void render(rtype::engine::Engine* engine);

            void updateObjectsPosition(sf::Vector2f deltaSize);
		};
	};
};
