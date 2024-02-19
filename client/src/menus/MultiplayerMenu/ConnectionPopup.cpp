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
//  File:            ConnectionPopup.cpp                                             //
//  Description:     Popup displayed when user wants to join a room                   //
//                                                                                   //
// ********************************************************************************* //

#include "ConnectionPopup.hpp"
#include "MultiplayerRoomMenu.hpp"
#include "MultiplayerWaitingRoom.hpp"
#include <boost/algorithm/string.hpp>

void closePopup(rtype::engine::Engine *engine, rtype::buttonutils::CallbackData &data)
{
    rtype::client::MultiplayerRoomMenu::getInstance()->getPopup()->close();
}

void connectToRoom(rtype::engine::Engine *engine, rtype::buttonutils::CallbackData &data)
{
    std::string psw = rtype::client::MultiplayerRoomMenu::getInstance()->getPopup()->getPasswordInputContent();
    std::string nickname = engine->getClient()->getPseudo();

    sf::Color color = engine->gameDatas.getPlayerColor();

    std::string upperPsw = boost::to_upper_copy<std::string>(psw);

    engine->getClient()->askJoin(nickname, upperPsw);

    if (engine->getClient()->getOperationSuccedeed() == false) {
        rtype::client::MultiplayerRoomMenu::getInstance()->getPopup()->close();
        return;
    }

    engine->changeState(rtype::client::MultiplayerWaitingRoom::getInstance());
}

rtype::client::utilities::ConnectionPopup::ConnectionPopup(sf::Vector2f pos, rtype::engine::Engine *engine)
{
    sf::Vector2f windowSize = sf::Vector2f(engine->options.getWindowWidth(), engine->options.getWindowHeight());
    this->_isOpen = false;
    this->_background = rtype::client::utilities::CustomRect(windowSize, sf::Color(20, 20, 20, 192), sf::Vector2f(0, 0));
    this->_popupContent = rtype::client::utilities::CustomRect(sf::Vector2f(windowSize.x / 2, windowSize. y / 2), sf::Color(10, 10, 10, 192), sf::Vector2f(windowSize.x / 2, windowSize. y / 2), rtype::globalutils::MIDDLE, sf::Color::White, 5);

    this->_actualFocusEventID = 0;

    sf::FloatRect contentBounds = this->_popupContent.getGlobalBounds();
    this->_inputBox = std::make_unique<rtype::client::utilities::InputBox>(sf::Vector2f(contentBounds.left + (contentBounds.width/2), contentBounds.top + (contentBounds.height/2)), sf::Vector2f(contentBounds.width - 200, 75), 20.0f, "ROOM PASSWORD", 1);
    this->_title = std::make_unique<rtype::client::utilities::CustomText>("Enter password\nfor ROOM 1", FONT_PATH, 28, sf::Color::White, sf::Vector2f(contentBounds.left + (contentBounds.width / 2), contentBounds.top + 50));
    this->_connectBtn = std::make_unique<rtype::client::utilities::CustomButton>("CONNECT TO ROOM >", sf::Vector2f(contentBounds.left + contentBounds.width - 200, contentBounds.top + contentBounds.height - 35), 20.0f, connectToRoom, 2);
    this->_closeBtn = std::make_unique<rtype::client::utilities::CustomButton>("X CLOSE", sf::Vector2f(contentBounds.left + 100, contentBounds.top + contentBounds.height - 35), 20.0f, closePopup, 3);
    if (!this->_title || !this->_connectBtn || !this->_closeBtn || !this->_inputBox)
        throw rtype::settings::Errors("Cannot allocate more memory", "MemoryError");

    this->_inputBox->setDownElements(2);

    this->_connectBtn->setUpElements(1);
    this->_connectBtn->setLeftElements(3);

    this->_closeBtn->setUpElements(1);
    this->_closeBtn->setRightElements(2);

}

void rtype::client::utilities::ConnectionPopup::setPosition(sf::Vector2f position)
{
    this->_background.setPosition(position);
}

void rtype::client::utilities::ConnectionPopup::move(sf::Vector2f offset)
{
    this->_background.getRect().move(offset);
    this->_popupContent.getRect().move(offset);
    this->_connectBtn->getText()->getTextObject().move(offset);
    this->_closeBtn->getText()->getTextObject().move(offset);
    this->_title->getTextObject().move(offset);
    this->_inputBox->move(offset);
}

sf::Vector2f rtype::client::utilities::ConnectionPopup::getPosition(void)
{
    return this->_background.getPosition();
}

float rtype::client::utilities::ConnectionPopup::getWidth(void)
{
    return this->_background.getLocalBounds().width;
}

float rtype::client::utilities::ConnectionPopup::getHeight(void)
{
    return this->_background.getLocalBounds().height;
}

void rtype::client::utilities::ConnectionPopup::open(std::string roomName)
{
    this->_title->setText("Enter password for " + roomName);
    this->_isOpen = true;
}

void rtype::client::utilities::ConnectionPopup::close(void)
{
    this->_isOpen = false;
}


bool rtype::client::utilities::ConnectionPopup::isOpen(void)
{
    return this->_isOpen;
}

void rtype::client::utilities::ConnectionPopup::draw(rtype::engine::Engine* engine)
{
    this->_background.draw(engine);
    this->_popupContent.draw(engine);
    this->_inputBox->draw(engine);
    this->_title->draw(engine);
    this->_connectBtn->draw(engine);
    this->_closeBtn->draw(engine);
    this->_inputBox->drawKeyboard(engine);
}

void rtype::client::utilities::ConnectionPopup::handleEvents(sf::Event event, rtype::engine::Engine *engine)
{
    int newId = -1;

    if (sf::Joystick::isConnected(0)) {
        if (this->_connectBtn->getId() == this->_actualFocusEventID)
            newId = this->_connectBtn->eventUpdateJoystick(event, engine);
        if (this->_closeBtn->getId() == this->_actualFocusEventID)
            newId = this->_closeBtn->eventUpdateJoystick(event, engine);
        if (this->_inputBox->getId() == this->_actualFocusEventID)
            newId = this->_inputBox->eventUpdateJoystick(event, engine);
    }
    else {
        newId = this->_connectBtn->eventUpdate(event, engine);
        newId = this->_closeBtn->eventUpdate(event, engine);
        newId = this->_inputBox->eventUpdate(event, engine);
    }

    if (newId != -1)
        this->_actualFocusEventID = newId;
    if (newId == 0 || this->_actualFocusEventID == 0)
        this->_actualFocusEventID = 1;
}

void rtype::client::utilities::ConnectionPopup::update(rtype::engine::Engine* engine)
{
        this->_connectBtn->update(engine);
        this->_closeBtn->update(engine);
        this->_inputBox->update(engine);
}

const std::string rtype::client::utilities::ConnectionPopup::getPasswordInputContent(void)
{
    return this->_inputBox->getContent();
}