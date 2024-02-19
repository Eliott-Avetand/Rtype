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
//  File:            GameEngine.hpp                                                  //
//  Description:     The game engine header                                          //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <mutex>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Time.hpp"
#include "Options.hpp"
#include "GameDatas.hpp"

#ifdef CLIENT
    #include "../../client/include/UDPClient.hpp"
#endif


#include "DataStruct.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define ICON_TEXTURE "./client/assets/sprites/icon.png"
#define MISSING_TEXTURE "./client/assets/sprites/missing_texture.png"

class UDPServer;

namespace rtype {
    namespace scene {
        class AGameState;
    };

    namespace engine {
        class Engine {
            private:
                int _key;
                sf::Vector2i _mousePoint;
                sf::Time _keepTime;
                sf::Clock _clock;
                sf::View view;
                double _lag;
                bool _shouldClose;
                bool _isFullscreen;
                std::vector<rtype::scene::AGameState *> _states;

                static Engine *_gameEngineInstance;
                static std::mutex _mutex;
                #ifdef CLIENT
                    UDPClient *_client;
                #endif
                #ifdef SERVER
                    UDPServer *_server;
                #endif

            public:
                rtype::settings::Options options;
                rtype::engine::GameDatas gameDatas;
                sf::RenderWindow window;
                sf::Event event;

                void init();
                void cleanup();

                void changeState(rtype::scene::AGameState *state);
                void pushState(rtype::scene::AGameState *state);
                void popState();

                void processInput(sf::Event event, rtype::engine::Engine *engine);
                void update();
                void render();
                void gameLoop();

                sf::Vector2i getMouseCoordinates();
                void shouldClose();

                void processResizeEvent(sf::Event event);
                bool isFullscreen(void);
                void toggleFullscreen(void);
                void setWindowIcon(void);

                static Engine *getInstance()
                {
                    std::lock_guard<std::mutex> lock(_mutex);

                    if (_gameEngineInstance == nullptr)
                        _gameEngineInstance = new Engine();
                    return _gameEngineInstance;
                }

                #ifdef CLIENT
                    UDPClient *getClient(void) { return _client; }
                    void setClient(UDPClient *client) { this->_client = client; }
                #endif
                #ifdef SERVER
                    UDPServer *getServer(void) { return _server; }
                    void setServer(UDPServer *server) { this->_server = server; }
                #endif
        };
    };
};