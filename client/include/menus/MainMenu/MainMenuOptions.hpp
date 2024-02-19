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
//  File:            MainMenuOptions.hpp                                             //
//  Description:     Main Menu Options Scene Header                                  //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <mutex>
#include "Engine.hpp"
#include "AGameState.hpp"
#include "CustomText.hpp"
#include "CustomButton.hpp"
#include "BackgroundSpaceGenerator.hpp"
#include "CustomCheckBox.hpp"
#include "CustomSlider.hpp"
#include "InputHandler.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

/**
 * @brief This is a callback, it return to main menu
 * @see This method is call by back button
 * 
 * @param engine    A reference of the engine given by the engine himself
 * @param data      All datas needed for the functions
 */
void backToMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

void musicVolumeManagement(rtype::engine::Engine* engine, rtype::sliderutils::CallbackData& data);

void soundVolumeManagement(rtype::engine::Engine* engine, rtype::sliderutils::CallbackData& data);

/**
 * @brief This is a callback, it toggle music when call
 * @see This method is call by click on the music checkbox
 * 
 * @param engine    A reference of the engine given by the engine himself
 * @param data      All datas needed for the functions
 */
void toggleMusic(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

/**
 * @brief This is a callback, it toggle sound when call
 * @see This method is call by click on the sound checkbox
 * 
 * @param engine    A reference of the engine given by the engine himself
 * @param data      All datas needed for the functions
 */
void toggleSound(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

void cbToggleFullscreen(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data);

namespace rtype {
    namespace client {
        class MainMenuOptions : public rtype::scene::AGameState {
            private:
                static MainMenuOptions *_mainMenuOptions;
                static std::mutex _mutex;

                std::unique_ptr<rtype::client::utilities::CustomText> _titleScene;
                std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _allTexts;
                std::vector <rtype::client::utilities::CustomButton> _allButtons;
                std::vector<rtype::client::utilities::CustomCheckBox> _allCheckBox;
                std::vector<rtype::client::utilities::CustomSlider> _allSliders;
                rtype::client::utilities::BackgroundSpaceGenerator _background;

                int _actualFocusEventID;

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
                MainMenuOptions() {}
                ~MainMenuOptions() {}

            public:
                MainMenuOptions(MainMenuOptions&) = delete;
                void operator=(MainMenuOptions const &) = delete;

                static MainMenuOptions* getInstance()
                {
                    std::lock_guard<std::mutex> lock(_mutex);

                    if (_mainMenuOptions == nullptr)
                        _mainMenuOptions = new MainMenuOptions();
                    return _mainMenuOptions;
                }

                static void releaseInstance()
                {
                    _mainMenuOptions = nullptr;
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

                void updateObjectsPosition(sf::Vector2f deltaSize);
        };
    }
}