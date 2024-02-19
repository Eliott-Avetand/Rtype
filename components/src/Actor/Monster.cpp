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
//  File:            Monster.cpp                                                     //
//  Description:     Implement functions of the Monster class                        //
//                                                                                   //
// ********************************************************************************* //

#include "Monster.hpp"
#include "CircularPattern.hpp"
#include "SinusoidalPattern.hpp"
#include "ZigZagPattern.hpp"
#include "RandomPattern.hpp"
#include "QuickLeftPattern.hpp"

rtype::actor::Monster::Monster(rtype::actor::Breed& breed) :
    _breed(breed),
    _clock(),
    _reloadClock(),
    _reloadTime(rtype::client::utilities::RandomNumbers::randomFloat(1.0f, 10.0f)),
    _speed(3.0f),
    _angleSpeed(0.1f),
    _radius(300.0f),
    _angle(0.0f),
    _dirUp(true),
    _centerOrigins({ 1920.0f, 400.0f })
{
    this->_sprite = std::make_unique<rtype::client::utilities::AnimatedSprite>(_breed._sprite, sf::Vector2f(500.0f, 500.0f), sf::Vector2f(33.125f, 33.5f), rtype::client::utilities::AnimatedSprite::HORIZONTAL, sf::Vector2i(static_cast<int>(33.125f), static_cast<int>(33.5f)), sf::Vector2i(static_cast<int>(33.125f), static_cast<int>(33.5f)), 0.1f, false);
    this->_deathSprite = std::make_unique<rtype::client::utilities::AnimatedSprite>(EXPLOSION_SPRITE, sf::Vector2f(500.0f, 500.0f), sf::Vector2f(32.0f, 32.0f), rtype::client::utilities::AnimatedSprite::HORIZONTAL, sf::Vector2i(static_cast<int>(0), static_cast<int>(0)), sf::Vector2i(static_cast<int>(32.0f * 5), static_cast<int>(0.0f)), 0.1f, false);
    this->_deathSound = std::make_unique<rtype::engine::utilities::CustomSound>(EXPLOSION_SOUND);
    this->_entityID = "";

    if (!this->_sprite || !this->_deathSprite || !this->_deathSound)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->_state = rtype::actor::Actor::ALIVE;

    this->_canShoot = false;

    this->_sprite->getSprite().scale(sf::Vector2f(3, 3));
    this->_deathSprite->getSprite().scale(sf::Vector2f(3, 3));
    this->_strength = 1;
    this->_health = 1;
    this->_score = 50;

    if (this->_breed.getPatternName() == "circular")
        this->_pattern = std::make_unique<rtype::action::CircularPattern>();
    else if (this->_breed.getPatternName() == "sinusoidal")
        this->_pattern = std::make_unique<rtype::action::SinusoidalPattern>();
    else if (this->_breed.getPatternName() == "zigzag")
        this->_pattern = std::make_unique<rtype::action::ZigZagPattern>();
    else if (this->_breed.getPatternName() == "random")
        this->_pattern = std::make_unique<rtype::action::RandomPattern>();
    else if (this->_breed.getPatternName() == "quickLeft")
        this->_pattern = std::make_unique<rtype::action::QuickLeftPattern>();

}

std::string rtype::actor::Monster::getName()
{
    return _breed.getName();
}

void rtype::actor::Monster::update(rtype::engine::Engine* engine)
{
    if (this->_state == rtype::actor::Actor::ALIVE) {
        this->setPosition(this->getPosition());
        if (this->_reloadClock.getElapsedTime().asSeconds() >= this->_reloadTime) {
            this->_canShoot = true;
            this->_reloadClock.restart();
        }
    }

    if (this->_state == rtype::actor::Actor::EXPLOSION)
        this->_deathSprite->updateAnimation();

    if (this->_deathSprite->isAnimationFinish())
        this->_state = rtype::actor::Actor::DEAD;

    for (int i = 0; i < this->_allMissiles.size(); i++) {
        _allMissiles[i]->update();
        if (_allMissiles[i]->isDestroyed())
            _allMissiles.erase(_allMissiles.begin() + i);
    }
}

void rtype::actor::Monster::draw(rtype::engine::Engine* engine)
{
    if (this->_state == rtype::actor::Actor::ALIVE)
        _sprite->draw(engine);
    else if (this->_state == rtype::actor::Actor::EXPLOSION)
        this->_deathSprite->draw(engine);
    for (auto &missile : this->_allMissiles)
        missile->draw(engine);
}

void rtype::actor::Monster::shoot()
{
    this->_allMissiles.push_back(std::make_unique<rtype::actor::Missile>(this->getPosition(), 7, 1,  rtype::actor::Missile::missileDirection::LEFT));
    this->_canShoot = false;
}

void rtype::actor::Monster::moveUp()
{
    this->_position = { this->_position.first, this->_position.second - 3 };
}

void rtype::actor::Monster::moveUp(float speed)
{
    this->_position = { this->_position.first, this->_position.second - speed };
}

void rtype::actor::Monster::moveDown()
{
    this->_position = { this->_position.first, this->_position.second + 3 };
}

void rtype::actor::Monster::moveDown(float speed)
{
    this->_position = { this->_position.first, this->_position.second + speed };
}

void rtype::actor::Monster::moveLeft()
{
    this->_position = { this->_position.first - 3, this->_position.second };
}

void rtype::actor::Monster::moveLeft(float speed)
{
    this->_position = { this->_position.first - speed, this->_position.second };
}

void rtype::actor::Monster::moveRight()
{
    this->_position = { this->_position.first + 3, this->_position.second };
}

void rtype::actor::Monster::moveRight(float speed)
{
    this->_position = { this->_position.first + speed, this->_position.second };
}

