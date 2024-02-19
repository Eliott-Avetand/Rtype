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
#include "PauseMenuOptions.hpp"
#include "MainMenuOptions.hpp"

rtype::client::PauseMenuOptions *rtype::client::PauseMenuOptions::_pauseMenuOptionsInstance{nullptr};
std::mutex rtype::client::PauseMenuOptions::_mutex;

void backToPauseMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData& data)
{
    engine->popState();
}

void rtype::client::PauseMenuOptions::init(rtype::engine::Engine* engine)
{
    std::cout << "Initializing the PauseMenuOptions class..." << std::endl;

    _pauseMenuOptionsInstance = this;
    this->_levelInstance = rtype::client::Level_1::getInstance();

    if (!this->_levelInstance)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->createButtons(engine);
    this->createBackground(engine);
    this->createTitleText(engine);
    this->createAllOptionsTexts(engine);
    this->createAllCheckbox(engine);
    this->createAllSliders(engine);

    _actualFocusEventID = 0;

    std::cout << "PauseMenuOptions class initialized." << std::endl;
}

void rtype::client::PauseMenuOptions::cleanup()
{
    std::cout << "Cleaning the PauseMenuOptions class..." << std::endl;
    releaseInstance();
    std::cout << "PauseMenuOptions class cleaned up." << std::endl;
}

void rtype::client::PauseMenuOptions::pause()
{
}

void rtype::client::PauseMenuOptions::resume()
{
}

void rtype::client::PauseMenuOptions::networkReceiver(rtype::engine::Engine* engine)
{
    this->_levelInstance->networkReceiver(engine);
}

void rtype::client::PauseMenuOptions::networkSender(rtype::engine::Engine* engine)
{
    this->_levelInstance->networkSender(engine);
}

void rtype::client::PauseMenuOptions::processInput(sf::Event event, rtype::engine::Engine* engine)
{
    int newId = 0;

    if (sf::Joystick::isConnected(0)) {
        if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == rtype::action::joystickButton::START) {
            engine->popState();
            engine->popState();
        }
        for (unsigned int i = 0; i < _allButtons.size(); i++) {
            if (_allButtons[i].getId() == _actualFocusEventID)
                newId = _allButtons[i].eventUpdateJoystick(event, engine);
        }
        for (unsigned int i = 0; i < _allCheckBox.size(); i++) {
            if (_allCheckBox[i].getId() == _actualFocusEventID)
                newId = _allCheckBox[i].eventUpdateJoystick(event, engine);
        }
        for (unsigned int i = 0; i < _allSliders.size(); i++) {
            if (_allSliders[i].getId() == _actualFocusEventID)
                newId = _allSliders[i].eventUpdateJoystick(event, engine);
        }

    }
    else {
        for (unsigned int i = 0; i < _allButtons.size(); i++)
            newId = _allButtons[i].eventUpdate(event, engine);
        for (unsigned int i = 0; i < _allCheckBox.size(); i++)
            newId = _allCheckBox[i].eventUpdate(event, engine);
        for (unsigned int i = 0; i < _allSliders.size(); i++)
            newId = _allSliders[i].eventUpdate(event, engine);
    }

    if (newId != -1)
        this->_actualFocusEventID = newId;

    if (newId == 0)
        this->_actualFocusEventID = 1;
}

void rtype::client::PauseMenuOptions::update(rtype::engine::Engine* engine)
{

    for (unsigned int i = 0; i < _allSliders.size(); i++) {
        if (_allSliders[i].getId() == _actualFocusEventID)
            _allSliders[i].update(engine);
    }
    for (unsigned int i = 0; i < _allButtons.size(); i++) {
        if (_allButtons[i].getId() == _actualFocusEventID)
            _allButtons[i].update(engine);

    }
    for (unsigned int i = 0; i < _allCheckBox.size(); i++) {
        if (_allCheckBox[i].getId() == _actualFocusEventID)
            _allCheckBox[i].update(engine);
    }

    if (engine->getClient()->getIsOnline())
        _levelInstance->update(engine);
}

