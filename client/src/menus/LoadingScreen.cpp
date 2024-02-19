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
//  File:            LoadingScreen.cpp                                               //
//  Description:     File for the LoadingScreen Class                                //
//                                                                                   //
// ********************************************************************************* //

#include "LoadingScreen.hpp"

rtype::client::LoadingScreen *rtype::client::LoadingScreen::_loadingInstance{nullptr};
std::mutex rtype::client::LoadingScreen::_mutex;

void rtype::client::LoadingScreen::init(rtype::engine::Engine* engine)
{
    std::cout << "Initializing the MainMenu class..." << std::endl;

    _loadingInstance = this;

    createTitleLevel(engine);
    createLoadingText(engine);

    if (engine->options.music.getMusicStream().getStatus() == sf::SoundSource::Playing && engine->options.music.isMusicActive() == true)
        engine->options.music.getMusicStream().stop();

    std::cout << "MainMenu class initialized." << std::endl;
}

void rtype::client::LoadingScreen::cleanup()
{
    std::cout << "Cleaning the MainMenu class..." << std::endl;
    releaseInstance();
    std::cout << "MainMenu class cleaned up." << std::endl;
}

void rtype::client::LoadingScreen::pause()
{
}

void rtype::client::LoadingScreen::resume()
{
}

void rtype::client::LoadingScreen::networkReceiver(rtype::engine::Engine *engine)
{
}

void rtype::client::LoadingScreen::networkSender(rtype::engine::Engine *engine)
{
}

void rtype::client::LoadingScreen::processInput(sf::Event event, rtype::engine::Engine* engine)
{
    // Void
}

void rtype::client::LoadingScreen::update(rtype::engine::Engine* engine)
{
    sf::Time time = this->_loadingTextClock.getElapsedTime();
    float milliseconds = time.asMilliseconds();

    if (minimalLoading())
        engine->changeState(rtype::client::Level_1::getInstance());

    if (milliseconds > 300) {
        if (this->_loadingText->getText() == "LOADING")
            this->_loadingText->setText("LOADING .");
        else if (this->_loadingText->getText() == "LOADING .")
            this->_loadingText->setText("LOADING ..");
        else if (this->_loadingText->getText() == "LOADING ..")
            this->_loadingText->setText("LOADING ...");
        else if (this->_loadingText->getText() == "LOADING ...")
            this->_loadingText->setText("LOADING");

        this->_loadingText->setOrigin(0, this->_loadingText->getGlobalBounds().height / 2);
        this->_loadingTextClock.restart();
    }

}

void rtype::client::LoadingScreen::render(rtype::engine::Engine* engine)
{
    engine->window.clear();

    displayTitleLevel(engine);
    displayLoadingText(engine);

    engine->window.display();
}

void rtype::client::LoadingScreen::updateObjectsPosition(sf::Vector2f deltaSize)
{
    this->_levelTitle->getTextObject().move(deltaSize);
    this->_loadingText->getTextObject().move(deltaSize);
    this->_rectangleShape.move(deltaSize);
}

bool rtype::client::LoadingScreen::minimalLoading(void)
{
    sf::Time time = _sceneClock.getElapsedTime();
    float seconds = time.asSeconds();

    if (seconds > 5)
        return true;
    return false;
}

void rtype::client::LoadingScreen::createTitleLevel(rtype::engine::Engine* engine)
{
    float windowWidth = (float)engine->options.getWindowWidth();
    float windowHeight = engine->options.getWindowHeight();

    _levelTitle = std::make_unique<rtype::client::utilities::CustomText>("Level " + std::to_string(engine->gameDatas.getActualLevel()), FONT_PATH, 75, sf::Color::White, sf::Vector2f(50.0f, 50.0f));

    if (!_levelTitle)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    _levelTitle->setPosition({windowWidth * 50 / 100, (windowHeight * 30 / 100) - _levelTitle->getGlobalBounds().height});

    _rectangleShape.setSize({ windowWidth * 40 / 100, 10 });
    _rectangleShape.setOrigin(_rectangleShape.getGlobalBounds().width / 2, _rectangleShape.getGlobalBounds().height / 2);
    _rectangleShape.setFillColor(sf::Color::White);
    _rectangleShape.setPosition(windowWidth * 50 / 100, windowHeight * 30 / 100);
}

void rtype::client::LoadingScreen::displayTitleLevel(rtype::engine::Engine* engine)
{
    _levelTitle->draw(engine);
    engine->window.draw(_rectangleShape);
}

void rtype::client::LoadingScreen::createLoadingText(rtype::engine::Engine* engine)
{
    float windowWidth = (float)engine->options.getWindowWidth();
    float windowHeight = engine->options.getWindowHeight();

    _loadingText = std::make_unique<rtype::client::utilities::CustomText>("LOADING ...", FONT_PATH, 30, sf::Color::White, sf::Vector2f(50.0f, 50.0f));

    if (!_loadingText)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    _loadingText->setPosition({windowWidth - _loadingText->getGlobalBounds().width - 10, windowHeight - _loadingText->getGlobalBounds().height / 2 - 10});
    this->_loadingText->setOrigin(0, this->_loadingText->getGlobalBounds().height / 2);

}

void rtype::client::LoadingScreen::displayLoadingText(rtype::engine::Engine* engine)
{
    _loadingText->draw(engine);
}