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
//  File:            TransitionScreen.hpp                                            //
//  Description:     TransitionScreen Header Class                                   //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "Engine.hpp"
#include "LoadingScreen.hpp"
#include "Level_1.hpp"

namespace rtype {
	namespace client {
		class TransitionScreen : public rtype::scene::AGameState {
        private:
            static TransitionScreen* _instance;
            static std::mutex _mutex;

            rtype::client::Level_1* _levelInstance;

            sf::RectangleShape _background;
            sf::Clock _backgroundClock;

        protected:
            TransitionScreen() {}
            ~TransitionScreen() {}

        public:
            TransitionScreen(TransitionScreen&) = delete;
            void operator=(TransitionScreen const&) = delete;

            static TransitionScreen* getInstance()
            {
                std::lock_guard<std::mutex> lock(_mutex);

                if (_instance == nullptr)
                    _instance = new TransitionScreen();
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
	};
};
