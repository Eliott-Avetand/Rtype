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
//  File:            MainMenuEsaterEgg.cpp                                           //
//  Description:     File for the MainMenuEsaterEgg Class                            //
//                                                                                   //
// ********************************************************************************* //
#include "MainMenuEasterEgg.hpp"
#include "MainMenu.hpp"

rtype::client::MainMenuEasterEgg *rtype::client::MainMenuEasterEgg::_mainMenuEasterEggInstance{nullptr};
std::mutex rtype::client::MainMenuEasterEgg::_mutex;

void backToMainMenu(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    engine->changeState(rtype::client::MainMenu::getInstance());
}

void rtype::client::MainMenuEasterEgg::init(rtype::engine::Engine* engine)
{
    std::cout << "Initializing the exemple class..." << std::endl;

    _mainMenuEasterEggInstance = this;

    createMainMenuTexts(engine);
    createTitleLogo(engine);
    createEasterEggLoad(engine);
    createEasterEgg(engine);
    createEasterEggEnd(engine);

    _displayFunctionsMap = { {rtype::client::MainMenuEasterEgg::menuState::MENU_DESTROY, &rtype::client::MainMenuEasterEgg::displayMenuDestroy},
                             {rtype::client::MainMenuEasterEgg::menuState::EASTEREGG_LOAD, &rtype::client::MainMenuEasterEgg::displayEasterEggLoad},
                             {rtype::client::MainMenuEasterEgg::menuState::EASTEREGG, &rtype::client::MainMenuEasterEgg::displayEasterEgg},
                             {rtype::client::MainMenuEasterEgg::menuState::EASTEREGG_END, &rtype::client::MainMenuEasterEgg::displayEasterEggEnd}};

    _updateFunctionsMap = { {rtype::client::MainMenuEasterEgg::menuState::MENU_DESTROY, &rtype::client::MainMenuEasterEgg::updateMenuDestroy},
                             {rtype::client::MainMenuEasterEgg::menuState::EASTEREGG_LOAD, &rtype::client::MainMenuEasterEgg::updateEasterEggLoad},
                             {rtype::client::MainMenuEasterEgg::menuState::EASTEREGG, &rtype::client::MainMenuEasterEgg::updateEasterEgg},
                             {rtype::client::MainMenuEasterEgg::menuState::EASTEREGG_END, &rtype::client::MainMenuEasterEgg::updateEasterEggEnd} };

    _processInputFunctionsMap = { {rtype::client::MainMenuEasterEgg::menuState::MENU_DESTROY, &rtype::client::MainMenuEasterEgg::processInputMenuDestroy},
                         {rtype::client::MainMenuEasterEgg::menuState::EASTEREGG_LOAD, &rtype::client::MainMenuEasterEgg::processInputEasterEggLoad},
                         {rtype::client::MainMenuEasterEgg::menuState::EASTEREGG, &rtype::client::MainMenuEasterEgg::processInputEasterEgg},
                         {rtype::client::MainMenuEasterEgg::menuState::EASTEREGG_END, &rtype::client::MainMenuEasterEgg::processInputEasterEggEnd} };

    _state = rtype::client::MainMenuEasterEgg::menuState::MENU_DESTROY;
    _timeLeft = 2;
    _nbrDisplayed = 0;
    _isMenuDisplayed = true;

    _counter = 3;

    _scoreNbr = 0;

    _displayScore = false;

    std::cout << "Exemple class initialized." << std::endl;
}

void rtype::client::MainMenuEasterEgg::cleanup()
{
    std::cout << "Cleaning the exemple class..." << std::endl;
    releaseInstance();
    std::cout << "Exemple class cleaned up." << std::endl;
}

void rtype::client::MainMenuEasterEgg::pause()
{
}

void rtype::client::MainMenuEasterEgg::resume()
{
}

void rtype::client::MainMenuEasterEgg::networkReceiver(rtype::engine::Engine *engine)
{
}

void rtype::client::MainMenuEasterEgg::networkSender(rtype::engine::Engine *engine)
{
}

void rtype::client::MainMenuEasterEgg::processInput(sf::Event event, rtype::engine::Engine* engine)
{
    _background.eventUpdate(event, engine);
    if (_displayFunctionsMap.contains(_state))
        (this->*_processInputFunctionsMap[_state])(event, engine);
}

void rtype::client::MainMenuEasterEgg::update(rtype::engine::Engine* engine)
{
    _background.update(engine);
    if (_displayFunctionsMap.contains(_state))
        (this->*_updateFunctionsMap[_state])(engine);

}

void rtype::client::MainMenuEasterEgg::render(rtype::engine::Engine* engine)
{
    std::vector<rtype::client::MainMenuEasterEgg::menuState> allStates = { MENU_DESTROY, EASTEREGG_LOAD, EASTEREGG, EASTEREGG_END };

    engine->window.clear();

    _background.display(engine);
    if (_displayFunctionsMap.contains(_state))
        (this->*_displayFunctionsMap[_state])(engine);
    engine->window.display();
}

