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
//  File:            MainMenuEsaterEgg.hpp                                           //
//  Description:     File for the MainMenuEsaterEgg Header                           //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <mutex>
#include "Engine.hpp"
#include "AGameState.hpp"
#include "CustomText.hpp"
#include "CustomButton.hpp"
#include "StaticSprite.hpp"
#include "BackgroundSpaceGenerator.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

#define LOGO_TEXTURE "./client/assets/sprites/game_logo.png"

/**
 * @brief This is a callback, it return to main menu scene when call
 * @see This method is call by back to main menu button
 * 
 * @param engine    A reference of the engine given by the engine himself
 * @param data      All datas needed for the functions
 */
void backToMainMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

namespace rtype {
    namespace client {
        class MainMenuEasterEgg : public rtype::scene::AGameState {
        private:
            enum menuState {
                MENU_DESTROY,
                EASTEREGG_LOAD,
                EASTEREGG,
                EASTEREGG_END,
            };

            static MainMenuEasterEgg *_mainMenuEasterEggInstance;
            static std::mutex _mutex;

            std::map<rtype::client::MainMenuEasterEgg::menuState, void (rtype::client::MainMenuEasterEgg::*)(rtype::engine::Engine* engine)> _displayFunctionsMap;

            /**
             * @brief This is a method which display all the texts of the last menu of the easter egg
             * @see This method is automatically called when scene is render
             * 
             * @param engine    A reference of the engine given by the engine himself
             */
            void displayMenuDestroy(rtype::engine::Engine* engine);

            /**
             * @brief This is a method which display texts for the loading of the easter egg
             * @see This method is automatically called when scene is render
             * 
             * @param engine    A reference of the engine given by the engine himself
             */
            void displayEasterEggLoad(rtype::engine::Engine* engine);
            
            /**
             * @brief This is a method which display texts for the main scene of easter egg
             * @see This method is automatically called when scene is render
             * 
             * @param engine    A reference of the engine given by the engine himself
             */
            void displayEasterEgg(rtype::engine::Engine* engine);

            /**
             * @brief This is a method which display texts at the end of the easter egg
             * @see This method is automatically called when scene is render
             * 
             * @param engine    A reference of the engine given by the engine himself
             */
            void displayEasterEggEnd(rtype::engine::Engine* engine);

            std::map<rtype::client::MainMenuEasterEgg::menuState, void (rtype::client::MainMenuEasterEgg::*)(rtype::engine::Engine* engine)> _updateFunctionsMap;

            /**
             * @brief This is a method which update the fake main menu scene
             * @see This method is automatically called when scene is update
             * 
             * @param engine    A reference of the engine given by the engine himself
             * @param deltaTime delta time of the game
             */
            void updateMenuDestroy(rtype::engine::Engine* engine);
            
            /**
             * @brief This is a method which update the loading of the easter egg
             * @see This method is automatically called when scene is update
             * 
             * @param engine    A reference of the engine given by the engine himself
             * @param deltaTime delta time of the game
             */
            void updateEasterEggLoad(rtype::engine::Engine* engine);
            
            /**
             * @brief This is a method which update the main scene of the easter egg
             * @see This method is automatically called when scene is update
             * 
             * @param engine    A reference of the engine given by the engine himself
             * @param deltaTime delta time of the game
             */
            void updateEasterEgg(rtype::engine::Engine* engine);
            
            /**
             * @brief This is a method which update the end of the easter egg
             * @see This method is automatically called when scene is render
             * 
             * @param engine    A reference of the engine given by the engine himself
             * @param deltaTime delta time of the game
             */
            void updateEasterEggEnd(rtype::engine::Engine* engine);

            std::map<rtype::client::MainMenuEasterEgg::menuState, void (rtype::client::MainMenuEasterEgg::*)(sf::Event event, rtype::engine::Engine* engine)> _processInputFunctionsMap;

            /**
             * @brief This is a method handle inputs in the beginning of the easter egg
             * @see This method is automatically called when scene is render
             * 
             * @param event     All events gives of the main loop
             * @param engine    A reference of the engine given by the engine himself
             */
            void processInputMenuDestroy(sf::Event event, rtype::engine::Engine* engine);
            
