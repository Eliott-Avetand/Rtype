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
#include "CustomText.hpp"
#include "BeginingScreen.hpp"

#include <SFML/Graphics.hpp>

#include <mutex>
#include <vector>
#include <utility>
#include <string>

#define LOGO_TEXTURE "./client/assets/sprites/game_logo.png"
#define CPP_TEXTURE "./client/assets/sprites/Credits/c++.png"
#define BOOST_TEXTURE "./client/assets/sprites/Credits/Boost.png"
#define SFML_TEXTURE "./client/assets/sprites/Credits/SFML.png"
#define DISCORD_TEXTURE "./client/assets/sprites/Credits/discord.png"
#define GITHUB_TEXTURE "./client/assets/sprites/Credits/Github.jpg"
#define POWERPOINT_TEXTURE "./client/assets/sprites/Credits/PowerPoint.png"
#define VISUALSTUDIO_TEXTURE "./client/assets/sprites/Credits/VS.png"
#define TRELLO_TEXTURE "./client/assets/sprites/Credits/Trello.png"


#define TITLE_SIZE 100
#define TEXT_SIZE 50

namespace rtype {
    namespace client {
        class Credits : public rtype::scene::AGameState {
            private:
                static Credits *_creditsInstance;
                static std::mutex _mutex;

                int _speed;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _programmers;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _producer;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _studio;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _artDirector;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _music;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _graphicDesigner;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _animator;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _gameDesigner;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _projectDirector;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _technicalDirector;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _networkDesigner;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _serverConceptor;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _engineConceptor;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _levelDesigner;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _documentation;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _tester;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _traductor;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _mentionsHonorables;

                std::vector<std::unique_ptr<rtype::client::utilities::StaticSprite>> _logo;

                rtype::client::utilities::BackgroundSpaceGenerator _background;

                sf::RectangleShape _backgroundBlack;
                sf::Clock _backgroundClock;

            protected:
                Credits() {}
                ~Credits() {}

            public:
                Credits(Credits &) = delete;
                void operator=(Credits const &) = delete;

                static Credits* getInstance()
                {
                    std::lock_guard<std::mutex> lock(_mutex);

                    if (_creditsInstance == nullptr)
                        _creditsInstance = new Credits();
                    return _creditsInstance;
                }

                static void releaseInstance()
                {
                    _creditsInstance = nullptr;
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