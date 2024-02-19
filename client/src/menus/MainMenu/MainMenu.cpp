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
//  File:            MainMenu.cpp                                                    //
//  Description:     File for the MainMenu Class                                     //
//                                                                                   //
// ********************************************************************************* //

#include <iostream>

#include "MainMenu.hpp"
#include "MultiplayerConfigMenu.hpp"
#include "MainMenuOptions.hpp"
#include "MainMenuEasterEgg.hpp"
#include "ColorPlayerPopup.hpp"
#include "LoadingScreen.hpp"
#include "Credits.hpp"

#include "Errors.hpp"

rtype::client::MainMenu *rtype::client::MainMenu::_mainMenuInstance{nullptr};
std::mutex rtype::client::MainMenu::_mutex;

void playerSettings(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data)
{
    engine->pushState(rtype::client::ColorPlayerPopup::getInstance());
}

void quitGame(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    engine->window.close();
}

void playGame(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    engine->changeState(rtype::client::LoadingScreen::getInstance());
}

void goToOptionsMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    engine->changeState(rtype::client::MainMenuOptions::getInstance());
}

void goToMultiplayer(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    engine->changeState(rtype::client::MultiplayerConfigMenu::getInstance());
}

void goToCredits(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    engine->changeState(rtype::client::Credits::getInstance());
}

void rtype::client::MainMenu::init(rtype::engine::Engine* engine)
{
    std::cout << "Initializing the MainMenu class..." << std::endl;

    _mainMenuInstance = this;

    engine->getClient()->setIsOnline(false);

    this->createTitleLogo(engine);
    this->createButtons(engine);

    _actualFocusEventID = 0;
    engine->gameDatas.setActualLevel(1);

    _hasDestroyedAsteroids = false;
    engine->gameDatas.setScore(0);

    _background.toggleAsteroidsSpawn(true);
    _background.manageAsteroidsSpawnTime(0.1, 1.5);
    _background.addPlanet(GAS_PLANET_TEXTURE, 1, sf::Vector2f((float)engine->options.getWindowWidth() * 90.0f / 100.0f, 100), 1, 2, false);
    _background.addPlanet(GALAXY_TEXTURE, 1, sf::Vector2f(100, engine->options.getWindowHeight() * 80.0f / 100.0f), 1, 2, false);

    if (engine->options.music.getMusicStream().getStatus() == sf::SoundSource::Stopped || engine->options.music.getMusicPath() != MAINMENU_MUSIC_PATH)
        engine->options.music.changeMusic(MAINMENU_MUSIC_PATH);

    std::cout << "MainMenu class initialized." << std::endl;
}

void rtype::client::MainMenu::cleanup()
{
    std::cout << "Cleaning the MainMenu class..." << std::endl;
    this->_allButtons.clear();
    this->releaseInstance();
    std::cout << "MainMenu class cleaned up." << std::endl;
}

void rtype::client::MainMenu::pause()
{
}

void rtype::client::MainMenu::resume()
{
}

void rtype::client::MainMenu::networkReceiver(rtype::engine::Engine *engine)
{
}

void rtype::client::MainMenu::networkSender(rtype::engine::Engine *engine)
{
}

void rtype::client::MainMenu::processInput(sf::Event event, rtype::engine::Engine *engine)
{
    int newId = 0;

    _background.eventUpdate(event, engine);

    if (sf::Joystick::isConnected(0)) {
        for (unsigned int i = 0; i < _allButtons.size(); i++) {
            if (this->_actualFocusEventID == _allButtons[i].getId())
                newId = _allButtons[i].eventUpdateJoystick(event, engine);
        }
    }
    else {
        for (unsigned int i = 0; i < _allButtons.size(); i++)
            newId = _allButtons[i].eventUpdate(event, engine);
    }

    if (newId != -1)
        this->_actualFocusEventID = newId;
    if (newId == 0)
        this->_actualFocusEventID = 1;
}

