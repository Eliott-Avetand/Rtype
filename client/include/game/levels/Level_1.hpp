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
//  File:            Level_1.hpp                                                     //
//  Description:     Level_1 Class Header                                            //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include <utility>
#include <string>

#include "AGameState.hpp"
#include "APlayerMovementComponent.hpp"
#include "Engine.hpp"
#include "Errors.hpp"

#include "Player.hpp"
#include "StaticSprite.hpp"
#include "Breed.hpp"
#include "BossDobkeratops.hpp"
#include "GameUI.hpp"
#include "BackgroundSpaceGenerator.hpp"

#include "APowerUpItem.hpp"
#include "TripleShootItem.hpp"

#include "MoveUpCommand.hpp"
#include "MoveDownCommand.hpp"
#include "MoveLeftCommand.hpp"
#include "MoveRightCommand.hpp"
#include "ShootCommand.hpp"

#include <SFML/Graphics.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define LEVEL1_MUSIC_PATH "./client/assets/musics/levels_musics/level_1.ogg"

const std::map < std::string, std::string > planetFilePath = {
    {"gas_planet", GAS_PLANET_TEXTURE},
    {"big_star", BIG_STAR_TEXTURE},
    {"ice_planet", ICE_WORLD_TEXTURE},
    {"black_hole", BLACK_HOLE_TEXTURE},
    {"galaxy", GALAXY_TEXTURE}
};

namespace rtype {
    namespace client {
        class Level_1 : public rtype::scene::AGameState, public rtype::scene::APlayerMovementComponent {
            private:
                static Level_1* _level1instance;
                static std::mutex _mutex;

                rtype::client::GameUI _gameUI;
                std::unique_ptr<rtype::client::utilities::BackgroundSpaceGenerator> _background;

                int _breedIndex;
                std::vector<std::unique_ptr<rtype::actor::Actor>> _players;
                std::vector<std::unique_ptr<rtype::actor::Actor>> _enemies;
                std::vector<std::unique_ptr<rtype::actor::Breed>> _breeds;
                std::unique_ptr<rtype::actor::BossDobkeratops> _boss;
                std::vector<std::unique_ptr<rtype::actor::Asteroid>> _asteroids;

                std::vector<std::unique_ptr<rtype::powerup::APowerUpItem>> _powerUpItems;

                std::vector<sf::Keyboard::Key> _keysPressed;
                std::vector<int> _joystickButtonsPressed;
                std::vector<std::pair<sf::Joystick::Axis, int>> _joystickAxisMoved;

                bool _sessionIsClosed;

                bool _threadCreated;
                std::thread *_networkThread;
                bool _threadActive;

                int _playerID;
                int _playerNumber;
                int _breedNumber;
                bool _isBoss;

                int _actualLevel;

                std::unique_ptr<rtype::client::utilities::CustomText> _pingText;
                bool _isPingDisplayed;

                sf::Clock _asteroidsClock;
                float _asteroidsTime;

                LevelState _levelStatus;
                bool _levelFinish;

                sf::Clock _timer;

                bool checkNextGameLevel(rtype::engine::Engine *engine);

                void initBackground(rtype::engine::Engine* engine);
                void initMusic(rtype::engine::Engine* engine);
                void initBoss(rtype::engine::Engine* engine);

                void updateOnline(rtype::engine::Engine* engine);
                void checkOfflineGameStatus(rtype::engine::Engine* engine);
                void checkOnlineGameStatus(rtype::engine::Engine* engine);

                void networkHandler(rtype::engine::Engine* engine);
                void threadNetworkSender(rtype::engine::Engine* engine);
                void threadNetworkReceiver(rtype::engine::Engine* engine);

            protected:
                Level_1() {}
                ~Level_1() {}

            public:
                Level_1(Level_1 &) = delete;
                void operator=(Level_1 const &) = delete;

                static Level_1* getInstance()
                {
                    std::lock_guard<std::mutex> lock(_mutex);

                    if (_level1instance == nullptr)
                        _level1instance = new Level_1();
                    return _level1instance;
                }

                static void releaseInstance()
                {
                    _level1instance = nullptr;
                }

                void init(rtype::engine::Engine *engine);
                void cleanup();

                /**
                 * @brief This is a method initialize all inputs for the scene
                 * @see This method is automatically called when scene is init
                 */
                void initInput();
                void loadEnemies(rtype::engine::Engine *engine);
                void loadBoss();

                void pause();
                void resume();

                void networkSender(rtype::engine::Engine *engine);
                void networkReceiver(rtype::engine::Engine *engine);

                void processInput(sf::Event event, rtype::engine::Engine *engine);
                void processPlayerInput(rtype::engine::Engine *engine);
                void update(rtype::engine::Engine *engine);
                void render(rtype::engine::Engine *engine);
                void renderLevel(rtype::engine::Engine* engine);

                void updateObjectsPosition(sf::Vector2f deltaSize);
        };
    }
}