std::vector<std::unique_ptr<rtype::actor::Missile>>& rtype::actor::Monster::getMissiles()
{
    return _allMissiles;
}

sf::Vector2f rtype::actor::Monster::getPosition()
{
    return { _position.first, _position.second };
}

void rtype::actor::Monster::setPosition(sf::Vector2f position)
{
    _position = { position.x, position.y };
    _sprite->setPosition(position);
}

int rtype::actor::Monster::getHealth(void)
{
    return _health;
}

void rtype::actor::Monster::takeDamage(int damage)
{
    if (this->_state != rtype::actor::Actor::ALIVE)
        return;

    if (this->_health - damage <= 0) {
        this->_health = 0;
        this->_state = rtype::actor::Actor::EXPLOSION;
        #ifdef CLIENT
        this->_deathSound->playSound(rtype::engine::Engine::getInstance()->options.music.isSoundActive());
        #endif // CLIENT

        this->_deathSprite->setPosition({ this->_position.first, this->_position.second });
    }
    else
        this->_health -= damage;
}

void rtype::actor::Monster::setDamage(int damage)
{
    this->_strength = damage;
}

int rtype::actor::Monster::getDamage(void)
{
    return this->_strength;
}

void rtype::actor::Monster::setHealth(int healthNumber)
{
    if (healthNumber <= 0) {
        this->_health = 0;
    }
    else
        this->_health = healthNumber;
}

float rtype::actor::Monster::getReloadedPercentage(void)
{
    return this->_shootReloaded;
}

int rtype::actor::Monster::getScore(void)
{
    return this->_score;
}

void rtype::actor::Monster::addScore(int scoreAdded)
{
    this->_score += scoreAdded;
}

void rtype::actor::Monster::subScore(int scoreSubstracted)
{
    if (this->_score - scoreSubstracted <= 0)
        this->_score = 0;
    else
        this->_score -= scoreSubstracted;
}

void rtype::actor::Monster::setScore(int score)
{
    if (score <= 0)
        this->_score = 0;
    else
        this->_score = score;
}

MoveDirection rtype::actor::Monster::getDirection(void)
{
    return this->_direction;
}

void rtype::actor::Monster::setDirection(MoveDirection direction)
{

}

sf::Clock& rtype::actor::Monster::getClock(void)
{
    return this->_clock;
}

void rtype::actor::Monster::setClock(sf::Clock clock)
{
    this->_clock = clock;
}

float rtype::actor::Monster::getSpeed(void)
{
    return this->_speed;
}

void rtype::actor::Monster::setSpeed(float speed)
{
    this->_speed = speed;
}

float rtype::actor::Monster::getAngleSpeed(void)
{
    return this->_angleSpeed;
}

float rtype::actor::Monster::getRadius(void)
{
    return this->_radius;
}

float rtype::actor::Monster::getAngle(void)
{
    return this->_angle;
}

void rtype::actor::Monster::setAngleSpeed(float angleSpeed)
{
    this->_angleSpeed = angleSpeed;
}

void rtype::actor::Monster::setRadius(float radius)
{
    this->_radius = radius;
}

void rtype::actor::Monster::setAngle(float angle)
{
    this->_angle = angle;
}

sf::Vector2f rtype::actor::Monster::getCenterOrigins(void)
{
    return this->_centerOrigins;
}

void rtype::actor::Monster::setCenterOrigins(sf::Vector2f centerOrigins)
{
    this->_centerOrigins = centerOrigins;
}

sf::Sprite& rtype::actor::Monster::getSprite(void)
{
    return this->_sprite->getSprite();
}

bool rtype::actor::Monster::isMissileCollision(rtype::actor::Missile *missile)
{
    if (this->_sprite->getSprite().getGlobalBounds().intersects(missile->getSprite().getGlobalBounds()) && this->_state == rtype::actor::Actor::ALIVE)
        return true;
    return false;
}

bool rtype::actor::Monster::isActorCollision(rtype::actor::Actor* actor)
{
    if (this->_sprite->getSprite().getGlobalBounds().intersects(actor->getSprite().getGlobalBounds()) && this->_state == rtype::actor::Actor::ALIVE)
        return true;
    return false;
}

bool rtype::actor::Monster::isAsteroidCollision(rtype::actor::Asteroid* asteroid)
{
    if (this->_sprite->getSprite().getGlobalBounds().intersects(asteroid->getSprite().getGlobalBounds()))
        return true;
    return false;
}

bool rtype::actor::Monster::isPowerUpCollision(std::unique_ptr<rtype::powerup::APowerUpItem> &powerup)
{
    return (this->_sprite->getSprite().getGlobalBounds().intersects(powerup->getSprite().getSprite().getGlobalBounds()) &&
            this->_state == rtype::actor::Actor::ALIVE && powerup->hasSpawned() && powerup->isVisible());
}

rtype::actor::Actor::actorState rtype::actor::Monster::getState(void)
{
    return this->_state;
}

void rtype::actor::Monster::setEntityID(std::string ID)
{
    if (ID.length() > 10)
        return;
    this->_entityID = ID;
}

std::string rtype::actor::Monster::getEntityID(void)
{
    return this->_entityID;
}

void rtype::actor::Monster::setBreedIndex(int index)
{
    this->_breedIndex = index;
}

int rtype::actor::Monster::getBreedIndex(void)
{
    return this->_breedIndex;
}

bool rtype::actor::Monster::canShoot(void)
{
    return this->_canShoot;
}
bool rtype::actor::Monster::getDirUp(void)
{
    return this->_dirUp;
}

void rtype::actor::Monster::setDirUp(bool dirUp)
{
    this->_dirUp = dirUp;
}

std::unique_ptr<rtype::action::ActorPattern> &rtype::actor::Monster::getPattern(void)
{
    return this->_pattern;
}
