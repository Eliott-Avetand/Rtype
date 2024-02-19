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
//  File:            TransitionScreen.cpp                                            //
//  Description:     TransitionScreen Class Definition                               //
//                                                                                   //
// ********************************************************************************* //

#include "TransitionScreen.hpp"
#include "MainMenu.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

rtype::client::TransitionScreen* rtype::client::TransitionScreen::_instance{ nullptr };
std::mutex rtype::client::TransitionScreen::_mutex;

void rtype::client::TransitionScreen::init(rtype::engine::Engine* engine)
{
    std::cout << "Initializing the TransitionScreen class..." << std::endl;

    sf::Vector2f windowSize(engine->options.getWindowWidth(), engine->options.getWindowHeight());
    this->_instance = this;
    this->_background.setSize(sf::Vector2f(windowSize.x * 2, windowSize.y * 2));
    this->_background.setOrigin(sf::Vector2f(windowSize.x * 2 / 2, windowSize.y * 2 / 2));
    this->_background.setFillColor(sf::Color(0, 0, 0, 0));
    this->_background.setPosition(sf::Vector2f(windowSize.x / 2, windowSize.y / 2));

    engine->gameDatas.setActualLevel(engine->gameDatas.getActualLevel() + 1);

    this->_levelInstance = rtype::client::Level_1::getInstance();
    if (!this->_levelInstance)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    std::cout << "TransitionScreen class initialized." << std::endl;
}


void rtype::client::TransitionScreen::initInput()
{

}


void rtype::client::TransitionScreen::loadEnemies()
{
    // Nothing
}

void rtype::client::TransitionScreen::cleanup()
{
    std::cout << "Cleaning the TransitionScreen class..." << std::endl;
    releaseInstance();
    std::cout << "TransitionScreen class cleaned up." << std::endl;
}

void rtype::client::TransitionScreen::pause()
{
}

void rtype::client::TransitionScreen::resume()
{
}

void rtype::client::TransitionScreen::networkReceiver(rtype::engine::Engine* engine)
{
    this->_levelInstance->networkReceiver(engine);
}

void rtype::client::TransitionScreen::networkSender(rtype::engine::Engine* engine)
{
    this->_levelInstance->networkSender(engine);
}

void rtype::client::TransitionScreen::processInput(sf::Event event, rtype::engine::Engine* engine)
{
    //this->_levelInstance->processInput(event, engine);
}

void rtype::client::TransitionScreen::update(rtype::engine::Engine* engine)
{
    sf::Time time = this->_backgroundClock.getElapsedTime();
    float milliseconds = time.asMilliseconds();

    sf::Color backgroundColor = this->_background.getFillColor();

    if (milliseconds > 20) {
        backgroundColor.a += 5;
        if (backgroundColor.a >= 255)
            backgroundColor.a = 255;
        this->_background.setFillColor(backgroundColor);
        this->_backgroundClock.restart();
    }

    if (backgroundColor.a >= 255) {
        try {
            boost::property_tree::ptree root;
            std::cout << "data/level_" + std::to_string(engine->gameDatas.getActualLevel()) + "/level.json" << std::endl;
            boost::property_tree::read_json("data/level_" + std::to_string(engine->gameDatas.getActualLevel()) + "/level.json", root);
            engine->changeState(rtype::client::LoadingScreen::getInstance());
        }
        catch (std::exception const& err) {
            engine->changeState(rtype::client::MainMenu::getInstance());
        }
    }

    this->_levelInstance->update(engine);
}

void rtype::client::TransitionScreen::render(rtype::engine::Engine* engine)
{
    engine->window.clear();

    this->_levelInstance->renderLevel(engine);

    engine->window.draw(this->_background);

    engine->window.display();
}

void rtype::client::TransitionScreen::updateObjectsPosition(sf::Vector2f deltaSize)
{
    this->_background.move(deltaSize);
}