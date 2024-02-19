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
//  File:            Exemple.cpp                                                     //
//  Description:     Exemple class file                                              //
//                                                                                   //
// ********************************************************************************* //
#include "PauseMenu.hpp"
#include "MainMenu.hpp"

rtype::client::PauseMenu *rtype::client::PauseMenu::_instance{nullptr};
std::mutex rtype::client::PauseMenu::_mutex;

void returnToMainMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data)
{
    (void)data;
    if (engine->getClient()->getIsOnline())
        engine->getClient()->askLeave();
    engine->changeState(rtype::client::MainMenu::getInstance());
}

void resumeGame(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data)
{
    engine->popState();
}

void goToOptionsGameMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data)
{
    (void)data;
    engine->pushState(rtype::client::PauseMenuOptions::getInstance());
}

void rtype::client::PauseMenu::init(rtype::engine::Engine* engine)
{
    std::cout << "Initializing the Pause Menu class..." << std::endl;
    _instance = this;
    initInput();

    this->_levelInstance = rtype::client::Level_1::getInstance();

    if (!this->_levelInstance)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->createBackground(engine);
    this->createTitle(engine);
    this->createButtons(engine);

    _actualFocusEventID = 0;

    std::cout << "Pause Menu class initialized." << std::endl;
}


void rtype::client::PauseMenu::initInput()
{

}


void rtype::client::PauseMenu::loadEnemies()
{
    // Nothing
}

void rtype::client::PauseMenu::cleanup()
{
    std::cout << "Cleaning the Pause Menu class..." << std::endl;
    releaseInstance();
    std::cout << "Pause Menu class cleaned up." << std::endl;
}

void rtype::client::PauseMenu::pause()
{
}

void rtype::client::PauseMenu::resume()
{
}

void rtype::client::PauseMenu::networkReceiver(rtype::engine::Engine* engine)
{
    this->_levelInstance->networkReceiver(engine);
}

void rtype::client::PauseMenu::networkSender(rtype::engine::Engine* engine)
{
    this->_levelInstance->networkSender(engine);
}

void rtype::client::PauseMenu::processInput(sf::Event event, rtype::engine::Engine* engine)
{
    int newId = 0;

    if (sf::Joystick::isConnected(0)) {
        if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == rtype::action::joystickButton::START)
            engine->popState();
        for (unsigned int i = 0; i < _allButtons.size(); i++) {
            if (this->_actualFocusEventID == _allButtons[i].getId())
                newId = _allButtons[i].eventUpdateJoystick(event, engine);
        }
    }
    else {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            engine->popState();
        for (unsigned int i = 0; i < _allButtons.size(); i++)
            newId = _allButtons[i].eventUpdate(event, engine);
    }

    if (newId != -1)
        this->_actualFocusEventID = newId;
    if (newId == 0)
        this->_actualFocusEventID = 1;

}

void rtype::client::PauseMenu::update(rtype::engine::Engine* engine)
{
    for (unsigned int i = 0; i < _allButtons.size(); i++) {
        if (this->_actualFocusEventID == _allButtons[i].getId())
            _allButtons[i].update(engine);
    }

    if (engine->getClient()->getIsOnline())
        this->_levelInstance->update(engine);
}

void rtype::client::PauseMenu::render(rtype::engine::Engine* engine)
{
    engine->window.clear();

    this->_levelInstance->renderLevel(engine);

    engine->window.draw(this->_background);

    this->_titleText.draw(engine);

    for (unsigned int i = 0; i < 3; i++)
        this->_allButtons[i].draw(engine);

    engine->window.display();
}

void rtype::client::PauseMenu::updateObjectsPosition(sf::Vector2f deltaSize)
{
    this->_titleText.move(deltaSize);
    this->_background.move(deltaSize);
    for (unsigned int i = 0; i < this->_allButtons.size(); i++)
        this->_allButtons[i].move(deltaSize);
}

void rtype::client::PauseMenu::createBackground(rtype::engine::Engine* engine)
{
    this->_background.setSize(sf::Vector2f(engine->options.getWindowWidth() * 2, engine->options.getWindowHeight() * 2));
    this->_background.setOrigin(sf::Vector2f(engine->options.getWindowWidth() / 2, engine->options.getWindowHeight() / 2));
    this->_background.setPosition(sf::Vector2f(engine->options.getWindowWidth() / 2, engine->options.getWindowHeight() / 2));
    this->_background.setFillColor(sf::Color(0, 0, 0, 160));
}

void rtype::client::PauseMenu::createTitle(rtype::engine::Engine* engine)
{
    this->_titleText.setText("PAUSE");
    this->_titleText.setColor(sf::Color::White);
    this->_titleText.setCharacterSize(125);
    this->_titleText.setPosition(sf::Vector2f(engine->options.getWindowWidth() / 2, engine->options.getWindowHeight() * 10 / 100));
}

void rtype::client::PauseMenu::createButtons(rtype::engine::Engine* engine)
{
    float windowWidth = engine->options.getWindowWidth();
    float windowHeight = engine->options.getWindowHeight();

    std::vector<std::string> textButton = { "RESUME", "OPTIONS", "QUIT GAME" };
    std::vector<sf::Vector2f> buttonPosition = {{windowWidth / 2, windowHeight / 2}, {windowWidth / 2, windowHeight / 2 + (windowHeight * 10 / 100)}, {windowWidth / 2, windowHeight / 2 + (windowHeight * 20 / 100)} };
    std::vector<rtype::buttonutils::callback_t> buttonCallback = { &resumeGame, &goToOptionsGameMenu, &returnToMainMenu };
    std::vector<int> id = { 1, 2, 3 };


    for (unsigned int i = 0; i < 3; i++)
        this->_allButtons.push_back(rtype::client::utilities::CustomButton(textButton[i], buttonPosition[i], 75, buttonCallback[i], id[i]));

    this->_allButtons[0].setDownElements(2);
    this->_allButtons[1].setUpElements(1);
    this->_allButtons[1].setDownElements(3);
    this->_allButtons[2].setUpElements(2);
    this->_allButtons[2].setDownElements(4);
}
