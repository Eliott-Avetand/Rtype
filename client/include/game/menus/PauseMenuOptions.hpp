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
#include "APlayerMovementComponent.hpp"
#include "AGameState.hpp"
#include "ACommand.hpp"
#include "Engine.hpp"
#include "Monster.hpp"
#include <SFML/Graphics.hpp>

#include "CustomText.hpp"
#include "MainMenuOptions.hpp"
#include "CustomButton.hpp"

#include "MainMenuOptions.hpp"
#include "PauseMenu.hpp"
#include "Level_1.hpp"

/**
 * @brief This is a callback, it return to pause menu
 * @see This method is call by back button
 *
 * @param engine    A reference of the engine given by the engine himself
 * @param data      All datas needed for the functions
 */
void backToPauseMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

namespace rtype {
    namespace client {
        class PauseMenuOptions : public rtype::scene::AGameState {
        private:
            static PauseMenuOptions *_pauseMenuOptionsInstance;
            static std::mutex _mutex;
            rtype::client::Level_1* _levelInstance;

            std::unique_ptr<rtype::client::utilities::CustomText> _titleScene;
            std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _allTexts;
            std::vector <rtype::client::utilities::CustomButton> _allButtons;
            std::vector<rtype::client::utilities::CustomCheckBox> _allCheckBox;
            std::vector<rtype::client::utilities::CustomSlider> _allSliders;
            sf::RectangleShape _background;

            int _actualFocusEventID;

            /**
             * @brief This is a method which initialize background
             * @see This method is automatically called when scene is init
             *
             * @param engine    A reference of the engine given by the engine himself
             */
            void createBackground(rtype::engine::Engine* engine);

            /**
             * @brief This is a method which initialize all buttons
             * @see This method is automatically called when scene is init
             *
             * @param engine    A reference of the engine given by the engine himself
             */
            void createButtons(rtype::engine::Engine* engine);

            /**
             * @brief This is a method which display all buttons
             * @see This method is automatically called when scene is render
             *
             * @param engine    A reference of the engine given by the engine himself
             */
            void displayButtons(rtype::engine::Engine* engine);

            /**
             * @brief This is a method which initialize title text
             * @see This method is automatically called when scene is init
             *
             * @param engine    A reference of the engine given by the engine himself
             */
            void createTitleText(rtype::engine::Engine* engine);

            /**
             * @brief This is a method which display title text
             * @see This method is automatically called when scene is render
             *
             * @param engine    A reference of the engine given by the engine himself
             */
            void displayTitleText(rtype::engine::Engine* engine);

            /**
             * @brief This is a method which create all options texts
             * @see This method is automatically called when scene is render
             *
             * @param engine    A reference of the engine given by the engine himself
             */
            void createAllSliders(rtype::engine::Engine* engine);

            /**
             * @brief This is a method which display all sliders
             * @see This method is automatically called when scene is render
             *
             * @param engine    A reference of the engine given by the engine himself
             */
            void displayAllSliders(rtype::engine::Engine* engine);

            /**
             * @brief This is a method which initialize all options texts
             * @see This method is automatically called when scene is init
             *
             * @param engine    A reference of the engine given by the engine himself
             */
            void createAllOptionsTexts(rtype::engine::Engine* engine);

            /**
             * @brief This is a method which display all options texts
             * @see This method is automatically called when scene is render
             *
             * @param engine    A reference of the engine given by the engine himself
             */
            void displayAllOptionsTexts(rtype::engine::Engine* engine);


            /**
             * @brief This is a method which initialize all checkboxes
             * @see This method is automatically called when scene is init
             *
             * @param engine    A reference of the engine given by the engine himself
             */
            void createAllCheckbox(rtype::engine::Engine* engine);

            /**
             * @brief This is a method which display all the checkboxes
             * @see This method is automatically called when scene is render
             *
             * @param engine    A reference of the engine given by the engine himself
             */
            void displayAllCheckBox(rtype::engine::Engine* engine);

        protected:
            PauseMenuOptions() {}
            ~PauseMenuOptions() {}

        public:
            PauseMenuOptions(PauseMenuOptions&) = delete;
            void operator=(PauseMenuOptions const&) = delete;

            static PauseMenuOptions* getInstance()
            {
                std::lock_guard<std::mutex> lock(_mutex);

                if (_pauseMenuOptionsInstance == nullptr)
                    _pauseMenuOptionsInstance = new PauseMenuOptions();
                return _pauseMenuOptionsInstance;
            }

            static void releaseInstance()
            {
                if (_pauseMenuOptionsInstance != nullptr) {
                    _pauseMenuOptionsInstance = nullptr;
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