void rtype::client::PauseMenuOptions::render(rtype::engine::Engine* engine)
{
    engine->window.clear();

    this->_levelInstance->renderLevel(engine);

    engine->window.draw(this->_background);
    this->displayTitleText(engine);
    this->displayButtons(engine);
    this->displayAllCheckBox(engine);
    this->displayAllOptionsTexts(engine);
    this->displayAllSliders(engine);

    engine->window.display();
}

void rtype::client::PauseMenuOptions::updateObjectsPosition(sf::Vector2f deltaSize)
{
    this->_titleScene->move(deltaSize);
    for (unsigned int i = 0; i < this->_allTexts.size(); i++)
        this->_allTexts[i]->move(deltaSize);
    for (unsigned int i = 0; i < this->_allButtons.size(); i++)
        this->_allButtons[i].move(deltaSize);
    for (unsigned int i = 0; i < this->_allCheckBox.size(); i++)
        this->_allCheckBox[i].move(deltaSize);
    for (unsigned int i = 0; i < this->_allSliders.size(); i++)
        this->_allSliders[i].move(deltaSize);
    this->_background.move(deltaSize);
}

void rtype::client::PauseMenuOptions::createAllCheckbox(rtype::engine::Engine* engine)
{
    sf::Vector2f windowSize((float)engine->options.getWindowWidth(), engine->options.getWindowHeight());
    std::vector<rtype::buttonutils::callback_t> callback = { &toggleMusic, &toggleSound, &cbToggleFullscreen };
    std::vector<bool> isActive = { engine->options.music.isMusicActive(), engine->options.music.isSoundActive(), engine->isFullscreen() };
    std::vector<int> id = { 3, 4, 5 };
    std::vector<sf::Vector2f> position = {
        {windowSize.x * 85 / 100, windowSize.y * 30 / 100},
        {windowSize.x * 85 / 100, windowSize.y * 50 / 100},
        {windowSize.x * 85 / 100, windowSize.y * 70 / 100}
    };

    for (unsigned int i = 0; i < callback.size(); i++)
        _allCheckBox.push_back(rtype::client::utilities::CustomCheckBox(position[i], 0.35, callback[i], isActive[i], id[i]));

    _allCheckBox[0].setLeftElements(1);
    _allCheckBox[0].setDownElements(4);
    _allCheckBox[1].setUpElements(3);
    _allCheckBox[1].setLeftElements(2);
    _allCheckBox[1].setDownElements(5);
    _allCheckBox[2].setUpElements(4);
    _allCheckBox[2].setDownElements(6);
}

void rtype::client::PauseMenuOptions::displayAllCheckBox(rtype::engine::Engine* engine)
{
    for (unsigned int i = 0; i < this->_allCheckBox.size(); i++)
        this->_allCheckBox[i].draw(engine);
}

void rtype::client::PauseMenuOptions::createAllOptionsTexts(rtype::engine::Engine* engine)
{
    sf::Vector2f windowSize((float)engine->options.getWindowWidth(), engine->options.getWindowHeight());
    std::vector<std::string> text = { "MUSIC", "SOUND", "FULLSCREEN" };
    std::vector<sf::Vector2f> position = {
        {windowSize.x * 15 / 100, windowSize.y * 30 / 100},
        {windowSize.x * 15 / 100, windowSize.y * 50 / 100},
        {windowSize.x * 15 / 100, windowSize.y * 70 / 100}
    };

    for (unsigned int i = 0; i < text.size(); i++) {
        this->_allTexts.push_back(std::make_unique<rtype::client::utilities::CustomText>(text[i], FONT_PATH, 65, sf::Color::White, position[i]));
        if (!this->_allTexts[i])
            throw rtype::settings::Errors("Not enough memories", "MemoryError:");
    }
    this->_allTexts[2]->getTextObject().move(sf::Vector2f(this->_allTexts[2]->getWidth() / 4, 0));
}

void rtype::client::PauseMenuOptions::displayAllOptionsTexts(rtype::engine::Engine* engine)
{
    for (unsigned int i = 0; i < this->_allTexts.size(); i++)
        this->_allTexts[i]->draw(engine);
}

