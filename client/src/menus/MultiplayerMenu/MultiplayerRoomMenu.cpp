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
//  File:            MultiplayerRoomMenu.cpp                                         //
//  Description:     Display all rooms created in the server                         //
//                                                                                   //
// ********************************************************************************* //

#include "MultiplayerRoomMenu.hpp"
#include "MultiplayerConfigMenu.hpp"
#include "MultiplayerWaitingRoom.hpp"

rtype::client::MultiplayerRoomMenu *rtype::client::MultiplayerRoomMenu::_roomMenuInstance{nullptr};
std::mutex rtype::client::MultiplayerRoomMenu::_mutex;

void goToMultiplayerConfig(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    engine->changeState(rtype::client::MultiplayerConfigMenu::getInstance());
}

void createRoom(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    engine->getClient()->askCreate();

    if (engine->getClient()->getOperationSuccedeed()) {
        engine->changeState(rtype::client::MultiplayerWaitingRoom::getInstance());
        engine->getClient()->resetOperationSuccedeed();
    } else
        std::cout << "Demand failed | Get a false return" << std::endl;
}

void refreshRoom(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    rtype::client::MultiplayerRoomMenu::getInstance()->setUpdate(true);
    engine->getClient()->askList();
}

void rtype::client::MultiplayerRoomMenu::init(rtype::engine::Engine *engine)
{
    _roomMenuInstance = this;
    this->_sceneTitle = std::make_unique<rtype::client::utilities::CustomText>("MULTIPLAYER ROOMS", FONT_PATH, 65, sf::Color::White, sf::Vector2f(engine->options.getWindowWidth() / 2, 50));
    this->_goBackBtn = std::make_unique<rtype::client::utilities::CustomButton>("< BACK", sf::Vector2f(0, 0), 20.0f, &goToMultiplayerConfig, 1);
    this->_createRoomBtn = std::make_unique<rtype::client::utilities::CustomButton>("+ CREATE ROOM", sf::Vector2f(0, 0), 20.0f, &createRoom, 2);
    this->_refreshRoomBtn = std::make_unique<rtype::client::utilities::CustomButton>("REFRESH", sf::Vector2f(0, 0), 20.0f, &refreshRoom, 3);
    this->_popup = std::make_unique<rtype::client::utilities::ConnectionPopup>(sf::Vector2f(engine->options.getWindowWidth() / 2, engine->options.getWindowHeight() / 2), engine);
    this->createMultiplayerRooms(engine);

    if (!this->_sceneTitle || !this->_goBackBtn || !this->_popup || !this->_createRoomBtn || !this->_refreshRoomBtn)
        throw rtype::settings::Errors("Cannot allocate more memory", "MemoryError");

    this->_isUpdate = false;
    this->_goBackBtn->setRightElements(2);
    this->_createRoomBtn->setLeftElements(1);
    this->_createRoomBtn->setUpElements(3);
    this->_refreshRoomBtn->setDownElements(2);
    this->_refreshRoomBtn->setLeftElements(1);

    this->_actualFocusEventID = 0;

    sf::FloatRect backBtnBounds = this->_goBackBtn->getText()->getGlobalBounds();
    sf::FloatRect createBtnBounds = this->_createRoomBtn->getText()->getGlobalBounds();
    sf::FloatRect refreshBtnBounds = this->_refreshRoomBtn->getText()->getGlobalBounds();
    this->_goBackBtn->setPosition(sf::Vector2f((backBtnBounds.width/2) + 35, engine->options.getWindowHeight() - (backBtnBounds.height/2) - 20));
    this->_createRoomBtn->setPosition(sf::Vector2f(engine->options.getWindowWidth() - (createBtnBounds.width/2) - 35, engine->options.getWindowHeight() - (createBtnBounds.height/2) - 20));
    this->_refreshRoomBtn->setPosition(sf::Vector2f(engine->options.getWindowWidth() - (refreshBtnBounds.width/2) - 35, engine->options.getWindowHeight() - (refreshBtnBounds.height/2) - 70));
    engine->getClient()->resetServerEndpoint();
}

void rtype::client::MultiplayerRoomMenu::cleanup()
{
    this->_rooms.clear();
    this->releaseInstance();
}

void rtype::client::MultiplayerRoomMenu::pause()
{
}

void rtype::client::MultiplayerRoomMenu::resume()
{
}

void rtype::client::MultiplayerRoomMenu::networkReceiver(rtype::engine::Engine *engine)
{
}

void rtype::client::MultiplayerRoomMenu::networkSender(rtype::engine::Engine *engine)
{
}

