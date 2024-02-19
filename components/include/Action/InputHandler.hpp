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
//  File:            InputHandler.hpp                                                //
//  Description:     Abstract class of the Input class management                    //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <memory>
#include <map>

#include "ACommand.hpp"
#include "SFML/Graphics.hpp"

// POVx, POVy are probably the d - pad.
// X, Y are probably the left most controller
// Z, R are either the right joystick or the triggers.
// U, V are whichever one Z, R isn't.

namespace rtype {
    namespace action {

        enum joystickAxis {
            UP      =   -100,
            DOWN    =   100,
            RIGHT   =   100,
            LEFT    =   -100,
        };

        enum joystickButton {
            A           =   0,
            B           =   1,
            X           =   2,
            Y           =   3,
    
            LB          =   4,
            RB          =   5,

            SELECT      =   6,
            START       =   7,

            CLICK_JOY_L =   8,
            CLICK_JOY_R =   9,
        };

        class InputHandler {
            private:
                std::map<sf::Keyboard::Key, std::unique_ptr<rtype::action::ACommand>> _inputsKeyboardMap;
                std::map<int, std::unique_ptr<rtype::action::ACommand>> _inputsJoystickButtonsMap;
                std::map<std::pair<sf::Joystick::Axis, int>, std::unique_ptr<rtype::action::ACommand>> _inputsJoystickAxisMap;

                std::vector<sf::Keyboard::Key> _bindedKeys;
                std::vector<int> _joystickBindedButtons;
                std::vector<std::pair<sf::Joystick::Axis, int>> _joystickBindedAxis;

            public:
                bool checkIfKeyIsBind(sf::Keyboard::Key key);
                std::unique_ptr<rtype::action::ACommand> &handleInput(sf::Keyboard::Key);
                void bindCommand(sf::Keyboard::Key, std::unique_ptr<rtype::action::ACommand>);

                std::vector<sf::Keyboard::Key> getBindedKeys(void);


                bool checkIfKeyIsBind(int joystickButton);
                std::unique_ptr<rtype::action::ACommand>& handleInput(int joystickButton);
                void bindCommand(int joystickButton, std::unique_ptr<rtype::action::ACommand>);

                std::vector<int> getJoystickBindedButtons(void);


                bool checkIfKeyIsBind(std::pair<sf::Joystick::Axis, int> joystickAxis);
                std::unique_ptr<rtype::action::ACommand>& handleInput(std::pair<sf::Joystick::Axis, int> joystickAxis);
                void bindCommand(std::pair<sf::Joystick::Axis, int> joystickAxis, std::unique_ptr<rtype::action::ACommand>);

                std::vector<std::pair<sf::Joystick::Axis, int>> getJoystickBindedAxis(void);

                void registerBindedKeys(void);
        };
    }
}