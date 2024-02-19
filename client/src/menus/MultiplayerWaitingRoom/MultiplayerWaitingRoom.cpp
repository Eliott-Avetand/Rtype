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
//  File:            MultiplayerWaitingRoom.cpp                                      //
//  Description:     Waiting room scene before the game launch                       //
//                                                                                   //
// ********************************************************************************* //

#include "MultiplayerWaitingRoom.hpp"
#include "MainMenu.hpp"
#include "LoadingScreen.hpp"
#include "MultiplayerRoomMenu.hpp"

rtype::client::MultiplayerWaitingRoom *rtype::client::MultiplayerWaitingRoom::_waitingRoomInstance{nullptr};
std::mutex rtype::client::MultiplayerWaitingRoom::_mutex;

void goBackToMultiplayerRoomMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    (void)data;
    engine->getClient()->askLeave();
    engine->changeState(rtype::client::MultiplayerRoomMenu::getInstance());
}

void launchGame(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    (void)data;
    engine->getClient()->askStart();
    if (engine->getClient()->getOperationSuccedeed() == false)
        return;
}

void rtype::client::MultiplayerWaitingRoom::init(rtype::engine::Engine *engine)
{
    _waitingRoomInstance = this;

    sf::Color color = engine->gameDatas.getPlayerColor();

    engine->getClient()->askName();
    engine->getClient()->askSetPlayerColor({color.r, color.g, color.b}, 20);

    this->_sceneTitle = std::make_unique<rtype::client::utilities::CustomText>("WAITING ROOM", FONT_PATH, 65, sf::Color::White, sf::Vector2f(engine->options.getWindowWidth() / 2, 50));
    this->_exitRoom = std::make_unique<rtype::client::utilities::CustomButton>("< EXIT", sf::Vector2f(0, 0), 20.0f, &goBackToMultiplayerRoomMenu, 2);
    this->_launchGame = std::make_unique<rtype::client::utilities::CustomButton>("LAUNCH GAME >", sf::Vector2f(0, 0), 20.0f, &launchGame, 1);
    this->_roomPassword = std::make_unique<rtype::client::utilities::CustomText>("", FONT_PATH, 20, sf::Color::White, sf::Vector2f(0, 0));

    if (!this->_sceneTitle || !this->_exitRoom || !this->_launchGame || !this->_roomPassword)
        throw rtype::settings::Errors("Cannot allocate more memory", "MemoryError");

    this->createMultiplayerSlots(engine);

    this->_exitRoom->setRightElements(1);
    this->_launchGame->setLeftElements(2);

    this->_actualFocusEventID = 0;

    sf::FloatRect exitBounds = this->_exitRoom->getText()->getGlobalBounds();
    sf::FloatRect launchBounds = this->_launchGame->getText()->getGlobalBounds();
    this->_exitRoom->setPosition(sf::Vector2f((exitBounds.width/2) + 35, engine->options.getWindowHeight() - (exitBounds.height/2) - 20));
    this->_launchGame->setPosition(sf::Vector2f(engine->options.getWindowWidth() - (launchBounds.width/2) - 35, engine->options.getWindowHeight() - (launchBounds.height/2) - 20));
}

void rtype::client::MultiplayerWaitingRoom::cleanup()
{
    this->_slots.clear();
    releaseInstance();
}

void rtype::client::MultiplayerWaitingRoom::pause()
{
}

void rtype::client::MultiplayerWaitingRoom::resume()
{
}

void rtype::client::MultiplayerWaitingRoom::networkReceiver(rtype::engine::Engine *engine)
{
    DataStruct data;
    boost::asio::ip::udp::endpoint senderEndpoint;
    boost::system::error_code error;

    engine->getClient()->getSocket().non_blocking(true);
    size_t byteReceive = engine->getClient()->getSocket().receive_from(boost::asio::buffer(&data, sizeof(DataStruct)), senderEndpoint, 0, error);

    if (byteReceive == 0) {
        return;
    } else {
        std::cout << "Received data" << std::endl;
    }
    int count = 0;

    switch (data.command) {
        case INFO:
            count = data.currentRoom.playerCount;

            this->updateDisplayOfRoomPassword(engine, std::string(data.currentRoom.password));

            for (int i = 0 ; i < 4; i++) {
                if (i < count)
                    this->_slots[i].setPlayerName(data.currentRoom.players[i].pseudo);
                else {
                    std::string str("SLOT " + std::to_string(i + 1));
                    this->_slots[i].setPlayerName(str);
                }
                this->_slots[i].updatePlayerNamePosition();

            }
            for (uint16_t i = 0; i < this->_slots.size(); i++) {
                std::string nameInSlot = this->_slots[i].getPlayerNameInSlot();
                if (nameInSlot == engine->getClient()->getPseudo()) {
                    this->_slots[i].getReadyCheckbox()->setId(3);
                    this->_slots[i].getReadyCheckbox()->setDownElements(1);
                }
            }
            this->_launchGame->setUpElements(3);
            this->_exitRoom->setUpElements(3);
            break;
        case START:
            engine->changeState(rtype::client::LoadingScreen::getInstance());
            break;
        case CLOSING:
            engine->changeState(rtype::client::MainMenu::getInstance());
            break;
        default:
            std::cout << "Received unknown command" << std::endl;
            break;
    }
}