void rtype::client::MultiplayerRoomMenu::processInput(sf::Event event, rtype::engine::Engine *engine)
{
    int newId = 0;

    if (this->_popup->isOpen()) {
        this->_popup->handleEvents(event, engine);
        return;
    }

    if (sf::Joystick::isConnected(0)) {
        if (this->_goBackBtn->getId() == this->_actualFocusEventID)
            newId = this->_goBackBtn->eventUpdateJoystick(event, engine);
        if (this->_createRoomBtn->getId() == this->_actualFocusEventID)
            newId = this->_createRoomBtn->eventUpdateJoystick(event, engine);
        if (this->_refreshRoomBtn->getId() == this->_actualFocusEventID)
            newId = this->_refreshRoomBtn->eventUpdateJoystick(event, engine);
        for (uint16_t i = 0; i < this->_rooms.size(); i++) {
            if (this->_rooms[i].getJoinRoomBtn()->getId() == this->_actualFocusEventID)
                newId = this->_rooms[i].getJoinRoomBtn()->eventUpdateJoystick(event, engine);
        }
    }
    else {
        newId = this->_goBackBtn->eventUpdate(event, engine);
        newId = this->_createRoomBtn->eventUpdate(event, engine);
        newId = this->_refreshRoomBtn->eventUpdate(event, engine);
        for (uint16_t i = 0; i < this->_rooms.size(); i++)
            newId = this->_rooms[i].getJoinRoomBtn()->eventUpdate(event, engine);
    }


    if (newId != -1)
        this->_actualFocusEventID = newId;

    if (newId == 0)
        this->_actualFocusEventID = 1;
}

void rtype::client::MultiplayerRoomMenu::update(rtype::engine::Engine *engine)
{
    this->_background.update(engine);

    if (this->_popup->isOpen())
        this->_popup->update(engine);

    if (this->_goBackBtn->getId() == this->_actualFocusEventID)
        this->_goBackBtn->update(engine);
    if (this->_createRoomBtn->getId() == this->_actualFocusEventID)
        this->_createRoomBtn->update(engine);
    if (this->_refreshRoomBtn->getId() == this->_actualFocusEventID)
        this->_refreshRoomBtn->update(engine);

    for (uint16_t i = 0; i < this->_rooms.size(); i++) {
        if (this->_rooms[i].getJoinRoomBtn()->getId() == this->_actualFocusEventID)
            this->_rooms[i].getJoinRoomBtn()->update(engine);
    }

    if (this->_isUpdate) {
        this->createMultiplayerRooms(engine);
        this->_isUpdate = false;
    }

}

void rtype::client::MultiplayerRoomMenu::render(rtype::engine::Engine *engine)
{
    engine->window.clear();

    this->_background.display(engine);
    this->_sceneTitle->draw(engine);
    for (uint16_t i = 0; i < this->_rooms.size(); i++)
        this->_rooms[i].draw(engine);
    this->_goBackBtn->draw(engine);
    this->_createRoomBtn->draw(engine);
    this->_refreshRoomBtn->draw(engine);
    if (this->_popup->isOpen())
        this->_popup->draw(engine);

    engine->window.display();
}

void rtype::client::MultiplayerRoomMenu::updateObjectsPosition(sf::Vector2f deltaSize)
{
    this->_createRoomBtn->move(deltaSize);
    this->_goBackBtn->move(deltaSize);
    this->_refreshRoomBtn->move(deltaSize);
    for (unsigned int i = 0; i < this->_rooms.size(); i++)
        this->_rooms[i].move(deltaSize);
    this->_popup->move(deltaSize);
}

void rtype::client::MultiplayerRoomMenu::createMultiplayerRooms(rtype::engine::Engine* engine)
{
    if (this->_rooms.size() != 0)
        this->_rooms.clear();
    engine->getClient()->resetServerEndpoint();
    engine->getClient()->askList();
    std::vector<RoomInfo> rooms = engine->getClient()->getRoomList();
    std::vector<std::pair<std::string, int>> ROOM_DATA;

    for (size_t i = 0; i < rooms.size(); i++) {
        ROOM_DATA.push_back(std::pair<std::string, int>(std::string(rooms[i].roomName), static_cast<int>(rooms[i].playerCount)));
    }

    int posX = engine->options.getWindowWidth() / 4;
    int posY = 300;
    for (int i = 0; i < ROOM_DATA.size(); i++) {
        posX = (engine->options.getWindowWidth() / 4) * ((i % 3) + 1);
        this->_rooms.push_back(rtype::client::utilities::RoomInfoComponent(ROOM_DATA[i].first, ROOM_DATA[i].second, sf::Vector2f(posX, posY)));
        posY += (((i % 3) + 1) == 3) ? 230 : 0;

        this->_rooms[i].getJoinRoomBtn()->setId(4 + i);
        this->linkMultiplayerButtons();
    }
}

std::unique_ptr<rtype::client::utilities::ConnectionPopup> &rtype::client::MultiplayerRoomMenu::getPopup(void)
{
    return this->_popup;
}

void rtype::client::MultiplayerRoomMenu::setUpdate(bool isUpdate)
{
    this->_isUpdate = isUpdate;
}

