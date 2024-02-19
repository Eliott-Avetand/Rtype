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
//  File:            BackgroundSpaceGenerator.cpp                                    //
//  Description:     BackgroundSpaceGenerator File Class                             //
//                                                                                   //
// ********************************************************************************* //

#include "BackgroundSpaceGenerator.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

rtype::client::utilities::BackgroundSpaceGenerator::BackgroundSpaceGenerator(std::string backgroundName)
{
    this->_asteroidsGenerator.toggleAsteroidsSpawn(false);
    this->_planetsGenerator.togglePlanetsSpawn(false);
    this->_isBackgroundMoving = false;
    this->_speed = 1;
    this->createBackground(rtype::engine::Engine::getInstance(), backgroundName);
}

rtype::client::utilities::BackgroundSpaceGenerator::BackgroundSpaceGenerator()
{
    this->_asteroidsGenerator.toggleAsteroidsSpawn(false);
    this->_planetsGenerator.togglePlanetsSpawn(false);
    this->_isBackgroundMoving = false;
    this->_speed = 1;
    this->createBackground(rtype::engine::Engine::getInstance(), "default");
}

void rtype::client::utilities::BackgroundSpaceGenerator::update(rtype::engine::Engine* engine)
{
    this->_asteroidsGenerator.update(engine);
    this->_planetsGenerator.update(engine);
    this->updateBackground(engine);
    if (_starsImage.size() > 0)
        this->updateStars();
}

void rtype::client::utilities::BackgroundSpaceGenerator::eventUpdate(sf::Event event, rtype::engine::Engine* engine)
{
    this->_asteroidsGenerator.eventUpdate(event, engine);
}

void rtype::client::utilities::BackgroundSpaceGenerator::display(rtype::engine::Engine* engine)
{
    this->displayBackground(engine);
    this->_asteroidsGenerator.drawAsteroids(engine);
}

int rtype::client::utilities::BackgroundSpaceGenerator::getDestroyedAsteroids(void)
{
    return this->_asteroidsGenerator.getDestroyedAsteroids();
}

void rtype::client::utilities::BackgroundSpaceGenerator::destroyAllAsteroids(void)
{
    this->_asteroidsGenerator.destroyAllAsteroids();
}

bool rtype::client::utilities::BackgroundSpaceGenerator::isAsteroids(void)
{
    return this->_asteroidsGenerator.isAsteroids();
}

void rtype::client::utilities::BackgroundSpaceGenerator::toggleAsteroidsSpawn(bool canSpawn)
{
    this->_asteroidsGenerator.toggleAsteroidsSpawn(canSpawn);
}

void rtype::client::utilities::BackgroundSpaceGenerator::createBackground(rtype::engine::Engine* engine, std::string backgroundName)
{
    sf::Vector2f middleScreen = sf::Vector2f(engine->options.getWindowWidth() / 2, engine->options.getWindowHeight() / 2);

    _starsState = rtype::client::utilities::BackgroundSpaceGenerator::starsState::STATIC;
    _starsTimeDisplay = rtype::client::utilities::RandomNumbers::randomInteger(4, 10);
    _starsDisplay = STARS_1_INDEX;

    std::vector<std::map<std::string, std::string>> datas;
    boost::property_tree::ptree root;

    boost::property_tree::read_json("data/backgrounds.json", root);

    try {
        for (boost::property_tree::ptree::value_type& parent : root.get_child(backgroundName)) {

            if (parent.first == "background") {
                this->_backgroundImage = std::make_unique<rtype::client::utilities::StaticSprite>(parent.second.data(), middleScreen);

                if (!this->_backgroundImage)
                    throw rtype::settings::Errors("Not enough memories", "MemoryError:");

            }

            if (parent.first == "dusts") {
                this->_dustImage.push_back(rtype::client::utilities::StaticSprite(parent.second.data(), middleScreen));

                std::string reverse = parent.second.data();

                for (int i = 0; i < 4; i++)
                    reverse = reverse.erase(reverse.size() - 1);

                this->_dustImage.push_back(rtype::client::utilities::StaticSprite(reverse + "_reverse.png", sf::Vector2f(middleScreen.x * 3, middleScreen.y)));

                this->_dustImage[0].setOpacity(100);
                this->_dustImage[1].setOpacity(100);
            }

            if (parent.first == "stars") {

                if (parent.second.size() != 2)
                    throw rtype::settings::Errors("", "");

                for (auto& stars : parent.second)
                    this->_starsImage.push_back(rtype::client::utilities::StaticSprite(stars.second.get_value<std::string>(), middleScreen));

                for (auto& stars : parent.second)
                    _starsImage.push_back(rtype::client::utilities::StaticSprite(stars.second.get_value<std::string>(), sf::Vector2f(middleScreen.x * 3, middleScreen.y)));

                this->_starsImage[1].setOpacity(0);
                this->_starsImage[3].setOpacity(0);

            }

            if (parent.first == "defile") {
                this->_backgroundDefile.push_back(rtype::client::utilities::StaticSprite(parent.second.data(), middleScreen));
                this->_backgroundDefile.push_back(rtype::client::utilities::StaticSprite(parent.second.data(), sf::Vector2f(middleScreen.x * 3, middleScreen.y)));
                if (this->_backgroundDefile.size() != 2)
                    throw rtype::settings::Errors("Not enough memories", "MemoryError:");
            }
        }
    }
    catch (const std::exception &error) {
        throw rtype::settings::Errors("", "");
    }
}

