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
//  File:            GameEngine.cpp                                                  //
//  Description:     The game engine main class                                      //
//                                                                                   //
// ********************************************************************************* //

#include "Engine.hpp"
#include "AGameState.hpp"
#include "APlayerMovementComponent.hpp"
#include "SFML/Graphics.hpp"

#include <iostream>
#include <memory>

#ifdef SERVER
    #include "../../server/include/UDPServer.hpp"
#endif

rtype::engine::Engine *rtype::engine::Engine::_gameEngineInstance {nullptr};
std::mutex rtype::engine::Engine::_mutex;

void rtype::engine::Engine::init()
{
    std::cout << "Initializing the game engine..." << std::endl;
    srand(time(NULL));

    _gameEngineInstance = this;

    window.create(sf::VideoMode::getFullscreenModes()[0], options.getTitle(), sf::Style::Fullscreen);
    window.setFramerateLimit(options.getFps());
    this->setWindowIcon();
    view = window.getDefaultView();
    window.setView(view);
    this->_shouldClose = false;
    this->_isFullscreen = true;
    this->_clock.restart();
    this->_keepTime = _clock.getElapsedTime();
    this->_lag = 0.0;
    std::cout << "The game engine has been initialized." << std::endl;
}

void rtype::engine::Engine::cleanup()
{
    std::cout << "Cleaning the game engine..." << std::endl;
    while (!this->_states.empty()) {
        this->_states.back()->cleanup();
        this->_states.pop_back();
    }
    this->window.close();
    std::cout << "The game engine has been cleanup." << std::endl;
}

void rtype::engine::Engine::changeState(rtype::scene::AGameState *state)
{
    while (!this->_states.empty()) {
        this->_states.back()->cleanup();
        this->_states.pop_back();
    }
    this->_states.push_back(state);
    this->_states.back()->init(this);
}

void rtype::engine::Engine::pushState(rtype::scene::AGameState *state)
{
    if (!this->_states.empty())
        this->_states.back()->pause();
    this->_states.push_back(state);
    this->_states.back()->init(this);
}

void rtype::engine::Engine::popState()
{
    if (!this->_states.empty()) {
        this->_states.back()->cleanup();
        this->_states.pop_back();
    }
    if (!this->_states.empty())
        this->_states.back()->resume();
}

void rtype::engine::Engine::processInput(sf::Event event, rtype::engine::Engine *engine)
{
    if (this->window.hasFocus()) {
        auto ptr = dynamic_cast<rtype::scene::APlayerMovementComponent *>(this->_states.back());
        if (ptr)
            ptr->processPlayerInput(engine);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11))
            this->toggleFullscreen();
    }
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            this->_shouldClose = true;
        if (this->window.hasFocus()) {
            this->_states.back()->processInput(event, engine);
            this->processResizeEvent(event);
        }
    }
}

void rtype::engine::Engine::update()
{
    this->_mousePoint = sf::Mouse::getPosition();
    this->options.music.update();
    this->_states.back()->update(this);
}

void rtype::engine::Engine::render()
{
    this->_states.back()->render(this);
}

void rtype::engine::Engine::gameLoop()
{
    double MS_PER_UPDATE = ((double)1 / (double)options.getFps());

    while (window.isOpen() && !this->_shouldClose) {
        sf::Time current = this->_clock.getElapsedTime();
        #ifdef CLIENT
            bool isOnline = this->getClient()->getIsOnline();
        #else
            bool isOnline = true;
        #endif
        double elapsed = (double)current.asSeconds() - (double)this->_keepTime.asSeconds();
        this->_keepTime = current;
        this->_lag += elapsed;
        if (isOnline)
            this->_states.back()->networkReceiver(this);
        this->processInput(event, this);
        while (this->_lag >= MS_PER_UPDATE) {
            update();
            this->_lag -= MS_PER_UPDATE;
        }
        if (isOnline)
            this->_states.back()->networkSender(this);
        this->render();
    }
}

sf::Vector2i rtype::engine::Engine::getMouseCoordinates()
{
    return this->_mousePoint;
}

void rtype::engine::Engine::shouldClose()
{
    this->_shouldClose = true;
}

void rtype::engine::Engine::processResizeEvent(sf::Event event)
{
    // if (event.type == sf::Event::Resized) {
    //     sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
    //     this->window.setView(sf::View(visibleArea));
    // }
}

bool rtype::engine::Engine::isFullscreen(void)
{
    return this->_isFullscreen;
}

void rtype::engine::Engine::setWindowIcon(void)
{
    sf::Image icon;

    if (!icon.loadFromFile(ICON_TEXTURE)) {
        if (!icon.loadFromFile(MISSING_TEXTURE))
            throw rtype::settings::Errors("File not found", "TextureError:");
    }

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void rtype::engine::Engine::toggleFullscreen()
{
    sf::Vector2u oldSize(this->options.getWindowWidth(), this->options.getWindowHeight());

    this->window.close();
    if (this->_isFullscreen == false)
        this->window.create(sf::VideoMode::getFullscreenModes()[0], this->options.getTitle(), sf::Style::Fullscreen);
    else
        this->window.create(sf::VideoMode::getDesktopMode(), this->options.getTitle(), sf::Style::Default);
    this->_isFullscreen = !this->_isFullscreen;
    this->setWindowIcon();
    this->window.setFramerateLimit(this->options.getFps());

    sf::Vector2u newSize = this->window.getSize();
    this->options.setWindowSize(sf::Vector2f((float)newSize.x, (float)newSize.y));
    this->_states.back()->updateObjectsPosition(sf::Vector2f((float)newSize.x - (float)oldSize.x, (float)newSize.y - (float)oldSize.y));
}