void rtype::client::MultiplayerRoomMenu::linkMultiplayerButtons(void)
{
    if (this->_rooms.size() == 0) {
        this->_refreshRoomBtn->setUpElements(-1);
        this->_goBackBtn->setUpElements(-1);
        this->_actualFocusEventID = 1;
    }

    if (this->_rooms.size() >= 1) {
        this->_goBackBtn->setUpElements(this->_rooms[0].getJoinRoomBtn()->getId());
        this->_refreshRoomBtn->setUpElements(this->_rooms[0].getJoinRoomBtn()->getId());
        this->_rooms[0].getJoinRoomBtn()->setDownElements(this->_goBackBtn->getId());
    }

    if (this->_rooms.size() >= 2) {
        this->_refreshRoomBtn->setUpElements(this->_rooms[1].getJoinRoomBtn()->getId());
        this->_rooms[0].getJoinRoomBtn()->setRightElements(this->_rooms[1].getJoinRoomBtn()->getId());
        this->_rooms[1].getJoinRoomBtn()->setDownElements(this->_goBackBtn->getId());
        this->_rooms[1].getJoinRoomBtn()->setLeftElements(this->_rooms[0].getJoinRoomBtn()->getId());
    }

    if (this->_rooms.size() >= 3) {
        this->_refreshRoomBtn->setUpElements(this->_rooms[2].getJoinRoomBtn()->getId());
        this->_rooms[1].getJoinRoomBtn()->setRightElements(this->_rooms[2].getJoinRoomBtn()->getId());
        this->_rooms[2].getJoinRoomBtn()->setDownElements(this->_refreshRoomBtn->getId());
        this->_rooms[2].getJoinRoomBtn()->setLeftElements(this->_rooms[1].getJoinRoomBtn()->getId());
    }

    if (this->_rooms.size() >= 4) {
        this->_goBackBtn->setUpElements(this->_rooms[3].getJoinRoomBtn()->getId());
        this->_rooms[0].getJoinRoomBtn()->setDownElements(this->_rooms[3].getJoinRoomBtn()->getId());
        this->_rooms[3].getJoinRoomBtn()->setUpElements(this->_rooms[1].getJoinRoomBtn()->getId());
        this->_rooms[3].getJoinRoomBtn()->setDownElements(this->_goBackBtn->getId());
    }
    if (this->_rooms.size() >= 5) {
        this->_rooms[1].getJoinRoomBtn()->setDownElements(this->_rooms[4].getJoinRoomBtn()->getId());
        this->_rooms[3].getJoinRoomBtn()->setRightElements(this->_rooms[4].getJoinRoomBtn()->getId());
        this->_rooms[4].getJoinRoomBtn()->setUpElements(this->_rooms[1].getJoinRoomBtn()->getId());
        this->_rooms[4].getJoinRoomBtn()->setLeftElements(this->_rooms[3].getJoinRoomBtn()->getId());
        this->_rooms[4].getJoinRoomBtn()->setDownElements(this->_goBackBtn->getId());
    }
    if (this->_rooms.size() >= 6) {
        this->_refreshRoomBtn->setUpElements(this->_rooms[5].getJoinRoomBtn()->getId());
        this->_rooms[2].getJoinRoomBtn()->setDownElements(this->_rooms[5].getJoinRoomBtn()->getId());
        this->_rooms[4].getJoinRoomBtn()->setRightElements(this->_rooms[5].getJoinRoomBtn()->getId());
        this->_rooms[5].getJoinRoomBtn()->setUpElements(this->_rooms[2].getJoinRoomBtn()->getId());
        this->_rooms[5].getJoinRoomBtn()->setLeftElements(this->_rooms[4].getJoinRoomBtn()->getId());
        this->_rooms[5].getJoinRoomBtn()->setDownElements(this->_refreshRoomBtn->getId());
    }
    if (this->_rooms.size() >= 7) {
        this->_goBackBtn->setUpElements(this->_rooms[6].getJoinRoomBtn()->getId());
        this->_rooms[3].getJoinRoomBtn()->setDownElements(this->_rooms[6].getJoinRoomBtn()->getId());
        this->_rooms[6].getJoinRoomBtn()->setUpElements(this->_rooms[3].getJoinRoomBtn()->getId());
        this->_rooms[6].getJoinRoomBtn()->setDownElements(this->_goBackBtn->getId());
    }
    if (this->_rooms.size() >= 8) {
        this->_rooms[4].getJoinRoomBtn()->setDownElements(this->_rooms[7].getJoinRoomBtn()->getId());
        this->_rooms[6].getJoinRoomBtn()->setRightElements(this->_rooms[7].getJoinRoomBtn()->getId());
        this->_rooms[7].getJoinRoomBtn()->setUpElements(this->_rooms[4].getJoinRoomBtn()->getId());
        this->_rooms[7].getJoinRoomBtn()->setLeftElements(this->_rooms[6].getJoinRoomBtn()->getId());
        this->_rooms[7].getJoinRoomBtn()->setDownElements(this->_goBackBtn->getId());
    }
    if (this->_rooms.size() >= 9) {
        this->_refreshRoomBtn->setUpElements(this->_rooms[8].getJoinRoomBtn()->getId());
        this->_rooms[7].getJoinRoomBtn()->setRightElements(this->_rooms[8].getJoinRoomBtn()->getId());
        this->_rooms[8].getJoinRoomBtn()->setLeftElements(this->_rooms[7].getJoinRoomBtn()->getId());
        this->_rooms[8].getJoinRoomBtn()->setDownElements(this->_refreshRoomBtn->getId());
    }
}