void rtype::client::utilities::BackgroundSpaceGenerator::updateBackground(rtype::engine::Engine* engine)
{
    sf::Time time = this->_backgroundMoveClock.getElapsedTime();
    float milliseconds = time.asMilliseconds();

    float endPosition = (float)(engine->options.getWindowWidth() / 2) * -1;
    float beginPosition = engine->options.getWindowWidth() / 2 * 3;

    if (!_isBackgroundMoving)
        return;

    if (milliseconds > 10) {
        for (size_t i = 0; i < this->_starsImage.size(); i++) {
            if (this->_starsImage[i].getPosition().x - (0.1 * this->_speed) < endPosition)
                this->_starsImage[i].setPosition(sf::Vector2f(beginPosition, this->_starsImage[i].getPosition().y));
            else
                this->_starsImage[i].setPosition(sf::Vector2f(this->_starsImage[i].getPosition().x - (0.1 * this->_speed), this->_starsImage[i].getPosition().y));

        }
        for (size_t i = 0; i < this->_dustImage.size(); i++) {
            if (this->_dustImage[i].getPosition().x - 2 < endPosition)
                this->_dustImage[i].setPosition(sf::Vector2f(beginPosition - (2 * this->_speed), this->_dustImage[i].getPosition().y));
            else
                this->_dustImage[i].setPosition(sf::Vector2f(this->_dustImage[i].getPosition().x - (2 * this->_speed), this->_dustImage[i].getPosition().y));
        }
        for (size_t i = 0; i < this->_backgroundDefile.size(); i++) {
            if (this->_backgroundDefile[i].getPosition().x - 2 < endPosition)
                this->_backgroundDefile[i].setPosition(sf::Vector2f(beginPosition - (2 * this->_speed), this->_backgroundDefile[i].getPosition().y));
            else
                this->_backgroundDefile[i].setPosition(sf::Vector2f(this->_backgroundDefile[i].getPosition().x - (2 * this->_speed), this->_backgroundDefile[i].getPosition().y));
        }
    }
        this->_backgroundMoveClock.restart();
}

void rtype::client::utilities::BackgroundSpaceGenerator::displayBackground(rtype::engine::Engine* engine)
{
    this->_backgroundImage->draw(engine);
    for (auto imageComponent : this->_starsImage)
        imageComponent.draw(engine);
    this->_planetsGenerator.drawPlanets(engine);
    for (auto imageComponent : this->_dustImage)
        imageComponent.draw(engine);
    for (auto defile : _backgroundDefile)
        defile.draw(engine);
}

