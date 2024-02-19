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
#include "CustomSlider.hpp"
#include "CustomButton.hpp"
#include "Player.hpp"
#include "BackgroundSpaceGenerator.hpp"

#include <SFML/Graphics.hpp>

#include <mutex>
#include <vector>
#include <utility>
#include <string>

namespace rtype {
    namespace client {
        class ColorPlayerPopup : public rtype::scene::AGameState {
        private:
            static ColorPlayerPopup* _instance;
            static std::mutex _mutex;

            std::vector<rtype::client::utilities::CustomSlider> _allSliders;
            std::vector<std::unique_ptr<rtype::client::utilities::CustomText>> _allTexts;
            std::vector<rtype::client::utilities::CustomButton> _allButtons;

            rtype::actor::Player _player;
            sf::Color _oldColor;

            sf::RectangleShape _background;

            void createSliders(rtype::engine::Engine* engine);
            void createButtons(rtype::engine::Engine* engine);
            void createTexts(rtype::engine::Engine* engine);

            int _actualFocusEventID;

        protected:
            ColorPlayerPopup() : _player("", 0) {}
            ~ColorPlayerPopup() {}

        public:
            ColorPlayerPopup(ColorPlayerPopup&) = delete;
            void operator=(ColorPlayerPopup const&) = delete;

            static ColorPlayerPopup* getInstance()
            {
                std::lock_guard<std::mutex> lock(_mutex);

                if (_instance == nullptr)
                    _instance = new ColorPlayerPopup();
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
    }
}