void rtype::client::PauseMenuOptions::createBackground(rtype::engine::Engine* engine)
{
    this->_background.setSize(sf::Vector2f(engine->options.getWindowWidth() * 2, engine->options.getWindowHeight() * 2));
    this->_background.setOrigin(sf::Vector2f(engine->options.getWindowWidth() / 2, engine->options.getWindowHeight() / 2));
    this->_background.setPosition(sf::Vector2f(engine->options.getWindowWidth() / 2, engine->options.getWindowHeight() / 2));
    this->_background.setFillColor(sf::Color(0, 0, 0, 160));
}

void rtype::client::PauseMenuOptions::createButtons(rtype::engine::Engine* engine)
{
    float windowWidth = (float)engine->options.getWindowWidth();
    float windowHeight = engine->options.getWindowHeight();

    std::vector<std::string> buttonText = { "BACK" };
    std::vector<int> buttonSize = { 65 };
    std::vector<rtype::buttonutils::callback_t> buttonCallback = { &backToPauseMenu };
    std::vector<int> id = { 6 };

    for (unsigned int i = 0; i < 1; i++)
        _allButtons.push_back(rtype::client::utilities::CustomButton(buttonText[i], { 0, 0 }, buttonSize[i], buttonCallback[i], id[i]));

    std::vector<sf::Vector2f> buttonPosition = { {windowWidth / 2, windowHeight - _allButtons[0].getHeight() / 2 - windowHeight * 10 / 100} };

    for (unsigned int i = 0; i < 1; i++)
        _allButtons[i].setPosition(buttonPosition[i]);

    _allButtons[0].setUpElements(2);
}

void rtype::client::PauseMenuOptions::displayButtons(rtype::engine::Engine* engine)
{
    for (unsigned int i = 0; i < this->_allButtons.size(); i++)
        this->_allButtons[i].draw(engine);
}

void rtype::client::PauseMenuOptions::createTitleText(rtype::engine::Engine* engine)
{
    float windowWidth = (float)engine->options.getWindowWidth();
    float windowHeight = engine->options.getWindowHeight();

    this->_titleScene = std::make_unique<rtype::client::utilities::CustomText>("OPTIONS", FONT_PATH, 85, sf::Color::White, sf::Vector2f(windowWidth / 2, windowHeight * 10 / 100));

    if (!this->_titleScene)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");
}

void rtype::client::PauseMenuOptions::displayTitleText(rtype::engine::Engine* engine)
{
    this->_titleScene->draw(engine);
}

void rtype::client::PauseMenuOptions::createAllSliders(rtype::engine::Engine* engine)
{
    sf::Vector2f windowSize((float)engine->options.getWindowWidth(), engine->options.getWindowHeight());
    std::vector<rtype::sliderutils::callback_t> sliderCallback = { &musicVolumeManagement, &soundVolumeManagement };
    std::vector<float> percentages = { (float)engine->options.music.getMusicVolume(), (float)engine->options.music.getSoundVolume() };
    std::vector<int> id = { 1, 2 };
    std::vector<sf::Vector2f> sliderPosition = {
        {windowSize.x / 2, windowSize.y * 30 / 100},
        {windowSize.x / 2, windowSize.y * 50 / 100}
    };

    for (unsigned int i = 0; i < 2; i++)
        _allSliders.push_back(rtype::client::utilities::CustomSlider(sliderPosition[i], sf::Vector2f(3, 1), percentages[i], sliderCallback[i], id[i]));

    this->_allSliders[0].setDownElements(2);
    this->_allSliders[0].setRightElements(3);
    this->_allSliders[1].setUpElements(1);
    this->_allSliders[1].setRightElements(4);
    this->_allSliders[1].setDownElements(5);
}

void rtype::client::PauseMenuOptions::displayAllSliders(rtype::engine::Engine* engine)
{
    for (unsigned i = 0; i < _allSliders.size(); i++)
        this->_allSliders[i].draw(engine);
}