            /**
             * @brief This is a method handle inputs in the loading of the easter egg
             * @see This method is automatically called when scene is render
             * 
             * @param event     All events gives of the main loop
             * @param engine    A reference of the engine given by the engine himself
             */
            void processInputEasterEggLoad(sf::Event event, rtype::engine::Engine* engine);
            
            /**
             * @brief This is a method handle inputs in the easter egg himself
             * @see This method is automatically called when scene is render
             * 
             * @param event     All events gives of the main loop
             * @param engine    A reference of the engine given by the engine himself
             */
            void processInputEasterEgg(sf::Event event, rtype::engine::Engine* engine);
            
            /**
             * @brief This is a method handle inputs at the end of the easter egg
             * @see This method is automatically called when scene is render
             * 
             * @param event     All events gives of the main loop
             * @param engine    A reference of the engine given by the engine himself
             */
            void processInputEasterEggEnd(sf::Event event, rtype::engine::Engine* engine);

            rtype::client::MainMenuEasterEgg::menuState _state;

            rtype::client::utilities::BackgroundSpaceGenerator _background;

            sf::Clock _sceneClock;
            float _timeLeft;

            // Methods for Menu Destroy State

            rtype::client::utilities::StaticSprite _titleLogo;
            std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _allBeginTexts;

            bool _isMenuDisplayed;
            int _nbrDisplayed;

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
             * @brief This is a method which initialize all texts for the scene
             * @see This method is automatically called when scene is init
             * 
             * @param engine    A reference of the engine given by the engine himself
             */
            void createMainMenuTexts(rtype::engine::Engine* engine);

            /**
             * @brief This is a method which display all text of the main menu at beginning
             * @see This method is automatically called when scene is render
             * 
             * @param engine    A reference of the engine given by the engine himself
             */
            void displayMainMenuTexts(rtype::engine::Engine* engine);

            // Methods for EasterEggLoad State

            std::unique_ptr<rtype::client::utilities::CustomText> _counterText;
            int _counter;

            /**
             * @brief This is a method which initialize loading of the easter egg
             * @see This method is automatically called when scene is init
             * 
             * @param engine    A reference of the engine given by the engine himself
             */
            void createEasterEggLoad(rtype::engine::Engine* engine);

            // Methods for EasterEgg State

            /**
             * @brief This is a method which initialize easter egg himself
             * @see This method is automatically called when scene is init
             * 
             * @param engine    A reference of the engine given by the engine himself
             */
            void createEasterEgg(rtype::engine::Engine* engine);

            std::unique_ptr<rtype::client::utilities::CustomText> _scoreText;
            std::unique_ptr<rtype::client::utilities::CustomText> _timeLeftText;

            int _scoreNbr;

            // Methods for EasterEggEnd State

            /**
             * @brief This is a method which initialize the end of the easter egg
             * @see This method is automatically called when scene is init
             * 
             * @param engine    A reference of the engine given by the engine himself
             */
            void createEasterEggEnd(rtype::engine::Engine* engine);

            std::unique_ptr<rtype::client::utilities::CustomText> _endText;
            std::unique_ptr<rtype::client::utilities::CustomButton> _backToMenuButton;
            bool _displayScore;

        protected:
            MainMenuEasterEgg() : _titleLogo(LOGO_TEXTURE, sf::Vector2f(0, 0)) {}
            ~MainMenuEasterEgg() {}

        public:
            MainMenuEasterEgg(MainMenuEasterEgg&) = delete;
            void operator=(MainMenuEasterEgg const&) = delete;

            static MainMenuEasterEgg* getInstance()
            {
                std::lock_guard<std::mutex> lock(_mutex);

                if (_mainMenuEasterEggInstance == nullptr)
                    _mainMenuEasterEggInstance = new MainMenuEasterEgg();
                return _mainMenuEasterEggInstance;
            }

            static void releaseInstance()
            {
                _mainMenuEasterEggInstance = nullptr;
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
    }
}