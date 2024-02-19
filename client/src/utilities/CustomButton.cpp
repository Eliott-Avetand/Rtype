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
//  File:            CustomButton.cpp                                                //
//  Description:     File for the CustomButton Class                                 //
//                                                                                   //
// ********************************************************************************* //

#include "CustomButton.hpp"

rtype::client::utilities::CustomButton::CustomButton(std::string text, sf::Vector2f position, float fontSize, rtype::buttonutils::callback_t callback, int id)
{
    this->_color = sf::Color::White;
    this->_callback = callback;
    this->_text = std::make_unique<CustomText>(text, FONT_PATH, static_cast<int>(fontSize), sf::Color::White, position);
    if (!this->_text)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");
    this->_bounds = _text->getGlobalBounds();
    this->_text->setPosition(position);

    this->_joystickState = rtype::buttonutils::joystickState::ARRIVED;

    this->_id = id;
    this->_rightElementsID = -1;
    this->_leftElementsID = -1;
    this->_upElementsID = -1;
    this->_downElementsID = -1;
}

void rtype::client::utilities::CustomButton::draw(rtype::engine::Engine* engine)
{
    _text->draw(engine);
}

void rtype::client::utilities::CustomButton::hover(bool isSound, rtype::engine::Engine* engine)
{
    if (isSound && _state != rtype::client::utilities::CustomButton::buttonState::HOVER) {
        engine->options.music.playHoverSound();
        this->_joystickClock.restart();
    }
    _state = rtype::client::utilities::CustomButton::buttonState::HOVER;
    _text->setColor(sf::Color(128, 128, 128));
}

void rtype::client::utilities::CustomButton::press()
{
    _state = rtype::client::utilities::CustomButton::buttonState::CLICKED;
}

void rtype::client::utilities::CustomButton::click(bool isSound, rtype::engine::Engine* engine)
{
    engine->options.music.playClickSound();
    if (this->_callback)
        this->_callback(engine, this->_data);
}

void rtype::client::utilities::CustomButton::released()
{
    _state = rtype::client::utilities::CustomButton::buttonState::NORMAL;
    _joystickState = rtype::buttonutils::joystickState::ARRIVED;
    _text->setColor(sf::Color::White);
}

bool rtype::client::utilities::CustomButton::isHover(sf::Vector2f mousePosition)
{
    if (_bounds.contains(mousePosition))
        return true;
    return false;
}

void rtype::client::utilities::CustomButton::setColor(sf::Color color)
{
    _color = color;
}

void rtype::client::utilities::CustomButton::setCallback(rtype::buttonutils::callback_t callback)
{
    _callback = callback;
}

void rtype::client::utilities::CustomButton::setPosition(sf::Vector2f position)
{
    _text->setPosition(position);
    _bounds = _text->getGlobalBounds();
}

void rtype::client::utilities::CustomButton::move(sf::Vector2f offset)
{
    _text->move(offset);
    _bounds = _text->getGlobalBounds();
}

void rtype::client::utilities::CustomButton::setOrigin(float x, float y)
{
    this->_text->setOrigin(x, y);
}

float rtype::client::utilities::CustomButton::getWidth(void)
{
    return _bounds.width;
}

float rtype::client::utilities::CustomButton::getHeight(void)
{
    return _bounds.height;
}

std::unique_ptr<rtype::client::utilities::CustomText> &rtype::client::utilities::CustomButton::getText(void)
{
    return this->_text;
}

void rtype::client::utilities::CustomButton::setAdditionalData(rtype::buttonutils::CallbackData &data)
{
    this->_data = data;
}

void rtype::client::utilities::CustomButton::update(rtype::engine::Engine* engine)
{
    sf::Time time = this->_joystickClock.getElapsedTime();
    float milliseconds = time.asMilliseconds();

    if (this->_state != HOVER) {
        this->_joystickClock.restart();
        return;
    }

    if (milliseconds > 10) {
        if (this->_joystickState == rtype::buttonutils::joystickState::STAY)
            this->_joystickState = rtype::buttonutils::joystickState::CHANGE;
    }
}

int rtype::client::utilities::CustomButton::eventUpdateJoystick(sf::Event event, rtype::engine::Engine* engine)
{
    int newID = -1;

    hover(engine->options.music.isSoundActive(), engine);

    if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == rtype::action::joystickButton::A)
        click(engine->options.music.isSoundActive(), engine);

    if (event.type == sf::Event::JoystickMoved) {
        if (this->_joystickState == rtype::buttonutils::joystickState::CHANGE) {
            if (event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position >= (int)rtype::action::joystickAxis::LEFT && event.joystickMove.position < -50))
                newID = this->_leftElementsID;
            if (event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position <= (int)rtype::action::joystickAxis::RIGHT && event.joystickMove.position > 50))
                newID = this->_rightElementsID;
            if (event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position >= (int)rtype::action::joystickAxis::UP && event.joystickMove.position < -50))
                newID = this->_upElementsID;
            if (event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position <= (int)rtype::action::joystickAxis::DOWN && event.joystickMove.position > 50))
                newID = this->_downElementsID;
        
            if (newID != -1)
                released();

            return newID;
        }
        else if (this->_joystickState == rtype::buttonutils::joystickState::ARRIVED) {
            if ((event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position <= 0 && event.joystickMove.position > -5)) ||
                (event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position >= 0 && event.joystickMove.position < 5)) ||
                (event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position <= 0 && event.joystickMove.position > 5)) ||
                (event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position >= 0 && event.joystickMove.position < 5)))
                _joystickState = rtype::buttonutils::joystickState::STAY;
        }
    }

    return this->_id;
}

int rtype::client::utilities::CustomButton::eventUpdate(sf::Event event, rtype::engine::Engine *engine)
{
    if (isHover(engine->window.mapPixelToCoords(sf::Mouse::getPosition(engine->window)))) {
        hover(engine->options.music.isSoundActive(), engine);
        if (event.type == sf::Event::MouseButtonPressed)
            click(engine->options.music.isSoundActive(), engine);
    } else
        released();
    return this->_id;
}

int rtype::client::utilities::CustomButton::getId(void)
{
    return this->_id;
}

void rtype::client::utilities::CustomButton::setId(int Id)
{
    this->_id = Id;
}

void rtype::client::utilities::CustomButton::setRightElements(int Id)
{
    this->_rightElementsID = Id;
}

int rtype::client::utilities::CustomButton::getRightElements(void)
{
    return this->_rightElementsID;
}

void rtype::client::utilities::CustomButton::setLeftElements(int Id)
{
    this->_leftElementsID = Id;
}

int rtype::client::utilities::CustomButton::getLeftElements(void)
{
    return this->_leftElementsID;
}

void rtype::client::utilities::CustomButton::setUpElements(int Id)
{
    this->_upElementsID = Id;
}

int rtype::client::utilities::CustomButton::getUpElements(void)
{
    return this->_upElementsID;
}

void rtype::client::utilities::CustomButton::setDownElements(int Id)
{
    this->_downElementsID = Id;
}

int rtype::client::utilities::CustomButton::getDownElements(void)
{
    return this->_downElementsID;
}