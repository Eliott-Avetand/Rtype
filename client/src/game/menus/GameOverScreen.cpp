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
//  File:            GameOverScreen.cpp                                              //
//  Description:     GameOverScreen Class Definition                                 //
//                                                                                   //
// ********************************************************************************* //
#include "GameOverScreen.hpp"
#include "MainMenu.hpp"

rtype::client::GameOverScreen* rtype::client::GameOverScreen::_instance{ nullptr };
std::mutex rtype::client::GameOverScreen::_mutex;

void disconnectAndBackToMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data)
{
    if (engine->getClient()->getIsOnline())
        engine->getClient()->askLeave();
    engine->changeState(rtype::client::MainMenu::getInstance());
}

void rtype::client::GameOverScreen::init(rtype::engine::Engine* engine)
{
    std::cout << "Initializing the GameOverScreen class..." << std::endl;
    _instance = this;

    this->_levelInstance = rtype::client::Level_1::getInstance();

    if (!this->_levelInstance)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->createBackground(engine);
    this->createTexts(engine);
    this->createButtons(engine);

    _actualFocusEventID = 0;

    if (engine->gameDatas.getScore() > engine->gameDatas.getHighScore())
        engine->gameDatas.setHighScore(engine->gameDatas.getScore());

    std::cout << "GameOverScreen class initialized." << std::endl;
}


void rtype::client::GameOverScreen::initInput()
{

}


void rtype::client::GameOverScreen::loadEnemies()
{
    // Nothing
}

void rtype::client::GameOverScreen::cleanup()
{
    std::cout << "Cleaning the GameOverScreen class..." << std::endl;
    releaseInstance();
    std::cout << "GameOverScreen class cleaned up." << std::endl;
}

void rtype::client::GameOverScreen::pause()
{
}

void rtype::client::GameOverScreen::resume()
{
}

void rtype::client::GameOverScreen::networkReceiver(rtype::engine::Engine* engine)
{
    this->_levelInstance->networkReceiver(engine);
}

void rtype::client::GameOverScreen::networkSender(rtype::engine::Engine* engine)
{
    this->_levelInstance->networkSender(engine);
}

void rtype::client::GameOverScreen::processInput(sf::Event event, rtype::engine::Engine* engine)
{
    int newId = 0;

    if (sf::Joystick::isConnected(0)) {
        if (this->_goToMenu->getId() == _actualFocusEventID)
            newId = this->_goToMenu->eventUpdateJoystick(event, engine);
    }
    else {
        newId = this->_goToMenu->eventUpdate(event, engine);
    }

    if (newId != -1)
        this->_actualFocusEventID = newId;
    if (newId == 0)
        this->_actualFocusEventID = 1;
}

void rtype::client::GameOverScreen::update(rtype::engine::Engine* engine)
{
    if (this->_actualFocusEventID == this->_goToMenu->getId())
        this->_goToMenu->update(engine);

    if (engine->getClient()->getIsOnline())
        this->_levelInstance->update(engine);
}

void rtype::client::GameOverScreen::render(rtype::engine::Engine* engine)
{
    engine->window.clear();

    this->_levelInstance->renderLevel(engine);

    engine->window.draw(this->_background);

    this->_gameOverText->draw(engine);
    engine->window.draw(this->_gameOverBar);

    this->_scoreSectionText->draw(engine);
    this->_scoreText->draw(engine);

    this->_goToMenu->draw(engine);

    engine->window.display();
}

void rtype::client::GameOverScreen::updateObjectsPosition(sf::Vector2f deltaSize)
{
    this->_gameOverText->move(deltaSize);
    this->_gameOverBar.move(deltaSize);
    this->_goToMenu->move(deltaSize);
    this->_scoreSectionText->move(deltaSize);
    this->_scoreText->move(deltaSize);
}

void rtype::client::GameOverScreen::createBackground(rtype::engine::Engine* engine)
{
    sf::Vector2f windowSize(engine->options.getWindowWidth(), engine->options.getWindowHeight());

    this->_background.setSize(sf::Vector2f(windowSize.x * 2, windowSize.y * 2));
    this->_background.setOrigin(sf::Vector2f(windowSize.x * 2 / 2, windowSize.y * 2 / 2));
    this->_background.setPosition(sf::Vector2f(windowSize.x / 2, windowSize.y / 2));
    this->_background.setFillColor(sf::Color(0, 0, 0, 160));
}

void rtype::client::GameOverScreen::createTexts(rtype::engine::Engine* engine)
{
    sf::Vector2f windowSize(engine->options.getWindowWidth(), engine->options.getWindowHeight());
    int score = engine->gameDatas.getScore();

    this->_gameOverText = std::make_unique<rtype::client::utilities::CustomText>("GAME OVER", FONT_PATH, 112, sf::Color::White, sf::Vector2f(windowSize.x / 2, windowSize.y * 10 / 100));
    if (!this->_gameOverText)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->_gameOverText->setPosition({ (float)windowSize.x * 50 / 100, ((float)windowSize.y * 20 / 100) - this->_gameOverText->getGlobalBounds().height });
    this->_gameOverBar.setSize({ (float)windowSize.x * 50 / 100, 10 });
    this->_gameOverBar.setOrigin(_gameOverBar.getGlobalBounds().width / 2, _gameOverBar.getGlobalBounds().height / 2);
    this->_gameOverBar.setFillColor(sf::Color::White);
    this->_gameOverBar.setPosition((float)windowSize.x * 50 / 100, (float)windowSize.y * 20 / 100);

    this->_scoreSectionText = std::make_unique<rtype::client::utilities::CustomText>("YOUR SCORE:", FONT_PATH, 75, sf::Color::White, sf::Vector2f(windowSize.x / 2, windowSize.y / 2));

    if (!this->_scoreSectionText)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->_scoreSectionText->setPosition({ (float)windowSize.x * 50 / 100, ((float)windowSize.y * 50 / 100) - this->_scoreSectionText->getGlobalBounds().height });

    this->_scoreText = std::make_unique<rtype::client::utilities::CustomText>(std::to_string(score), FONT_PATH, 75, sf::Color::White, sf::Vector2f((float)windowSize.x / 2, (float) windowSize.y * 10 / 100));

    if (!this->_scoreText)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->_scoreText->setPosition({ (float)windowSize.x * 50 / 100, ((float)windowSize.y * 50 / 100) + this->_scoreText->getGlobalBounds().height });
}

void rtype::client::GameOverScreen::createButtons(rtype::engine::Engine* engine)
{
    sf::Vector2f windowSize((float)engine->options.getWindowWidth(), (float)engine->options.getWindowHeight());

    this->_goToMenu = std::make_unique<rtype::client::utilities::CustomButton>("RETURN TO MENU", sf::Vector2f(windowSize.x / 2, windowSize.y - (windowSize.y * 10 / 100 )), 75.0f, &disconnectAndBackToMenu, 1);
    if (!this->_goToMenu)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");
}