void rtype::client::MultiplayerWaitingRoom::networkSender(rtype::engine::Engine *engine)
{
}

void rtype::client::MultiplayerWaitingRoom::processInput(sf::Event event, rtype::engine::Engine *engine)
{
    this->_background.eventUpdate(event, engine);

    int newId = -1;

    if (sf::Joystick::isConnected(0)) {
        if (this->_actualFocusEventID == this->_exitRoom->getId())
            newId = this->_exitRoom->eventUpdateJoystick(event, engine);
        if (this->_actualFocusEventID == this->_launchGame->getId())
            newId = this->_launchGame->eventUpdateJoystick(event, engine);

        for (uint16_t i = 0; i < this->_slots.size(); i++) {
            std::string nameInSlot = this->_slots[i].getPlayerNameInSlot();
            if (nameInSlot == engine->getClient()->getPseudo() && this->_actualFocusEventID == this->_slots[i].getReadyCheckbox()->getId())
                newId = this->_slots[i].getReadyCheckbox()->eventUpdateJoystick(event, engine);
        }
    }
    else {
        newId = this->_exitRoom->eventUpdate(event, engine);
        newId = this->_launchGame->eventUpdate(event, engine);

        for (uint16_t i = 0; i < this->_slots.size(); i++) {
            std::string nameInSlot = this->_slots[i].getPlayerNameInSlot();
            if (nameInSlot == engine->getClient()->getPseudo())
                newId = this->_slots[i].getReadyCheckbox()->eventUpdate(event, engine);
        }
    }

    if (newId != -1)
        this->_actualFocusEventID = newId;
    if (newId == 0 || this->_actualFocusEventID == 0)
        this->_actualFocusEventID = 1;
}

void rtype::client::MultiplayerWaitingRoom::update(rtype::engine::Engine *engine)
{
    this->_background.update(engine);

    if (this->_actualFocusEventID == this->_exitRoom->getId())
        this->_exitRoom->update(engine);
    if (this->_actualFocusEventID == this->_launchGame->getId())
        this->_launchGame->update(engine);

    for (uint16_t i = 0; i < this->_slots.size(); i++) {
        std::string nameInSlot = this->_slots[i].getPlayerNameInSlot();
        if (nameInSlot == engine->getClient()->getPseudo() && this->_actualFocusEventID == this->_slots[i].getReadyCheckbox()->getId())
            this->_slots[i].getReadyCheckbox()->update(engine);
    }
}

void rtype::client::MultiplayerWaitingRoom::render(rtype::engine::Engine *engine)
{
    engine->window.clear();

    this->_background.display(engine);
    this->_sceneTitle->draw(engine);
    for (uint16_t i = 0; i < this->_slots.size(); i++)
        this->_slots[i].draw(engine);
    this->_exitRoom->draw(engine);
    this->_launchGame->draw(engine);
    this->_roomPassword->draw(engine);

    engine->window.display();
}

void rtype::client::MultiplayerWaitingRoom::updateObjectsPosition(sf::Vector2f deltaSize)
{
    this->_exitRoom->move(deltaSize);
    this->_launchGame->move(deltaSize);
    this->_roomPassword->getTextObject().move(deltaSize);
    for (uint16_t i = 0; i < this->_slots.size(); i++)
        this->_slots[i].move(deltaSize);
}

void rtype::client::MultiplayerWaitingRoom::createMultiplayerSlots(rtype::engine::Engine* engine)
{
    int posX = engine->options.getWindowWidth() - 550;
    int posY = 300;
    for (int i = 0; i < 4; i++) {
        this->_slots.push_back(rtype::client::utilities::SlotInfoComponent("SLOT " + std::to_string(i+1), sf::Vector2f(posX, posY)));
        posY += 120;
    }
}

std::vector<rtype::client::utilities::SlotInfoComponent> &rtype::client::MultiplayerWaitingRoom::getSlots(void)
{
    return this->_slots;
}

void rtype::client::MultiplayerWaitingRoom::updateDisplayOfRoomPassword(rtype::engine::Engine* engine, std::string password)
{
    this->_roomPassword->setText("CODE : " + password);
    this->_roomPassword->setPosition(sf::Vector2f(engine->options.getWindowWidth() / 2, engine->options.getWindowHeight() - (this->_roomPassword->getHeight()/2) - 20));
}