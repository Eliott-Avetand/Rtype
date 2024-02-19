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
#include "Exemple.hpp"
#include "ACommand.hpp"
#include "TestCommand.hpp"
#include "Player.hpp"
#include <memory>
#include <iostream>
#include <string>
#include "Errors.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

rtype::exemple::Exemple *rtype::exemple::Exemple::_exempleInstance {nullptr};
std::mutex rtype::exemple::Exemple::_mutex;

/**
 * @brief Initialize everything needed by the scene like the sprite, enemies or inputs
 * @see This method is automatically called by the engine
 * @param engine    A reference of the engine given by the engine himself
 */
void rtype::exemple::Exemple::init(rtype::engine::Engine *engine)
{
    std::cout << "Initializing the exemple class..." << std::endl;
    //_player = std::make_unique<rtype::actor::Player>();
    loadEnemies();
    initInput();
    std::cout << "Exemple class initialized." << std::endl;
}

/**
 * @brief Initialize the command by associating an input to it
 * @see This method must be call inside "init" method to bind an input to a command
 */
void rtype::exemple::Exemple::initInput()
{
    inputHandler.bindCommand(sf::Keyboard::A, std::make_unique<rtype::action::TestCommand>());
}

/**
 * @brief Initialize all breeds define in a file located at "data/level_{number of the level}/enemies.json"
 * @see This method must be call inside "init" method in scene that needs to load enemies
 */
void rtype::exemple::Exemple::loadEnemies()
{
    std::vector<std::map<std::string, std::string>> datas;
    boost::property_tree::ptree root;

    boost::property_tree::read_json("data/level_1/enemies.json", root);
    for (boost::property_tree::ptree::value_type &parent : root.get_child("")) {
        std::map<std::string, std::string> enemy;

        for (boost::property_tree::ptree::value_type &data : root.get_child(parent.first)) {
            enemy[data.first] = data.second.data();
        }
        datas.push_back(enemy);
    }
    for (int i = 0; i < datas.size(); i++) {
        std::cout << i << "+" << datas.size() << std::endl;
        std::cout << datas[i]["name"] << std::endl;
        _breeds.push_back(std::make_unique<rtype::actor::Breed>(std::stoi(datas[i]["maxHealth"]),
        datas[i]["name"], std::stoi(datas[i]["occurences"])));
    }
}

/**
 * @brief Basically, this method cleanup everything that have been initialized inside the init function
 * @see This method is automatically called by the engine upon deletion of this scene
 */
void rtype::exemple::Exemple::cleanup()
{
    std::cout << "Cleaning the exemple class..." << std::endl;
    _exempleInstance = nullptr;
    std::cout << "Exemple class cleaned up." << std::endl;
}

/**
 * @brief When a new scene like a menu is created in addition of an already existed scene,
 * it pauses the scene already created.
 * This method is here to stop everything inside the current scene (like a clock for exemple).
 * @see This method is automatically called by the engine upon creation of new scene.
 */
void rtype::exemple::Exemple::pause()
{
}

/**
 * @brief When scene like a menu is removed, it resumes the other scenes that have been paused.
 * This method is here to resume everything that have been paused.
 * @see This method is automatically called by the engine upon deletion of another scene.
 */
void rtype::exemple::Exemple::resume()
{
}

/**
 * @brief Each scene must be able to reiceive date from the server to verify the informations.
 * Here, you can wait and listen to every datas sent by the server.
 * @see This method is automatically called by the engine on each loop
 */
void rtype::exemple::Exemple::networkReceiver(rtype::engine::Engine *engine)
{
}

/**
 * @brief Each scene must be able to send date to the server.
 * Here, you can send datas to the server.
 * @see This method is automatically called by the engine on each loop
 */
void rtype::exemple::Exemple::networkSender(rtype::engine::Engine *engine)
{
}

/**
 * @brief This method scan each input and check if it's pressed.
 * If so, it perform the command associated to it.
 * @see This method is automatically called by the engine on each loop
 */
void rtype::exemple::Exemple::processInput(sf::Event event, rtype::engine::Engine *engine)
{
    for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
        if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i)) {
            std::unique_ptr<rtype::action::ACommand> &command = inputHandler.handleInput((sf::Keyboard::Key)i);
            if (command)
                command->perform(_player);
        }
    }
}

/**
 * @brief This method's goal is to update everything that arn't related to an action
 * like update the clock.
 * @see This method is automatically called by the engine on each loop
 */
void rtype::exemple::Exemple::update(rtype::engine::Engine *engine, double deltaTime)
{
    (void)deltaTime;
}

/**
 * @brief This method's goal is to simply render each sprite that need to be.
 * @see This method is automatically called by the engine on each loop
 */
void rtype::exemple::Exemple::render(rtype::engine::Engine *engine)
{
    engine->window.clear();
    engine->window.display();
}