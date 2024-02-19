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
//  File:            InputBox.cpp                                                    //
//  Description:     Input box where player can type a text                          //
//                                                                                   //
// ********************************************************************************* //

#include "InputBox.hpp"

rtype::client::utilities::InputBox::InputBox(sf::Vector2f position, sf::Vector2f size, float fontSize, std::string placeholder, int id)
{
    rtype::engine::Engine* engine = rtype::engine::Engine::getInstance();

    this->_isFocused = false;
    this->_rawInput = "";
    this->_box = rtype::client::utilities::CustomRect(size, sf::Color(0, 0, 0, 0), position, rtype::globalutils::MIDDLE, sf::Color::White, 3);
    this->_placeholder = std::make_unique<rtype::client::utilities::CustomText>(placeholder, FONT_PATH, (int)fontSize, sf::Color(128, 128, 128, 192), position);
    this->_displayedText = std::make_unique<rtype::client::utilities::CustomText>("", FONT_PATH, (int)fontSize, sf::Color::White, position);
    if (!this->_placeholder || !this->_displayedText)
        throw rtype::settings::Errors("Cannot allocate more memory", "MemoryError");

    this->_joystickState = rtype::inputboxutils::joystickState::ARRIVED;
    this->_state = rtype::client::utilities::InputBox::inputboxState::NORMAL;

    if (position.y >= engine->options.getWindowHeight() / 2)
        this->_keyboard = std::make_unique<rtype::client::utilities::VirtualKeyboard>(rtype::client::utilities::VirtualKeyboard::TOP);
    else
        this->_keyboard = std::make_unique<rtype::client::utilities::VirtualKeyboard>(rtype::client::utilities::VirtualKeyboard::BOTTOM);

    if (!this->_keyboard)
        throw rtype::settings::Errors("Cannot allocate more memory", "MemoryError");

    this->_id = id;
    this->_rightElementsID = -1;
    this->_leftElementsID = -1;
    this->_upElementsID = -1;
    this->_downElementsID = -1;
}

void rtype::client::utilities::InputBox::draw(rtype::engine::Engine* engine)
{
    this->_box.draw(engine);
    if (!this->_isFocused && this->_rawInput.getSize() == 0)
        this->_placeholder->draw(engine);
    this->_displayedText->draw(engine);
}

void rtype::client::utilities::InputBox::drawKeyboard(rtype::engine::Engine* engine)
{
    this->_keyboard->draw(engine);
}

void rtype::client::utilities::InputBox::setPosition(sf::Vector2f position)
{
    this->_box.setPosition(position);
    this->_placeholder->setPosition(position);
    this->_displayedText->setPosition(position);
}

void rtype::client::utilities::InputBox::move(sf::Vector2f offset)
{
    this->_box.getRect().move(offset);
    this->_placeholder->getTextObject().move(offset);
    this->_displayedText->getTextObject().move(offset);
}

float rtype::client::utilities::InputBox::getWidth(void)
{
    return this->_box.getWidth();
}

float rtype::client::utilities::InputBox::getHeight(void)
{
    return this->_box.getHeight();
}

void rtype::client::utilities::InputBox::update(rtype::engine::Engine* engine)
{
    sf::Time time = this->_joystickClock.getElapsedTime();
    float milliseconds = time.asMilliseconds();

    this->_keyboard->update(engine);

    if (this->_state == rtype::client::utilities::InputBox::inputboxState::NORMAL) {
        this->_box.setOutlineColor(sf::Color::White);
        this->_joystickClock.restart();
        return;
    }

    if (this->_joystickState == rtype::inputboxutils::joystickState::FOCUS) {
        //if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -50 && sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) >= (int)rtype::action::joystickAxis::LEFT) {
        //    this->setPercentage(this->_percentage - 5);
        //    click(engine, this->_percentage);
        //}
        //if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) <= (int)rtype::action::joystickAxis::RIGHT && sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 50) {
        //    this->setPercentage(this->_percentage + 5);
        //    click(engine, this->_percentage);
        //}
    }

    if (milliseconds > 10) {
        if (this->_joystickState == rtype::inputboxutils::joystickState::STAY)
            this->_joystickState = rtype::inputboxutils::joystickState::CHANGE;
    }
}

int rtype::client::utilities::InputBox::eventUpdateJoystick(sf::Event event, rtype::engine::Engine* engine)
{
    int newID = -1;

    this->hover(engine->options.music.isSoundActive(), engine);

    if (this->_joystickState == rtype::inputboxutils::joystickState::FOCUS)
        this->_keyboard->updateJoystickEvent(event, engine);

    if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == rtype::action::joystickButton::A) {
        if (this->_joystickState != rtype::inputboxutils::joystickState::FOCUS) {
            if (engine->options.music.isSoundActive())
                engine->options.music.playClickSound();
            this->_joystickState = rtype::inputboxutils::joystickState::FOCUS;
            this->_keyboard->openKeyboard();
        }
        else {
            if (this->_keyboard->getLastKeyPressed() != "") {
                if (engine->options.music.isSoundActive())
                    engine->options.music.playClickSound();
                if (this->_keyboard->getLastKeyPressed() == "DEL") {
                    if (!this->_rawInput.isEmpty())
                        this->setContent(this->_displayedText->getText().erase(this->_displayedText->getText().size() - 1, 1));
                    return this->_id;
                }
                if (this->_rawInput.getSize() + 1 < INPUT_MAX_CHAR)
                    this->setContent(std::string(this->_displayedText->getText() + this->_keyboard->getLastKeyPressed()));
            }
        }
    }

    if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == rtype::action::joystickButton::B) {
        if (this->_joystickState == rtype::inputboxutils::joystickState::FOCUS) {
            this->_joystickState = rtype::inputboxutils::joystickState::STAY;
            this->_keyboard->closeKeyboard();
        }
    }

    if (event.type == sf::Event::JoystickMoved) {
        if (this->_joystickState == rtype::inputboxutils::joystickState::CHANGE) {
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
        else if (this->_joystickState == rtype::inputboxutils::joystickState::ARRIVED) {
            if ((event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position <= 0 && event.joystickMove.position > -5)) ||
                (event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position >= 0 && event.joystickMove.position < 5)) ||
                (event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position <= 0 && event.joystickMove.position > 5)) ||
                (event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position >= 0 && event.joystickMove.position < 5)))
                _joystickState = rtype::inputboxutils::joystickState::STAY;
        }
    }

    return this->_id;
}

