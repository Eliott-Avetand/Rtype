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
//  File:            InputHandler.cpp                                                //
//  Description:     InputHandler class                                              //
//                                                                                   //
// ********************************************************************************* //

#include "InputHandler.hpp"

void rtype::action::InputHandler::registerBindedKeys(void)
{
    for (auto it = this->_inputsKeyboardMap.begin(); it != this->_inputsKeyboardMap.end(); it++)
        this->_bindedKeys.push_back(it->first);

    for (auto it = this->_inputsJoystickButtonsMap.begin(); it != this->_inputsJoystickButtonsMap.end(); it++)
        this->_joystickBindedButtons.push_back(it->first);

    for (auto it = this->_inputsJoystickAxisMap.begin(); it != this->_inputsJoystickAxisMap.end(); it++)
        this->_joystickBindedAxis.push_back(it->first);
}

// Inputs KeyBoard

std::unique_ptr<rtype::action::ACommand> &rtype::action::InputHandler::handleInput(sf::Keyboard::Key key)
{
    return this->_inputsKeyboardMap[key];
}

void rtype::action::InputHandler::bindCommand(sf::Keyboard::Key key, std::unique_ptr<rtype::action::ACommand> command)
{
    _inputsKeyboardMap[key] = std::move(command);
}

bool rtype::action::InputHandler::checkIfKeyIsBind(sf::Keyboard::Key key)
{
    return this->_inputsKeyboardMap.find(key) != this->_inputsKeyboardMap.end();
}

std::vector<sf::Keyboard::Key> rtype::action::InputHandler::getBindedKeys(void)
{
    return this->_bindedKeys;
}

// Joysticks Buttons

std::unique_ptr<rtype::action::ACommand>& rtype::action::InputHandler::handleInput(int joystickButton)
{
    return this->_inputsJoystickButtonsMap[joystickButton];
}

void rtype::action::InputHandler::bindCommand(int joystickButton, std::unique_ptr<rtype::action::ACommand> command)
{
    _inputsJoystickButtonsMap[joystickButton] = std::move(command);
}

bool rtype::action::InputHandler::checkIfKeyIsBind(int joystickButton)
{
    return this->_inputsJoystickButtonsMap.find(joystickButton) != this->_inputsJoystickButtonsMap.end();
}

std::vector<int> rtype::action::InputHandler::getJoystickBindedButtons(void)
{
    return this->_joystickBindedButtons;
}

// Joysticks Axis

std::unique_ptr<rtype::action::ACommand>& rtype::action::InputHandler::handleInput(std::pair<sf::Joystick::Axis, int> joystickAxis)
{
    return this->_inputsJoystickAxisMap[joystickAxis];
}

void rtype::action::InputHandler::bindCommand(std::pair<sf::Joystick::Axis, int> joystickAxis, std::unique_ptr<rtype::action::ACommand> command)
{
    _inputsJoystickAxisMap[joystickAxis] = std::move(command);
}

bool rtype::action::InputHandler::checkIfKeyIsBind(std::pair<sf::Joystick::Axis, int> joystickAxis)
{
    return this->_inputsJoystickAxisMap.find(joystickAxis) != this->_inputsJoystickAxisMap.end();
}

std::vector<std::pair<sf::Joystick::Axis, int>> rtype::action::InputHandler::getJoystickBindedAxis(void)
{
    return this->_joystickBindedAxis;
}