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
//  File:            BeginingScreen.cpp                                              //
//  Description:     File for the BeginingScreen Class                               //
//                                                                                   //
// ********************************************************************************* //
#include "BeginingScreen.hpp"
#include "MainMenu.hpp"

rtype::client::BeginingScreen*rtype::client::BeginingScreen::_instance{nullptr};
std::mutex rtype::client::BeginingScreen::_mutex;

void rtype::client::BeginingScreen::init(rtype::engine::Engine* engine)
{
    std::cout << "Initializing the MainMenu class..." << std::endl;

    _instance = this;

    engine->options.music.toggleMusic(false);

    this->_animationIsFinish = false;
    this->_loustikSprite = std::make_unique<rtype::client::utilities::AnimatedSprite>(LOUSTIK_SPRITESHEET, sf::Vector2f(engine->window.getSize().x / 2, engine->window.getSize().y / 2), sf::Vector2f(546, 553), rtype::client::utilities::AnimatedSprite::HORIZONTAL, sf::Vector2i(0, 0), sf::Vector2i(546 * 22, 0), 0.15f, false, rtype::client::utilities::AnimatedSprite::RIGHT);

    if (!this->_loustikSprite)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->_loustikSprite->setScale(0.5, 0.5);
    engine->options.music.changePitch(1);


    createLoadingText(engine);

    std::cout << "MainMenu class initialized." << std::endl;
}

void rtype::client::BeginingScreen::cleanup()
{
    std::cout << "Cleaning the MainMenu class..." << std::endl;
    releaseInstance();
    std::cout << "MainMenu class cleaned up." << std::endl;
}

void rtype::client::BeginingScreen::pause()
{
}

void rtype::client::BeginingScreen::resume()
{
}

void rtype::client::BeginingScreen::networkReceiver(rtype::engine::Engine *engine)
{
}

void rtype::client::BeginingScreen::networkSender(rtype::engine::Engine *engine)
{
}

void rtype::client::BeginingScreen::processInput(sf::Event event, rtype::engine::Engine* engine)
{
    // Void
}

void rtype::client::BeginingScreen::update(rtype::engine::Engine* engine)
{
    sf::Time time = this->_loadingTextClock.getElapsedTime();
    float milliseconds = time.asMilliseconds();

    this->_loustikSprite->updateAnimation();

    if (this->_loustikSprite->isAnimationFinish() && !this->_animationIsFinish) {
        this->_animationIsFinish = true;
        this->_sound.playSound(engine->options.music.isSoundActive());
        this->_sceneClock.restart();
    }

    if (this->_animationIsFinish) {
        if (minimalLoading()) {
            engine->options.music.toggleMusic(true);
            engine->changeState(rtype::client::MainMenu::getInstance());
        }
    }
}

void rtype::client::BeginingScreen::render(rtype::engine::Engine* engine)
{
    engine->window.clear();

    this->_loustikSprite->draw(engine);

    if (this->_animationIsFinish) {
        this->_loadingText->draw(engine);
        engine->window.draw(this->_rectangleShape);
    }

    engine->window.display();
}

void rtype::client::BeginingScreen::updateObjectsPosition(sf::Vector2f deltaSize)
{
    this->_loadingText->getTextObject().move(deltaSize);
    this->_rectangleShape.move(deltaSize);
}

bool rtype::client::BeginingScreen::minimalLoading(void)
{
    sf::Time time = _sceneClock.getElapsedTime();
    float seconds = time.asSeconds();

    if (seconds > 2)
        return true;
    return false;
}

void rtype::client::BeginingScreen::createLoadingText(rtype::engine::Engine* engine)
{
    float windowWidth = (float)engine->options.getWindowWidth();
    float windowHeight = engine->options.getWindowHeight();

    _loadingText = std::make_unique<rtype::client::utilities::CustomText>("LOUSTIK STUDIO", FONT_PATH, 70, sf::Color::White, sf::Vector2f(50.0f, 50.0f));

    if (!_loadingText)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    _loadingText->setPosition({windowWidth / 2, windowHeight / 2 + (windowHeight * 20 / 100)});

    _rectangleShape.setSize({ windowWidth * 40 / 100, 10 });
    _rectangleShape.setOrigin(_rectangleShape.getGlobalBounds().width / 2, _rectangleShape.getGlobalBounds().height / 2);
    _rectangleShape.setFillColor(sf::Color::White);
    _rectangleShape.setPosition(windowWidth / 2, windowHeight / 2 + (windowHeight * 20 / 100) + _loadingText->getGlobalBounds().height);
}