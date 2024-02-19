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
//  File:            MainMenuOptions.cpp                                             //
//  Description:     Main Menu Options Scene Class                                   //
//                                                                                   //
// ********************************************************************************* //

#include "MainMenuOptions.hpp"
#include "MainMenu.hpp"
#include "Errors.hpp"

rtype::client::MainMenuOptions *rtype::client::MainMenuOptions::_mainMenuOptions{nullptr};
std::mutex rtype::client::MainMenuOptions::_mutex;

void backToMenu(rtype::engine::Engine *engine, rtype::buttonutils::CallbackData &data)
{
    engine->changeState(rtype::client::MainMenu::getInstance());
}

void musicVolumeManagement(rtype::engine::Engine* engine, rtype::sliderutils::CallbackData& data)
{
    engine->options.music.setMusicVolume(data.percentage);
}

void soundVolumeManagement(rtype::engine::Engine* engine, rtype::sliderutils::CallbackData& data)
{
    engine->options.music.setSoundVolume(data.percentage);
}

void toggleMusic(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    engine->options.music.toggleMusic(!engine->options.music.isMusicActive());
}

void toggleSound(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    engine->options.music.toggleSound(!engine->options.music.isSoundActive());
}

void cbToggleFullscreen(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    engine->toggleFullscreen();
}

void rtype::client::MainMenuOptions::init(rtype::engine::Engine *engine)
{
    std::cout << "Initializing the MainMenuOptions class..." << std::endl;

    _mainMenuOptions = this;

    createButtons(engine);
    createTitleText(engine);
    createAllOptionsTexts(engine);
    createAllCheckbox(engine);
    createAllSliders(engine);

    _actualFocusEventID = 0;

    _background.toggleAsteroidsSpawn(true);
    _background.manageAsteroidsSpawnTime(0.1, 1.5);

    std::cout << "MainMenuOptions class initialized." << std::endl;
}

void rtype::client::MainMenuOptions::cleanup()
{
    std::cout << "Cleaning the MainMenuOptions class..." << std::endl;
    this->_allButtons.clear();
    this->_allCheckBox.clear();
    this->_allSliders.clear();
    this->_allTexts.clear();
    this->releaseInstance();
    std::cout << "MainMenuOptions class cleaned up." << std::endl;
}

void rtype::client::MainMenuOptions::pause()
{
}

void rtype::client::MainMenuOptions::resume()
{
}

void rtype::client::MainMenuOptions::networkReceiver(rtype::engine::Engine *engine)
{
}

void rtype::client::MainMenuOptions::networkSender(rtype::engine::Engine *engine)
{
}

void rtype::client::MainMenuOptions::processInput(sf::Event event, rtype::engine::Engine *engine)
{
    int newId = 0;

    _background.eventUpdate(event, engine);

    if (sf::Joystick::isConnected(0)) {
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

void rtype::client::MainMenuOptions::update(rtype::engine::Engine *engine)
{
    _background.update(engine);

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
}

void rtype::client::MainMenuOptions::render(rtype::engine::Engine *engine)
{
    engine->window.clear();

    this->_background.display(engine);
    this->displayTitleText(engine);
    this->displayButtons(engine);
    this->displayAllCheckBox(engine);
    this->displayAllOptionsTexts(engine);
    this->displayAllSliders(engine);

    engine->window.display();
}

void rtype::client::MainMenuOptions::updateObjectsPosition(sf::Vector2f deltaSize)
{
    for (unsigned int i = 0; i < this->_allTexts.size(); i++)
        this->_allTexts[i]->move(deltaSize);
    for (unsigned int i = 0; i < this->_allButtons.size(); i++)
        this->_allButtons[i].move(deltaSize);
    for (unsigned int i = 0; i < this->_allCheckBox.size(); i++)
        this->_allCheckBox[i].move(deltaSize);
    for (unsigned int i = 0; i < this->_allSliders.size(); i++)
        this->_allSliders[i].move(deltaSize);
}

void rtype::client::MainMenuOptions::createAllCheckbox(rtype::engine::Engine* engine)
{
    sf::Vector2f windowSize((float)engine->options.getWindowWidth(), engine->options.getWindowHeight());
    std::vector<rtype::buttonutils::callback_t> callback = { &toggleMusic, &toggleSound, &cbToggleFullscreen};
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

void rtype::client::MainMenuOptions::displayAllCheckBox(rtype::engine::Engine* engine)
{
    for (unsigned int i = 0; i < this->_allCheckBox.size(); i++)
        this->_allCheckBox[i].draw(engine);
}

void rtype::client::MainMenuOptions::createAllOptionsTexts(rtype::engine::Engine* engine)
{
    sf::Vector2f windowSize((float)engine->options.getWindowWidth(), engine->options.getWindowHeight());
    std::vector<std::string> text = {"MUSIC", "SOUND", "FULLSCREEN"};
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

void rtype::client::MainMenuOptions::displayAllOptionsTexts(rtype::engine::Engine *engine)
{
    for (unsigned int i = 0; i < this->_allTexts.size(); i++)
        _allTexts[i]->draw(engine);
}

void rtype::client::MainMenuOptions::createButtons(rtype::engine::Engine *engine)
{
    sf::Vector2f windowSize((float)engine->options.getWindowWidth(), engine->options.getWindowHeight());

    std::vector<std::string> buttonText = {"BACK"};
    std::vector<rtype::buttonutils::callback_t> buttonCallback = {&backToMenu};
    std::vector<int> id = {6};

    for (unsigned int i = 0; i < buttonText.size(); i++)
        this->_allButtons.push_back(rtype::client::utilities::CustomButton(buttonText[i], {0, 0}, 50, buttonCallback[i], id[i]));

    std::vector<sf::Vector2f> btnPos = {
        {(windowSize.x / 2), windowSize.y - (this->_allButtons[0].getHeight() / 2) - 35}
    };
    for (unsigned int i = 0; i < buttonText.size(); i++)
        this->_allButtons[i].setPosition(btnPos[i]);

    this->_allButtons[0].setUpElements(2);
}

void rtype::client::MainMenuOptions::displayButtons(rtype::engine::Engine *engine)
{
    for (unsigned int i = 0; i < this->_allButtons.size(); i++)
        _allButtons[i].draw(engine);
}

void rtype::client::MainMenuOptions::createTitleText(rtype::engine::Engine *engine)
{
    _titleScene = std::make_unique<rtype::client::utilities::CustomText>("GAME OPTIONS", FONT_PATH, 65, sf::Color::White, sf::Vector2f((float)engine->options.getWindowWidth() / 2, 50));
    if (!_titleScene)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");
}

void rtype::client::MainMenuOptions::displayTitleText(rtype::engine::Engine *engine)
{
    _titleScene->draw(engine);
}

void rtype::client::MainMenuOptions::createAllSliders(rtype::engine::Engine* engine)
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

void rtype::client::MainMenuOptions::displayAllSliders(rtype::engine::Engine* engine)
{
    for (unsigned i = 0; i < this->_allSliders.size(); i++)
        this->_allSliders[i].draw(engine);
}