void rtype::client::MainMenuEasterEgg::updateObjectsPosition(sf::Vector2f deltaSize)
{
    (void)deltaSize;
}

void rtype::client::MainMenuEasterEgg::createTitleLogo(rtype::engine::Engine* engine)
{
    _titleLogo.setPosition(sf::Vector2f(engine->options.getWindowWidth() / 2, engine->options.getWindowHeight() * 10 / 100));
    _titleLogo.setScale(1, 1.25);
}

void rtype::client::MainMenuEasterEgg::displayTitleLogo(rtype::engine::Engine* engine)
{
    _titleLogo.draw(engine);
}

void rtype::client::MainMenuEasterEgg::createMainMenuTexts(rtype::engine::Engine* engine)
{
    float windowWidth = engine->options.getWindowWidth();
    float windowHeight = engine->options.getWindowHeight();

    std::vector<std::string> text = { "SOLO", "ONLINE", "OPTIONS", "QUIT GAME", "LoustikStudio 2023" };
    std::vector<sf::Vector2f> textPosition = { {windowWidth / 2, windowHeight / 2 - (windowHeight * 10 / 100)}, {windowWidth / 2, windowHeight / 2}, {windowWidth / 2, windowHeight / 2 + (windowHeight * 10 / 100)}, {windowWidth / 2, windowHeight / 2 + (windowHeight * 20 / 100)}, {0, 0} };
    std::vector<int> textSize = { 75, 75, 75, 75, 25 };

    for (unsigned int i = 0; i < 5; i++) {
        _allBeginTexts.push_back(std::make_unique<rtype::client::utilities::CustomText>(text[i], FONT_PATH, textSize[i], sf::Color::White, textPosition[i]));

        if (!_allBeginTexts[i])
            throw rtype::settings::Errors("Not enough memories", "MemoryError:");
    }

    sf::FloatRect bounds = _allBeginTexts[4]->getLocalBounds();
    _allBeginTexts[4]->setPosition(sf::Vector2f(windowWidth - bounds.width / 2 - 10, windowHeight - bounds.height));
}

void rtype::client::MainMenuEasterEgg::createEasterEggLoad(rtype::engine::Engine* engine)
{
    float windowWidth = engine->options.getWindowWidth();
    float windowHeight = engine->options.getWindowHeight();

    _counterText = std::make_unique<rtype::client::utilities::CustomText>("3", FONT_PATH, 75, sf::Color::White, sf::Vector2f(windowWidth / 2, windowHeight / 2));

    if (!_counterText)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");
}

void rtype::client::MainMenuEasterEgg::createEasterEgg(rtype::engine::Engine* engine)
{
    float windowWidth = engine->options.getWindowWidth();
    float windowHeight = engine->options.getWindowHeight();
    sf::FloatRect bounds;

    _scoreText = std::make_unique<rtype::client::utilities::CustomText>("SCORE 0", FONT_PATH, 60, sf::Color::White, sf::Vector2f(0, 0));
    _timeLeftText = std::make_unique<rtype::client::utilities::CustomText>("TIME LEFT 60", FONT_PATH, 60, sf::Color::White, sf::Vector2f(0, 0));

    if (!_scoreText || !_timeLeftText)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    bounds = _scoreText->getLocalBounds();
    _scoreText->setPosition(sf::Vector2f(0 + bounds.width / 2 + 10, windowHeight - bounds.height));
    bounds = _timeLeftText->getLocalBounds();
    _timeLeftText->setPosition(sf::Vector2f(0 + bounds.width / 2 + 10, 0 + bounds.height));
}

void rtype::client::MainMenuEasterEgg::createEasterEggEnd(rtype::engine::Engine* engine)
{
    float windowWidth = engine->options.getWindowWidth();
    float windowHeight = engine->options.getWindowHeight();

    _backToMenuButton = std::make_unique<rtype::client::utilities::CustomButton>("CONTINUE", sf::Vector2f(0.0f, 0.0f), 75.0f, backToMainMenu, 1);
    _endText = std::make_unique<rtype::client::utilities::CustomText>("FINISHED!!", FONT_PATH, 75, sf::Color::White, sf::Vector2f(windowWidth / 2, windowHeight / 2));

    if (!_endText || !_backToMenuButton)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    _endText->setPosition(sf::Vector2f(windowWidth / 2, windowHeight / 2));
    _backToMenuButton->setPosition(sf::Vector2f(windowWidth / 2, windowHeight * 90 / 100));

}

void rtype::client::MainMenuEasterEgg::displayMainMenuTexts(rtype::engine::Engine* engine)
{
    for (unsigned int i = 0; i < _allBeginTexts.size(); i++)
        _allBeginTexts[i]->draw(engine);
}

void rtype::client::MainMenuEasterEgg::displayMenuDestroy(rtype::engine::Engine* engine)
{
    displayTitleLogo(engine);
    displayMainMenuTexts(engine);
}

void rtype::client::MainMenuEasterEgg::displayEasterEggLoad(rtype::engine::Engine* engine)
{
    _counterText->draw(engine);
}

