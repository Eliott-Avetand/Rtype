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
//  File:            MultiplayerConfigMenu.cpp                                       //
//  Description:     Scene where player is asked to fill is pseudo and server ip     //
//                                                                                   //
// ********************************************************************************* //

#include "MultiplayerConfigMenu.hpp"
#include "MultiplayerRoomMenu.hpp"
#include "MainMenu.hpp"

rtype::client::MultiplayerConfigMenu *rtype::client::MultiplayerConfigMenu::_configMenuInstance{nullptr};
std::mutex rtype::client::MultiplayerConfigMenu::_mutex;

void goToMainMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    engine->changeState(rtype::client::MainMenu::getInstance());
}

void goToMultiplayerRoom(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    rtype::client::MultiplayerConfigMenu *mpConfigInstance = rtype::client::MultiplayerConfigMenu::getInstance();
    std::string pseudoContent = mpConfigInstance->getPseudoInput()->getContent();
    std::string ipContent = mpConfigInstance->getIpInput()->getContent();
    if (pseudoContent.empty() || ipContent.empty())
        return;

    std::regex word_regex("\\b(?:(?:2(?:[0-4][0-9]|5[0-5])|[0-1]?[0-9]?[0-9])\\.){3}(?:(?:2([0-4][0-9]|5[0-5])|[0-1]?[0-9]?[0-9]))\\b:[0-9]+");
    if (!std::regex_match(ipContent, word_regex))
        return;

    std::string ip(ipContent.substr(0, ipContent.find(":")));
    std::string port(ipContent.substr(ipContent.find(":") + 1, ipContent.size()));

    engine->getClient()->setServerHost(ip);
    engine->getClient()->setServerPort(static_cast<short>(std::stoi(port)));
    engine->getClient()->setPseudo(pseudoContent);
    engine->getClient()->resetServerEndpoint();
    engine->changeState(rtype::client::MultiplayerRoomMenu::getInstance());
}

void rtype::client::MultiplayerConfigMenu::init(rtype::engine::Engine *engine)
{
    _configMenuInstance = this;

    this->_actualFocusEventID = 0;

    this->_sceneTitle = std::make_unique<rtype::client::utilities::CustomText>("MULTIPLAYER CONFIG", FONT_PATH, 65, sf::Color::White, sf::Vector2f(engine->options.getWindowWidth() / 2, 50));
    this->_goBackBtn = std::make_unique<rtype::client::utilities::CustomButton>("< BACK", sf::Vector2f(0, 0), 20.0f, goToMainMenu, 4);
    this->_confirmBtn = std::make_unique<rtype::client::utilities::CustomButton>("CONFIRM >", sf::Vector2f(0, 0), 20.0f, goToMultiplayerRoom, 3);

    this->_pseudoLabel = std::make_unique<rtype::client::utilities::CustomText>("ENTER YOUR PSEUDO", FONT_PATH, 30, sf::Color::White, sf::Vector2f(engine->options.getWindowWidth() / 2, 350));
    this->_pseudoInput = std::make_unique<rtype::client::utilities::InputBox>(sf::Vector2f(engine->options.getWindowWidth() / 2, 425), sf::Vector2f(550, 75), 20.0f, "YOUR PSEUDO", 1);

    this->_ipLabel = std::make_unique<rtype::client::utilities::CustomText>("ENTER SERVER IP", FONT_PATH, 30, sf::Color::White, sf::Vector2f(engine->options.getWindowWidth() / 2, 650));
    this->_ipInput = std::make_unique<rtype::client::utilities::InputBox>(sf::Vector2f(engine->options.getWindowWidth() / 2, 725), sf::Vector2f(550, 75), 20.0f, "0.0.0.0:1234", 2);

    if (!this->_sceneTitle || !this->_goBackBtn || !this->_confirmBtn || !this->_pseudoLabel || !this->_pseudoInput || !this->_ipLabel || !this->_ipInput)
        throw rtype::settings::Errors("Cannot allocate more memory", "MemoryError");

    this->_pseudoInput->setDownElements(2);

    this->_ipInput->setUpElements(1);
    this->_ipInput->setDownElements(3);

    this->_goBackBtn->setRightElements(3);
    this->_goBackBtn->setUpElements(2);

    this->_confirmBtn->setLeftElements(4);
    this->_confirmBtn->setUpElements(2);

    sf::FloatRect backBounds = this->_goBackBtn->getText()->getGlobalBounds();
    sf::FloatRect confirmBounds = this->_confirmBtn->getText()->getGlobalBounds();
    this->_goBackBtn->setPosition(sf::Vector2f((backBounds.width/2) + 35, engine->options.getWindowHeight() - (backBounds.height/2) - 20));
    this->_confirmBtn->setPosition(sf::Vector2f(engine->options.getWindowWidth() - (confirmBounds.width/2) - 35, engine->options.getWindowHeight() - (confirmBounds.height/2) - 20));

    if (engine->getClient() && !engine->getClient()->getPseudo().empty())
        this->_pseudoInput->setContent(engine->getClient()->getPseudo());
    engine->getClient()->setIsOnline(true);
}

