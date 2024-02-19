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
#include "AGameState.hpp"
#include "ACommand.hpp"
#include "Engine.hpp"
#include "Monster.hpp"
#include <SFML/Graphics.hpp>

namespace rtype {
    namespace exemple {
        class Exemple : public rtype::scene::AGameState {
            private:
                static Exemple *_exempleInstance;
                static std::mutex _mutex;

                std::vector<std::unique_ptr<rtype::actor::Breed>> _breeds;
                std::vector<std::unique_ptr<rtype::actor::Monster>> _enemies;
                std::unique_ptr<rtype::actor::Actor> _player;

            protected:
                Exemple() {}
                ~Exemple() {}

            public:
                Exemple(Exemple &) = delete;
                void operator=(Exemple const &) = delete;

                static Exemple *getInstance()
                {
                    std::lock_guard<std::mutex> lock(_mutex);

                    if (_exempleInstance == nullptr)
                        _exempleInstance = new Exemple();
                    return _exempleInstance;
                }

                void init(rtype::engine::Engine *engine);
                void initInput();
                void loadEnemies();
                void cleanup();

                void pause();
                void resume();

                void networkSender(rtype::engine::Engine *engine);
                void networkReceiver(rtype::engine::Engine *engine);

                void processInput(sf::Event event, rtype::engine::Engine *engine);
                void update(rtype::engine::Engine *engine, double deltaTime);
                void render(rtype::engine::Engine *engine);
        };
    }
}