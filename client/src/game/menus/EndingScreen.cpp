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
//  File:            EndingScreen.cpp                                                //
//  Description:     EndingScreen Class Definition                                   //
//                                                                                   //
// ********************************************************************************* //

#include "EndingScreen.hpp"
#include "GameOverScreen.hpp"
#include "MainMenu.hpp"

rtype::client::EndingScreen* rtype::client::EndingScreen::_instance{ nullptr };
std::mutex rtype::client::EndingScreen::_mutex;

void backToFirstMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data)
{
    (void)data;
    engine->changeState(rtype::client::MainMenu::getInstance());
}

void rtype::client::EndingScreen::init(rtype::engine::Engine* engine)
{
    std::cout << "Initializing the EndingScreen class..." << std::endl;

    sf::Vector2f windowSize(engine->options.getWindowWidth(), engine->options.getWindowHeight());
    this->_instance = this;
    this->_background.setSize(sf::Vector2f(windowSize.x * 5, windowSize.y * 5));
    this->_background.setOrigin(sf::Vector2f(windowSize.x * 5 / 2, windowSize.y * 5 / 2));
    this->_background.setFillColor(sf::Color(0, 0, 0, 0));
    this->_background.setPosition(sf::Vector2f(windowSize.x / 2, windowSize.y / 2));

    this->_levelInstance = rtype::client::Level_1::getInstance();
    if (!this->_levelInstance)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->_player = std::make_unique<rtype::actor::Player>("", 0);

    if (!this->_player)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    if (engine->options.music.getMusicStream().getStatus() == sf::SoundSource::Stopped || engine->options.music.getMusicPath() != MAINMENU_MUSIC_PATH)
        engine->options.music.changeMusic(CONGRATULATION_MUSIC);

    this->_player->setPosition(sf::Vector2f(-10000, -10000));
    rtype::actor::Player *playerPtr = dynamic_cast<rtype::actor::Player *>(this->_player.get());
    if (!playerPtr)
        throw rtype::settings::Errors("Can't dynamic cast player", "MemoryError:");
    playerPtr->setPlayerColor(engine->gameDatas.getPlayerColor());

    this->_isBackgroundDisplayed = false;
    this->_isTextDisplayed = false;
    this->_startedTime.restart();

    this->_actualFocusEventID = -1;

    this->createTexts(engine);
    this->createButtons(engine);

    std::cout << "EndingScreen class initialized." << std::endl;
}

void rtype::client::EndingScreen::initPlayerPosition(sf::Vector2f position)
{
    this->_player->setPosition(position);
    this->_savePlayerPos = position;
}

void rtype::client::EndingScreen::initInput()
{

}


void rtype::client::EndingScreen::loadEnemies()
{
    // Nothing
}

void rtype::client::EndingScreen::cleanup()
{
    std::cout << "Cleaning the EndingScreen class..." << std::endl;
    releaseInstance();
    std::cout << "EndingScreen class cleaned up." << std::endl;
}

void rtype::client::EndingScreen::pause()
{
}

void rtype::client::EndingScreen::resume()
{
}

void rtype::client::EndingScreen::networkReceiver(rtype::engine::Engine* engine)
{
    this->_levelInstance->networkReceiver(engine);
}

void rtype::client::EndingScreen::networkSender(rtype::engine::Engine* engine)
{
    this->_levelInstance->networkSender(engine);
}

