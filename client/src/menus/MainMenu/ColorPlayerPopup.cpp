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

#include "ColorPlayerPopup.hpp"
#include "MainMenu.hpp"

#include "Errors.hpp"

rtype::client::ColorPlayerPopup* rtype::client::ColorPlayerPopup::_instance{ nullptr };
std::mutex rtype::client::ColorPlayerPopup::_mutex;

void popColorPopup(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data)
{
    engine->gameDatas.setPlayerColor(data.color);
    engine->popState();
}

void applyAndQuitColorPopup(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data)
{
    engine->popState();
}

void updateRedColor(rtype::engine::Engine* engine, rtype::sliderutils::CallbackData& data)
{
    sf::Color color = engine->gameDatas.getPlayerColor();

    color.r = 255 * data.percentage / 100;

    engine->gameDatas.setPlayerColor(color);
}

void updateGreenColor(rtype::engine::Engine* engine, rtype::sliderutils::CallbackData& data)
{
    sf::Color color = engine->gameDatas.getPlayerColor();

    color.g = 255 * data.percentage / 100;

    engine->gameDatas.setPlayerColor(color);
}

void updateBlueColor(rtype::engine::Engine* engine, rtype::sliderutils::CallbackData& data)
{
    sf::Color color = engine->gameDatas.getPlayerColor();

    color.b = 255 * data.percentage / 100;

    engine->gameDatas.setPlayerColor(color);
}

void rtype::client::ColorPlayerPopup::init(rtype::engine::Engine* engine)
{
    std::cout << "Initializing the MainMenu class..." << std::endl;

    sf::Vector2f windowSize((float)engine->options.getWindowWidth(), (float)engine->options.getWindowHeight());

    _instance = this;

    this->_oldColor = engine->gameDatas.getPlayerColor();
    this->_background.setFillColor(sf::Color(20, 20, 20, 192));
    this->_background.setOutlineThickness(5);
    this->_background.setOutlineColor(sf::Color::White);
    this->_background.setSize(sf::Vector2(windowSize.x * 50 / 100, windowSize.y * 50 /100));
    this->_background.setOrigin(sf::Vector2f(this->_background.getSize().x / 2, this->_background.getSize().y / 2));
    this->_background.setPosition(sf::Vector2f(windowSize.x / 2, windowSize.y / 2));

    this->createSliders(engine);
    this->createTexts(engine);
    this->createButtons(engine);

    this->_player.setPosition(sf::Vector2f(windowSize.x * 50 / 100, windowSize.y * 30 / 100));
    this->_player.update(engine);
   
    _actualFocusEventID = 0;
}

void rtype::client::ColorPlayerPopup::cleanup()
{
    std::cout << "Cleaning the MainMenu class..." << std::endl;
    this->_allButtons.clear();
    this->releaseInstance();
    std::cout << "MainMenu class cleaned up." << std::endl;
}

void rtype::client::ColorPlayerPopup::pause()
{
}

void rtype::client::ColorPlayerPopup::resume()
{
}

void rtype::client::ColorPlayerPopup::networkReceiver(rtype::engine::Engine* engine)
{
}

void rtype::client::ColorPlayerPopup::networkSender(rtype::engine::Engine* engine)
{
}

void rtype::client::ColorPlayerPopup::processInput(sf::Event event, rtype::engine::Engine* engine)
{
    int newId = 0;

    if (sf::Joystick::isConnected(0)) {
        for (unsigned int i = 0; i < _allButtons.size(); i++) {
            if (_allButtons[i].getId() == _actualFocusEventID)
                newId = _allButtons[i].eventUpdateJoystick(event, engine);
        }
        for (unsigned int i = 0; i < _allSliders.size(); i++) {
            if (_allSliders[i].getId() == _actualFocusEventID)
                newId = _allSliders[i].eventUpdateJoystick(event, engine);
        }

    }
    else {
        for (unsigned int i = 0; i < _allButtons.size(); i++)
            newId = _allButtons[i].eventUpdate(event, engine);
        for (unsigned int i = 0; i < _allSliders.size(); i++)
            newId = _allSliders[i].eventUpdate(event, engine);
    }

    if (newId != -1)
        this->_actualFocusEventID = newId;

    if (newId == 0)
        this->_actualFocusEventID = 1;
}

void rtype::client::ColorPlayerPopup::update(rtype::engine::Engine* engine)
{
    rtype::client::MainMenu* menu = rtype::client::MainMenu::getInstance();

    if (menu)
        menu->update(engine);

    for (unsigned int i = 0; i < _allSliders.size(); i++) {
        if (_allSliders[i].getId() == _actualFocusEventID)
            _allSliders[i].update(engine);
    }
    for (unsigned int i = 0; i < _allButtons.size(); i++) {
        if (_allButtons[i].getId() == _actualFocusEventID)
            _allButtons[i].update(engine);

    }

    this->_allTexts[1]->setText(std::to_string(engine->gameDatas.getPlayerColor().r));
    this->_allTexts[3]->setText(std::to_string(engine->gameDatas.getPlayerColor().g));
    this->_allTexts[5]->setText(std::to_string(engine->gameDatas.getPlayerColor().b));
    this->_player.setPlayerColor(engine->gameDatas.getPlayerColor());
}