void rtype::client::MainMenu::update(rtype::engine::Engine *engine)
{
    _background.update(engine);

    if (_background.getDestroyedAsteroids() >= 10 && !_hasDestroyedAsteroids) {
        _background.destroyAllAsteroids();
        _hasDestroyedAsteroids = true;
    }

    this->_allButtons.back().getText()->setColor(sf::Color(rtype::client::utilities::RandomNumbers::randomInteger(0, 255), rtype::client::utilities::RandomNumbers::randomInteger(0, 255), rtype::client::utilities::RandomNumbers::randomInteger(0, 255)));

    for (unsigned int i = 0; i < _allButtons.size(); i++) {
        if (this->_actualFocusEventID == _allButtons[i].getId())
            _allButtons[i].update(engine);
    }

    if (!_background.isAsteroids() && _hasDestroyedAsteroids)
        engine->changeState(rtype::client::MainMenuEasterEgg::getInstance());
}

void rtype::client::MainMenu::renderLevel(rtype::engine::Engine* engine)
{
    _background.display(engine);
    displayButtons(engine);
    displayTitleLogo(engine);
}

void rtype::client::MainMenu::render(rtype::engine::Engine *engine)
{
    engine->window.clear();

    this->renderLevel(engine);

    engine->window.display();
}

void rtype::client::MainMenu::updateObjectsPosition(sf::Vector2f deltaSize)
{
    for (int i = 0; i < this->_allButtons.size(); i++)
        this->_allButtons[i].move(deltaSize);
}

void rtype::client::MainMenu::createTitleLogo(rtype::engine::Engine* engine)
{
    _titleLogo.setPosition(sf::Vector2f(engine->options.getWindowWidth() / 2, engine->options.getWindowHeight() * 10 / 100));
    _titleLogo.setScale(1, 1.25);
}

void rtype::client::MainMenu::displayTitleLogo(rtype::engine::Engine* engine)
{
    _titleLogo.draw(engine);
}

void rtype::client::MainMenu::createButtons(rtype::engine::Engine* engine)
{
    float windowWidth = engine->options.getWindowWidth();
    float windowHeight = engine->options.getWindowHeight();

    std::vector<std::string> textButton = { "SOLO", "ONLINE", "OPTIONS", "QUIT GAME", "COLORS", "LOUSTIK STUDIO"};
    std::vector<sf::Vector2f> buttonPosition = { {windowWidth / 2, windowHeight / 2 - (windowHeight * 10 / 100)}, {windowWidth / 2, windowHeight / 2}, {windowWidth / 2, windowHeight / 2 + (windowHeight * 10 / 100)}, {windowWidth / 2, windowHeight / 2 + (windowHeight * 20 / 100)}, {0, 0}, {0, 0}};
    std::vector<rtype::buttonutils::callback_t> buttonCallback = { &playGame, &goToMultiplayer, &goToOptionsMenu, &quitGame, &playerSettings, &goToCredits };
    std::vector<int> buttonID = { 1, 2, 3, 4, 5, 6 };

    for (unsigned int i = 0; i < buttonCallback.size(); i++)
        _allButtons.push_back(rtype::client::utilities::CustomButton(textButton[i], buttonPosition[i], 75, buttonCallback[i], buttonID[i]));

    this->_allButtons[4].getText()->setCharacterSize(20);
    this->_allButtons[4].setPosition(sf::Vector2f(this->_allButtons[4].getText()->getLocalBounds().width / 2 + 35, engine->options.getWindowHeight() - (this->_allButtons[4].getText()->getLocalBounds().height / 2) - 20));
    this->_allButtons[5].getText()->setCharacterSize(20);
    this->_allButtons[5].setPosition(sf::Vector2f(engine->options.getWindowWidth() - (this->_allButtons[5].getText()->getLocalBounds().width / 2 + 35), engine->options.getWindowHeight() - (this->_allButtons[5].getText()->getLocalBounds().height / 2) - 20));

    this->_allButtons[0].setDownElements(2);
    this->_allButtons[1].setUpElements(1);
    this->_allButtons[1].setDownElements(3);
    this->_allButtons[2].setUpElements(2);
    this->_allButtons[2].setDownElements(4);
    this->_allButtons[3].setUpElements(3);
    this->_allButtons[3].setDownElements(5);
    this->_allButtons[4].setUpElements(4);
    this->_allButtons[4].setRightElements(6);
    this->_allButtons[5].setUpElements(4);
    this->_allButtons[5].setLeftElements(5);
}

void rtype::client::MainMenu::displayButtons(rtype::engine::Engine* engine)
{
    for (unsigned int i = 0; i < _allButtons.size(); i++)
        _allButtons[i].draw(engine);
}