void rtype::client::EndingScreen::processInput(sf::Event event, rtype::engine::Engine* engine)
{
    int newId = 0;

    if (!_isTextDisplayed)
        return;

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

void rtype::client::EndingScreen::update(rtype::engine::Engine* engine)
{
    float timePercent = ((100 * this->_startedTime.getElapsedTime().asSeconds()) / 5) / 100;
    float timeTextPercent = ((100 * this->_startedTime.getElapsedTime().asSeconds()) / 3) / 100;

    sf::Color backgroundColor = this->_background.getFillColor();

    if (timePercent <= 1 && !this->_isBackgroundDisplayed) {
        backgroundColor.a = std::lerp(0, 255, timePercent);
        this->_background.setFillColor(backgroundColor);
        this->_player->setPosition(sf::Vector2f(std::lerp(this->_savePlayerPos.x, engine->options.getWindowWidth() / 2, timePercent), std::lerp(this->_savePlayerPos.y, engine->options.getWindowHeight() / 2 + (engine->options.getWindowHeight() * 20 / 100), timePercent)));
    }
    else {
        if (!this->_isBackgroundDisplayed) {
            this->_isBackgroundDisplayed = true;
            this->_startedTime.restart();
            return;
        }
    }

    if (this->_isBackgroundDisplayed) {
        if (timeTextPercent <= 1 && !this->_isTextDisplayed) {
            sf::Color color = this->_congratulationText->getColor();
            color.a = std::lerp(0, 255, timePercent);

            this->_congratulationText->setColor(color);
            this->_congratulationBar.setFillColor(color);
            this->_scoreText->setColor(color);
            this->_scoreSectionText->setColor(color);
            this->_goToMenu->setColor(color);
        }
        else {
            if (!this->_isTextDisplayed) {
                this->_isTextDisplayed = true;
            }
        }
    }

    this->_player->update(engine);
    this->_levelInstance->update(engine); 
}

void rtype::client::EndingScreen::render(rtype::engine::Engine* engine)
{
    engine->window.clear();

    this->_levelInstance->renderLevel(engine);

    engine->window.draw(this->_background);

    this->_scoreSectionText->draw(engine);
    this->_scoreText->draw(engine);

    this->_congratulationText->draw(engine);
    engine->window.draw(this->_congratulationBar);

    if (_isTextDisplayed)
        this->_goToMenu->draw(engine);

    this->_player->draw(engine);

    engine->window.display();
}

void rtype::client::EndingScreen::updateObjectsPosition(sf::Vector2f deltaSize)
{
    this->_background.move(deltaSize);
}

void rtype::client::EndingScreen::createTexts(rtype::engine::Engine* engine)
{
    sf::Vector2f windowSize(engine->options.getWindowWidth(), engine->options.getWindowHeight());
    int score = engine->gameDatas.getScore();

    this->_congratulationText = std::make_unique<rtype::client::utilities::CustomText>("CONGRATULATIONS", FONT_PATH, 112, sf::Color::White, sf::Vector2f(windowSize.x / 2, windowSize.y * 10 / 100));
    if (!this->_congratulationText)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->_congratulationText->setPosition({ (float)windowSize.x * 50 / 100, ((float)windowSize.y * 20 / 100) - this->_congratulationText->getGlobalBounds().height });
    this->_congratulationText->setColor(sf::Color(255, 255, 255, 0));
    this->_congratulationBar.setSize({ (float)windowSize.x * 50 / 100, 10 });
    this->_congratulationBar.setOrigin(_congratulationBar.getGlobalBounds().width / 2, _congratulationBar.getGlobalBounds().height / 2);
    this->_congratulationBar.setFillColor(sf::Color(255,255,255,0));
    this->_congratulationBar.setPosition((float)windowSize.x * 50 / 100, (float)windowSize.y * 20 / 100);

    this->_scoreSectionText = std::make_unique<rtype::client::utilities::CustomText>("YOUR SCORE:", FONT_PATH, 75, sf::Color::White, sf::Vector2f(windowSize.x / 2, windowSize.y / 2));

    if (!this->_scoreSectionText)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->_scoreSectionText->setPosition({ (float)windowSize.x * 50 / 100, ((float)windowSize.y * 50 / 100) - this->_scoreSectionText->getGlobalBounds().height });

    this->_scoreSectionText->setColor(sf::Color(255, 255, 255, 0));

    this->_scoreText = std::make_unique<rtype::client::utilities::CustomText>(std::to_string(score), FONT_PATH, 75, sf::Color::White, sf::Vector2f((float)windowSize.x / 2, (float)windowSize.y * 10 / 100));

    if (!this->_scoreText)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->_scoreText->setColor(sf::Color(255, 255, 255, 0));
    this->_scoreText->setPosition({ (float)windowSize.x * 50 / 100, ((float)windowSize.y * 50 / 100) + this->_scoreText->getGlobalBounds().height });
}

void rtype::client::EndingScreen::createButtons(rtype::engine::Engine* engine)
{
    sf::Vector2f windowSize(engine->options.getWindowWidth(), engine->options.getWindowHeight());

    this->_goToMenu = std::make_unique<rtype::client::utilities::CustomButton>("RETURN TO MENU", sf::Vector2f(windowSize.x / 2, windowSize.y - (windowSize.y * 10 / 100)), 75.0f, &backToFirstMenu, 1);
    if (!this->_goToMenu)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");
    this->_goToMenu->setColor(sf::Color(255, 255, 255, 0));
}