void rtype::client::ColorPlayerPopup::render(rtype::engine::Engine* engine)
{
    engine->window.clear();

    rtype::client::MainMenu* menu = rtype::client::MainMenu::getInstance();

    if (menu)
        menu->renderLevel(engine);

    engine->window.draw(this->_background);

    for (int i = 0; i < this->_allSliders.size(); i++)
        this->_allSliders[i].draw(engine);

    for (int i = 0; i < this->_allTexts.size(); i++)
        this->_allTexts[i]->draw(engine);

    for (int i = 0; i < this->_allButtons.size(); i++)
        this->_allButtons[i].draw(engine);

    this->_player.draw(engine);

    engine->window.display();
}

void rtype::client::ColorPlayerPopup::updateObjectsPosition(sf::Vector2f deltaSize)
{
    //this->_copyright->move(deltaSize);
    //for (int i = 0; i < this->_allButtons.size(); i++)
    //    this->_allButtons[i].move(deltaSize);
}

void rtype::client::ColorPlayerPopup::createSliders(rtype::engine::Engine* engine)
{
    sf::Color color = engine->gameDatas.getPlayerColor();

    sf::Vector2f windowSize((float)engine->options.getWindowWidth(), engine->options.getWindowHeight());
    std::vector<rtype::sliderutils::callback_t> sliderCallback = { &updateRedColor, &updateGreenColor, &updateBlueColor };
    std::vector<float> percentages = { (float)(color.r / 255 * 100), (float)(color.g / 255 * 100), (float)(color.b / 255 * 100)};
    std::vector<int> id = { 1, 2, 3 };
    std::vector<sf::Vector2f> sliderPosition = {
        {windowSize.x / 2, windowSize.y * 40 / 100},
        {windowSize.x / 2, windowSize.y * 50 / 100},
        {windowSize.x / 2, windowSize.y * 60 / 100}
    };

    for (unsigned int i = 0; i < 3; i++)
        _allSliders.push_back(rtype::client::utilities::CustomSlider(sliderPosition[i], sf::Vector2f(3, 1), percentages[i], sliderCallback[i], id[i]));

    this->_allSliders[0].setDownElements(2);
    this->_allSliders[1].setUpElements(1);
    this->_allSliders[1].setDownElements(3);
    this->_allSliders[2].setUpElements(2);
    this->_allSliders[2].setDownElements(5);
}

void rtype::client::ColorPlayerPopup::createButtons(rtype::engine::Engine* engine)
{
    sf::FloatRect bounds = this->_background.getGlobalBounds();
    std::vector<rtype::buttonutils::callback_t> callback = { &popColorPopup, &applyAndQuitColorPopup };
    std::vector<std::string> text = { "X CLOSE", "APPLY >"};
    std::vector<sf::Vector2f> position = {
        {bounds.left + 90, bounds.top + bounds.height - 20},
        {bounds.left + bounds.width - 90 , bounds.top + bounds.height - 20},
    };

    rtype::buttonutils::CallbackData data;
    data.color = this->_oldColor;

    std::vector<int> id = { 4, 5 };

    for (unsigned int i = 0; i < 2; i++) {
        _allButtons.push_back(rtype::client::utilities::CustomButton(text[i], position[i], 20.0f, callback[i], id[i]));
        _allButtons[i].setAdditionalData(data);
    }

    _allButtons[0].setUpElements(3);
    _allButtons[0].setRightElements(5);

    _allButtons[1].setUpElements(3);
    _allButtons[1].setLeftElements(4);
}

void rtype::client::ColorPlayerPopup::createTexts(rtype::engine::Engine* engine)
{
    sf::Color color = engine->gameDatas.getPlayerColor();

    sf::Vector2f windowSize((float)engine->options.getWindowWidth(), engine->options.getWindowHeight());
    std::vector<std::string> text = { "R", std::to_string((int)(color.r)), "G", std::to_string((int)(color.g)), "B", std::to_string((int)(color.b)) };
    std::vector<sf::Vector2f> position = {
        {windowSize.x * 30 / 100, windowSize.y * 40 / 100},
        {windowSize.x * 70 / 100, windowSize.y * 40 / 100},
        {windowSize.x * 30 / 100, windowSize.y * 50 / 100},
        {windowSize.x * 70 / 100, windowSize.y * 50 / 100},
        {windowSize.x * 30 / 100, windowSize.y * 60 / 100},
        {windowSize.x * 70 / 100, windowSize.y * 60 / 100},
    };

    for (int i = 0; i < 6; i++) {
        this->_allTexts.push_back(std::make_unique<rtype::client::utilities::CustomText>(text[i], FONT_PATH, 30, sf::Color::White, position[i]));

        if (!this->_allTexts.back())
            throw rtype::settings::Errors("Not enough memories", "MemoryError:");
    }
}
