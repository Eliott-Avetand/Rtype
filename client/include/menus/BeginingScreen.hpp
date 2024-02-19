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
//  File:            BeginingScreen.hpp                                              //
//  Description:     BeginingScreen Class Header                                     //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "Engine.hpp"
#include "AGameState.hpp"
#include "CustomText.hpp"
#include "AnimatedSprite.hpp"
#include "Errors.hpp"

#include <iostream>

#define LOUSTIK_SPRITESHEET "./client/assets/sprites/LoustikStudioSprite.png"
#define BEGINING_SOUND "./client/assets/musics/beginingScreenSound.ogg"

namespace rtype {
    namespace client {
        class BeginingScreen : public rtype::scene::AGameState {
        private:
            static BeginingScreen* _instance;
            static std::mutex _mutex;

            sf::Clock _sceneClock;
            bool _animationIsFinish;

            sf::RectangleShape _rectangleShape;
            std::unique_ptr<rtype::client::utilities::CustomText> _loadingText;
            std::unique_ptr<rtype::client::utilities::AnimatedSprite> _loustikSprite;
            sf::Clock _loadingTextClock;

            rtype::engine::utilities::CustomSound _sound;

            bool minimalLoading(void);

            void createLoadingText(rtype::engine::Engine* engine);

        protected:
            BeginingScreen() : _sound(BEGINING_SOUND) {};
            ~BeginingScreen() {};

        public:
            BeginingScreen(BeginingScreen&) = delete;
            void operator=(BeginingScreen const&) = delete;

            static BeginingScreen* getInstance()
            {
                std::lock_guard<std::mutex> lock(_mutex);

                if (_instance == nullptr)
                    _instance = new BeginingScreen();
                return _instance;
            }

            static void releaseInstance()
            {
                _instance = nullptr;
            }

            void init(rtype::engine::Engine* engine);
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