void  rtype::client::utilities::BackgroundSpaceGenerator::swipeStars(void)
{
    if (this->_starsImage[this->_starsDisplay].getOpacity() > 0) {
        if (this->_starsDisplay == STARS_1_INDEX) {
            for (size_t i = 0; i < this->_starsImage.size(); i++) {
                if (i % 2 == STARS_1_INDEX)
                    this->_starsImage[i].setOpacity(this->_starsImage[i].getOpacity() - 5);
                else
                    this->_starsImage[i].setOpacity(this->_starsImage[i].getOpacity() + 5);
            }
        }
        else {
            for (size_t i = 0; i < _starsImage.size(); i++) {
                if (i % 2 == STARS_1_INDEX)
                    this->_starsImage[i].setOpacity(this->_starsImage[i].getOpacity() + 5);
                else
                    this->_starsImage[i].setOpacity(this->_starsImage[i].getOpacity() - 5);
            }
        }
        this->_starsClock.restart();
        this->_starsTimeDisplay = 0.05;
    }
    else {
        if (_starsDisplay == STARS_1_INDEX) {
            for (size_t i = 0; i < this->_starsImage.size(); i++) {
                if (i % 2 == STARS_1_INDEX)
                    this->_starsImage[i].setOpacity(0);
                else
                    this->_starsImage[i].setOpacity(255);
            }
            this->_starsDisplay = STARS_2_INDEX;
        }
        else {
            for (size_t i = 0; i < this->_starsImage.size(); i++) {
                if (i % 2 == STARS_1_INDEX)
                    this->_starsImage[i].setOpacity(255);
                else
                    this->_starsImage[i].setOpacity(0);
            }
            this->_starsDisplay = STARS_1_INDEX;
        }

        this->_starsState = rtype::client::utilities::BackgroundSpaceGenerator::starsState::STATIC;
        this->_starsClock.restart();
        this->_starsTimeDisplay = rtype::client::utilities::RandomNumbers::randomInteger(4, 10);
    }
}

void rtype::client::utilities::BackgroundSpaceGenerator::updateStars(void)
{
    sf::Time time = this->_starsClock.getElapsedTime();
    float seconds = time.asSeconds();

    if (seconds > _starsTimeDisplay) {
        if (this->_starsState == rtype::client::utilities::BackgroundSpaceGenerator::starsState::CHANGE)
            this->swipeStars();
        else {
            this->_starsState = rtype::client::utilities::BackgroundSpaceGenerator::starsState::CHANGE;
            this->_starsClock.restart();
            this->_starsTimeDisplay = 0.05;
        }
    }
}

void rtype::client::utilities::BackgroundSpaceGenerator::setBackgroundSpeed(float speedMultiplicator)
{
    this->_speed = speedMultiplicator;
}

void rtype::client::utilities::BackgroundSpaceGenerator::toggleBackgroundMove(bool isMoving)
{
    this->_isBackgroundMoving = isMoving;
}

void rtype::client::utilities::BackgroundSpaceGenerator::manageAsteroidsSpawnTime(float minTime, float maxTime)
{
    this->_asteroidsGenerator.manageAsteroidsSpawnTime(minTime, maxTime);
}

void rtype::client::utilities::BackgroundSpaceGenerator::togglePlanetsSpawn(bool canSpawn)
{
    this->_planetsGenerator.togglePlanetsSpawn(canSpawn);
}

void rtype::client::utilities::BackgroundSpaceGenerator::managePlanetsSpawnTime(float minTime, float maxTime)
{
    this->_planetsGenerator.managePlanetsSpawnTime(minTime, maxTime);
}

void rtype::client::utilities::BackgroundSpaceGenerator::addPlanet(std::string spriteSheet, int numberPlanet, sf::Vector2f position, float size, float speed, bool canMove)
{
    this->_planetsGenerator.forceSpawnPlanets(spriteSheet, numberPlanet, position, size, speed, canMove);
}

std::vector<Asteroids>& rtype::client::utilities::BackgroundSpaceGenerator::getAllAsteroids(void)
{
    return this->_asteroidsGenerator.getAllAsteroids();
}

void rtype::client::utilities::BackgroundSpaceGenerator::setAsteroidsPositionSpawn(Asteroids::asteroidsSpawn spawnPosition)
{
    this->_asteroidsGenerator.setAsteroidsPositionSpawn(spawnPosition);
}