void rtype::client::utilities::InputBox::released(void)
{
    this->_state = rtype::client::utilities::InputBox::NORMAL;
    this->_joystickState = rtype::inputboxutils::joystickState::ARRIVED;
}

void rtype::client::utilities::InputBox::hover(bool isSound, rtype::engine::Engine* engine)
{
    if (isSound && this->_state != rtype::client::utilities::InputBox::inputboxState::HOVER) {
        engine->options.music.playHoverSound();
        this->_joystickClock.restart();
    }
    if (sf::Joystick::isConnected(0))
        this->_box.setOutlineColor(sf::Color(128, 128, 128, 192));
    this->_state = rtype::client::utilities::InputBox::inputboxState::HOVER;
}

int rtype::client::utilities::InputBox::eventUpdate(sf::Event event, rtype::engine::Engine *engine)
{
    sf::Vector2f mousePos = engine->window.mapPixelToCoords(sf::Mouse::getPosition(engine->window));
    sf::FloatRect boxBounds = this->_box.getGlobalBounds();

    this->_keyboard->updateEvent(event, engine);

    this->_box.setOutlineColor(sf::Color::White);

    if (event.type == sf::Event::MouseButtonPressed)
        if (this->_keyboard->isHover(mousePos)) {
            this->_isFocused = true;
            if (this->_keyboard->getLastKeyPressed() != "") {
                if (engine->options.music.isSoundActive())
                    engine->options.music.playClickSound();
                this->setContent(std::string(this->_displayedText->getText() + this->_keyboard->getLastKeyPressed()));
            }
        }
        else
            this->_isFocused = boxBounds.contains(mousePos);

    if (this->_isFocused || boxBounds.contains(mousePos))
        this->hover(engine->options.music.isSoundActive(), engine);
    else
        released();

    if (!this->_isFocused && this->_keyboard->getState() == rtype::client::utilities::VirtualKeyboard::OPEN)
        this->_keyboard->closeKeyboard();

    if (this->_isFocused == true && event.type == sf::Event::KeyPressed) {
        if (event.key.control && event.key.code == sf::Keyboard::V) {
            std::string clipboard = sf::Clipboard::getString();
            if (clipboard.size() > INPUT_MAX_CHAR)
                clipboard.erase(clipboard.begin() + INPUT_MAX_CHAR, clipboard.end());
            this->_rawInput += clipboard;
            this->_displayedText->setText(this->_rawInput);
        }
    } else if (this->_isFocused == true && event.type == sf::Event::TextEntered) {
        if (event.text.unicode > 128 || event.text.unicode == '\t' || event.text.unicode == 22 || event.text.unicode == '_')
            return this->_id;
        else if (event.text.unicode == '\b') {
            if (!this->_rawInput.isEmpty())
                this->_rawInput.erase(this->_rawInput.getSize() - 1, 1);
        }
        else if (this->_rawInput.getSize() < INPUT_MAX_CHAR)
            this->_rawInput += event.text.unicode;
        this->_displayedText->setText(this->_rawInput);
    }

    return this->_id;

}

const std::string rtype::client::utilities::InputBox::getContent(void)
{
    return this->_rawInput;
}

void rtype::client::utilities::InputBox::setContent(std::string content)
{
    this->_rawInput = content;
    this->_displayedText->setText(content);
}

int rtype::client::utilities::InputBox::getId(void)
{
    return this->_id;
}

void rtype::client::utilities::InputBox::setId(int Id)
{
    this->_id = Id;
}

void rtype::client::utilities::InputBox::setRightElements(int Id)
{
    this->_rightElementsID = Id;
}

int rtype::client::utilities::InputBox::getRightElements(void)
{
    return this->_rightElementsID;
}

void rtype::client::utilities::InputBox::setLeftElements(int Id)
{
    this->_leftElementsID = Id;
}

int rtype::client::utilities::InputBox::getLeftElements(void)
{
    return this->_leftElementsID;
}

void rtype::client::utilities::InputBox::setUpElements(int Id)
{
    this->_upElementsID = Id;
}

int rtype::client::utilities::InputBox::getUpElements(void)
{
    return this->_upElementsID;
}

void rtype::client::utilities::InputBox::setDownElements(int Id)
{
    this->_downElementsID = Id;
}

int rtype::client::utilities::InputBox::getDownElements(void)
{
    return this->_downElementsID;
}