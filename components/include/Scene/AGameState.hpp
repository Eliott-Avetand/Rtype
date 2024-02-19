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
//  File:            AGameState.hpp                                                  //
//  Description:     Abstract class for the generic game engine                      //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include "Engine.hpp"
#include "InputHandler.hpp"

namespace rtype {
    namespace scene {

        /**
        ** @brief Abstract for game scene that handle inputs, update and render
        */
        class AGameState {
            protected:
                rtype::action::InputHandler inputHandler;

            public:
                /**
                 * @brief Init the scene, replace the constructor
                 *
                 * @param engine GameEngine pointer that will be used to draw in the window
                */
                virtual void init(rtype::engine::Engine *engine) = 0;

                /**
                 * @brief Used to free the scene components if needed
                */
                virtual void cleanup() = 0;

                /**
                 * @brief Pause the update and the render of the scene
                */
                virtual void pause() = 0;

                /**
                 * @brief Resume the update and the render of the scene
                */
                virtual void resume() = 0;

                /**
                 * @brief Used to send data from scene to a server
                */
                virtual void networkSender(rtype::engine::Engine *engine) = 0;

                /**
                 * @brief Used to receive data from server to scene
                */
                virtual void networkReceiver(rtype::engine::Engine *engine) = 0;

                /**
                 * @brief Handle user input in the scene
                 *
                 * @param event sf::Event contained in GameEngine, used to detect what event is fired by user
                 * @param engine GameEngine pointer that will be used to draw in the window
                */
                virtual void processInput(sf::Event event, rtype::engine::Engine *engine) = 0;

                /**
                 * @brief Update the components of the scene
                 *
                 * @param engine GameEngine pointer that will be used to draw in the window
                 * @param deltaTime Represent the time elapsed between two frames. Used to cap the framerate of each animations
                */
                virtual void update(rtype::engine::Engine *engine) = 0;

                /**
                 * @brief Render the components of the scene
                 *
                 * @param engine GameEngine pointer that will be used to draw in the window
                */
                virtual void render(rtype::engine::Engine *engine) = 0;

                /**
                 * @brief Update position of objects if the window is resized
                */
                virtual void updateObjectsPosition(sf::Vector2f deltaSize) = 0;

                /**
                 * @brief Change scene displayed in the window
                 *
                 * @param engine GameEngine pointer that will be used to draw in the window
                 * @param state Instance of the scene that will be displayed
                */
                virtual void changeState(rtype::engine::Engine *engine, rtype::scene::AGameState *state)
                {
                    engine->changeState(state);
                }
        };
    }
}