void rtype::client::MultiplayerConfigMenu::cleanup()
{
    releaseInstance();
}

void rtype::client::MultiplayerConfigMenu::pause()
{
}

void rtype::client::MultiplayerConfigMenu::resume()
{
}

void rtype::client::MultiplayerConfigMenu::networkReceiver(rtype::engine::Engine *engine)
{
}

void rtype::client::MultiplayerConfigMenu::networkSender(rtype::engine::Engine *engine)
{
}

void rtype::client::MultiplayerConfigMenu::processInput(sf::Event event, rtype::engine::Engine *engine)
{
    int newId = 0;

    this->_background.eventUpdate(event, engine);

    if (sf::Joystick::isConnected(0)) {
        if (this->_goBackBtn->getId() == this->_actualFocusEventID)
            newId = this->_goBackBtn->eventUpdateJoystick(event, engine);
        if (this->_confirmBtn->getId() == this->_actualFocusEventID)
            newId = this->_confirmBtn->eventUpdateJoystick(event, engine);
        if (this->_pseudoInput->getId() == this->_actualFocusEventID)
            newId = this->_pseudoInput->eventUpdateJoystick(event, engine);
        if (this->_ipInput->getId() == this->_actualFocusEventID)
            newId = this->_ipInput->eventUpdateJoystick(event, engine);
    }
    else {
        newId = this->_goBackBtn->eventUpdate(event, engine);
        newId = this->_confirmBtn->eventUpdate(event, engine);

        newId = this->_pseudoInput->eventUpdate(event, engine);
        newId = this->_ipInput->eventUpdate(event, engine);
    }

    if (newId != -1)
        this->_actualFocusEventID = newId;

    if (newId == 0)
        this->_actualFocusEventID = 1;

}

void rtype::client::MultiplayerConfigMenu::update(rtype::engine::Engine *engine)
{
    this->_background.update(engine);
    this->_goBackBtn->update(engine);
    this->_confirmBtn->update(engine);
    this->_ipInput->update(engine);
    this->_pseudoInput->update(engine);
}

void rtype::client::MultiplayerConfigMenu::render(rtype::engine::Engine *engine)
{
    engine->window.clear();

    this->_background.display(engine);
    this->_sceneTitle->draw(engine);

    this->_goBackBtn->draw(engine);
    this->_confirmBtn->draw(engine);

    this->_ipLabel->draw(engine);
    this->_pseudoLabel->draw(engine);
    this->_ipInput->draw(engine);
    this->_pseudoInput->draw(engine);

    this->_ipInput->drawKeyboard(engine);
    this->_pseudoInput->drawKeyboard(engine);

    engine->window.display();
}

void rtype::client::MultiplayerConfigMenu::updateObjectsPosition(sf::Vector2f deltaSize)
{
    this->_confirmBtn->move(deltaSize);
    this->_goBackBtn->move(deltaSize);
    this->_pseudoLabel->getTextObject().move(deltaSize);
    this->_ipLabel->getTextObject().move(deltaSize);
    this->_pseudoInput->move(deltaSize);
    this->_ipInput->move(deltaSize);
}

std::unique_ptr<rtype::client::utilities::InputBox> &rtype::client::MultiplayerConfigMenu::getPseudoInput(void)
{
    return this->_pseudoInput;
}

std::unique_ptr<rtype::client::utilities::InputBox> &rtype::client::MultiplayerConfigMenu::getIpInput(void)
{
    return this->_ipInput;
}