void rtype::client::MainMenuEasterEgg::displayEasterEgg(rtype::engine::Engine* engine)
{
    _timeLeftText->draw(engine);
    _scoreText->draw(engine);
}

void rtype::client::MainMenuEasterEgg::displayEasterEggEnd(rtype::engine::Engine* engine)
{
    _endText->draw(engine);
    if (_displayScore)
        _backToMenuButton->draw(engine);
}

void rtype::client::MainMenuEasterEgg::updateMenuDestroy(rtype::engine::Engine* engine)
{
    sf::Time time = _sceneClock.getElapsedTime();
    float seconds = time.asSeconds();

    if (seconds > _timeLeft) {

        if (_isMenuDisplayed)
            _titleLogo.setOpacity(_titleLogo.getOpacity() - 5);
        else
            _titleLogo.setOpacity(_titleLogo.getOpacity() + 5);

        for (unsigned int i = 0; i < _allBeginTexts.size(); i++) {
            sf::Color color = _allBeginTexts[i]->getColor();

            if (_isMenuDisplayed)
                _allBeginTexts[i]->setColor(sf::Color(color.r, color.g, color.b, color.a - 5));
            else
                _allBeginTexts[i]->setColor(sf::Color(color.r, color.g, color.b, color.a + 5));
        }
        
        if (_allBeginTexts[0]->getColor().a <= 0) {
            _nbrDisplayed += 1;

            if (_nbrDisplayed >= 3) {
                _state = rtype::client::MainMenuEasterEgg::EASTEREGG_LOAD;
                _timeLeft = 1;
                _sceneClock.restart();
                return;
            }
            _isMenuDisplayed = false;
        }
        if (_allBeginTexts[0]->getColor().a >= 255)
            _isMenuDisplayed = true;

        _timeLeft = 0.01;
        _sceneClock.restart();
    }
}

void rtype::client::MainMenuEasterEgg::updateEasterEggLoad(rtype::engine::Engine* engine)
{
    sf::Time time = _sceneClock.getElapsedTime();
    float seconds = time.asSeconds();

    if (seconds > _timeLeft) {
        _counter--;
        if (_counter == 0)
            _counterText->setText("GO!");
        else if (_counter < 0) {
            _state = rtype::client::MainMenuEasterEgg::EASTEREGG;
            _background.toggleAsteroidsSpawn(true);
            _background.manageAsteroidsSpawnTime(0.1, 0.9);
            _timeLeft = 60;
            _sceneClock.restart();
            return;
        }
        else
            _counterText->setText(std::to_string(_counter));
        _sceneClock.restart();
    }
}

void rtype::client::MainMenuEasterEgg::updateEasterEgg(rtype::engine::Engine* engine)
{
    sf::Time time = _sceneClock.getElapsedTime();
    float seconds = time.asSeconds();

    _scoreText->setText("SCORE " + std::to_string(_background.getDestroyedAsteroids()));
    _timeLeftText->setText("TIME LEFT " + std::to_string(60 - (int)seconds));

    if (60 - (int)seconds < 20)
        _background.manageAsteroidsSpawnTime(0.1, 0.2);

    if (seconds > _timeLeft) {
        _background.destroyAllAsteroids();
        _background.toggleAsteroidsSpawn(false);
        _timeLeft = 5;
        _state = rtype::client::MainMenuEasterEgg::EASTEREGG_END;
        _sceneClock.restart();
    }
}

void rtype::client::MainMenuEasterEgg::updateEasterEggEnd(rtype::engine::Engine* engine)
{
    sf::Time time = _sceneClock.getElapsedTime();
    float seconds = time.asSeconds();

    if (seconds > _timeLeft && !_displayScore) {
        _endText->setText("YOUR SCORE " + std::to_string(_background.getDestroyedAsteroids()));
        _displayScore = true;
        _sceneClock.restart();
        _isMenuDisplayed = true;
    }
    if (seconds > _timeLeft && _displayScore) {
        sf::Color color = _endText->getColor();

        if (_endText->getColor().a <= 0) {
            color.a = 0;
            _isMenuDisplayed = false;
        }

        if (_endText->getColor().a >= 255) {
            color.a = 255;
            _isMenuDisplayed = true;
        }

        if (_isMenuDisplayed)
            color.a -= 5;
        else
            color.a += 5;

        _endText->setColor(color);
        _sceneClock.restart();
    }
}

void rtype::client::MainMenuEasterEgg::processInputMenuDestroy(sf::Event event, rtype::engine::Engine* engine)
{

}

void rtype::client::MainMenuEasterEgg::processInputEasterEggLoad(sf::Event event, rtype::engine::Engine* engine)
{

}

void rtype::client::MainMenuEasterEgg::processInputEasterEgg(sf::Event event, rtype::engine::Engine* engine)
{

}

void rtype::client::MainMenuEasterEgg::processInputEasterEggEnd(sf::Event event, rtype::engine::Engine* engine)
{
    _backToMenuButton->eventUpdate(event, engine);
}