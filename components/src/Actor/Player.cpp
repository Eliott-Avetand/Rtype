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
//  File:            Player.cpp                                                      //
//  Description:     Implement functions of the Player class                         //
//                                                                                   //
// ********************************************************************************* //

#include "Player.hpp"

/**
 * @brief Create component which have all player informations
 * @see Constructor is called when this class is create
 * @param std::string	name
 * @param int id
 */
rtype::actor::Player::Player(std::string name, int id) :
    _shootSound(PLAYER_SHOOT_SOUND_1),
    _damageSound(PLAYER_DAMAGE_SOUND)
{
    this->_name = name;
    this->_id = id;

    this->_entityID = "";

    this->_direction = IDLE;

    this->_score = 0;

    this->_powerup = nullptr;

    this->_position = { 50.0f, 50.0f };
    this->_canShoot = true;
    this->_shootReloaded = 100;

    this->_health = 3;
    this->_strength = 1;

    this->_state = rtype::actor::Actor::ALIVE;
    this->_playerState = rtype::actor::Player::NORMAL;

    this->_sprite = std::make_unique<rtype::client::utilities::AnimatedSprite>(PLAYER_SPRITE_TOP, sf::Vector2f(static_cast<float>(_position.first), static_cast<float>(_position.second)), sf::Vector2f(32.8f, 15.0f), rtype::client::utilities::AnimatedSprite::HORIZONTAL, sf::Vector2i(static_cast<int>(32.8f * 2), 0), sf::Vector2i(static_cast<int>(32.8f * 2), 0), 0.1f, false);
    if (!this->_sprite)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    this->_bottomSprite = std::make_unique<rtype::client::utilities::AnimatedSprite>(PLAYER_SPRITE_BOTTOM, sf::Vector2f(static_cast<float>(_position.first), static_cast<float>(_position.second)), sf::Vector2f(32.8f, 15.0f), rtype::client::utilities::AnimatedSprite::HORIZONTAL, sf::Vector2i(static_cast<int>(32.8f * 2), 0), sf::Vector2i(static_cast<int>(32.8f * 2), 0), 0.1f, false);
    if (!this->_bottomSprite)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");
    this->_sprite->getSprite().scale(sf::Vector2f(3, 3));
    this->_bottomSprite->getSprite().scale(sf::Vector2f(3, 3));

    this->_deathSprite = std::make_unique<rtype::client::utilities::AnimatedSprite>(EXPLOSION_SPRITE, sf::Vector2f(500.0f, 500.0f), sf::Vector2f(32.0f, 32.0f), rtype::client::utilities::AnimatedSprite::HORIZONTAL, sf::Vector2i(static_cast<int>(0), static_cast<int>(0)), sf::Vector2i(static_cast<int>(32.0f * 5), static_cast<int>(0.0f)), 0.1f, false);

    if (!this->_deathSprite)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    std::vector<std::string> username = { "", "V" };

    for (int i = 0; i < username.size(); i++) {
        this->_usernameText.push_back(std::make_unique<rtype::client::utilities::CustomText>(username[i], FONT_PATH, 15, sf::Color::White, sf::Vector2f(this->_position.first, this->_position.second)));

        if (!this->_usernameText.back())
            throw rtype::settings::Errors("Not enough memories", "MemoryError:");
    }

}

rtype::actor::Player::~Player()
{
    //this->_allMissiles.clear();
}

void rtype::actor::Player::moveUp()
{
    if (this->_state == rtype::actor::Actor::DEAD)
        return;

    if (this->_position.second - 3 < 0)
        this->_position = { this->_position.first, 0.0f };
    else
        this->_position = { this->_position.first, this->_position.second - 3 };

    if (this->_direction != UP) {
        this->_direction = UP;
        this->_sprite->updateFrameAnimation(rtype::client::utilities::AnimatedSprite::HORIZONTAL, rtype::client::utilities::AnimatedSprite::direction::RIGHT, sf::Vector2i((int)(32.8 * 2), 0), sf::Vector2i((int)(32.8 * 4), 0), 0.1);
        this->_bottomSprite->updateFrameAnimation(rtype::client::utilities::AnimatedSprite::HORIZONTAL, rtype::client::utilities::AnimatedSprite::direction::RIGHT, sf::Vector2i((int)(32.8 * 2), 0), sf::Vector2i((int)(32.8 * 4), 0), 0.1);
    }

    this->_standClock.restart();
}

void rtype::actor::Player::moveDown()
{
    if (this->_state == rtype::actor::Actor::DEAD)
        return;

    rtype::engine::Engine* engine = rtype::engine::Engine::getInstance();

    float windowHeight = engine->options.getWindowHeight();

    if (this->_position.second + 3 > windowHeight - (windowHeight * 10 / 100))
        this->_position = { this->_position.first, windowHeight - (windowHeight * 10 / 100)};
    else
        this->_position = { this->_position.first, this->_position.second + 3 };

    if (this->_direction != DOWN) {
        this->_direction = DOWN;
        this->_sprite->updateFrameAnimation(rtype::client::utilities::AnimatedSprite::HORIZONTAL, rtype::client::utilities::AnimatedSprite::direction::LEFT, sf::Vector2i((int)(32.8 * 2), 0), sf::Vector2i(0, 0), 0.1);
        this->_bottomSprite->updateFrameAnimation(rtype::client::utilities::AnimatedSprite::HORIZONTAL, rtype::client::utilities::AnimatedSprite::direction::LEFT, sf::Vector2i((int)(32.8 * 2), 0), sf::Vector2i(0, 0), 0.1);
    }

    this->_standClock.restart();
}

void rtype::actor::Player::moveLeft()
{
    if (this->_state == rtype::actor::Actor::DEAD)
        return;

    if (this->_position.first - 3 < 0)
        this->_position = { 0.0f, this->_position.second };
    else
        this->_position = { this->_position.first - 3, this->_position.second };

    if (this->_direction != LEFT && (this->_direction == RIGHT || this->_direction == IDLE))
        this->_direction = LEFT;
}

void rtype::actor::Player::moveRight()
{
    if (this->_state == rtype::actor::Actor::DEAD)
        return;

    rtype::engine::Engine* engine = rtype::engine::Engine::getInstance();
    float windowWidth = (float)engine->options.getWindowWidth();

    if (this->_position.first + 3 > windowWidth)
        this->_position = { windowWidth, this->_position.second };
    else
        this->_position = { this->_position.first + 3, this->_position.second };

    if (this->_direction != RIGHT && (this->_direction == LEFT || this->_direction == IDLE))
        this->_direction = RIGHT;
}

void rtype::actor::Player::shoot(void)
{
    if (this->_state == rtype::actor::Actor::DEAD)
        return;

    #ifdef CLIENT
        rtype::engine::Engine* engine = rtype::engine::Engine::getInstance();
    #endif

    if (!this->_canShoot || this->_shootReloaded < 20)
        return;

    if (!this->_powerup) {
        this->_allMissiles.push_back(std::make_unique<rtype::actor::Missile>(sf::Vector2f(this->_position.first + 10, this->_position.second)));
        if (!this->_allMissiles[_allMissiles.size() - 1])
            throw rtype::settings::Errors("Not enough memories", "MemoryError:");
    } else
        this->_powerup->perform(this);

    #ifdef CLIENT
        if (engine->getClient()->getIsOnline())
            engine->getClient()->askShoot(this->_position.first, this->_position.second);
        this->_shootSound.playSound(engine->options.music.isSoundActive());
    #endif

    this->_canShoot = false;
    this->_shootReloaded -= 20;
}

void rtype::actor::Player::updateMissiles(rtype::engine::Engine* engine)
{
    sf::Time timeWait = this->_shootWaitClock.getElapsedTime();
    sf::Time timeReload = this->_reloadClock.getElapsedTime();

    float TimeWaitSeconds = timeWait.asSeconds();
    float TimeReloadSeconds = timeReload.asSeconds();

    std::cout << "[MISSILES] Avant for" << std::endl;
    std::cout << "[MISSILES] vec.Size: " << this->_allMissiles.size() << std::endl;

    for (size_t i = 0; i < this->_allMissiles.size(); i++) {
        std::cout << "Update du missile (index loop): " << i << std::endl;
        if (!this->_allMissiles[i])
            break;
        this->_allMissiles[i]->update();
        std::cout << "Fin de l'update du mssile (index loop): " << i << std::endl;

        std::cout << "Check si le missile (index)" << i << " est destroy" << std::endl;
        if (this->_allMissiles[i]->isDestroyed()) {
            std::cout << "Le missile (index) " << i << " va etre effacer (vec.erase)" << std::endl;
            this->_allMissiles.erase(this->_allMissiles.begin() + i);
            std::cout << "Le missile (index) " << i << " est effac�" << std::endl;
        }
        else {
            std::cout << "Le missile (index) " << i << " n'a pas besoins d'etre efface" << std::endl;
        }
    }

    std::cout << "[CLOCK] Start" << std::endl;
    if (TimeWaitSeconds > 0.2) {
        this->_canShoot = true;
        this->_shootWaitClock.restart();
    }

    if (TimeReloadSeconds > 0.1) {
        if (this->_shootReloaded + 5 >= 100)
            this->_shootReloaded = 100;
        else
            this->_shootReloaded += 5;
        this->_reloadClock.restart();
    }
    std::cout << "[CLOCK] End" << std::endl;
}

void rtype::actor::Player::updatePlayerState(void)
{
    sf::Time animationTime = this->_invincibleAnimationClock.getElapsedTime();
    float animationMilliseconds = animationTime.asMilliseconds();

    sf::Time time = this->_invincibleClock.getElapsedTime();
    float stateSeconds = time.asSeconds();

    if (this->_playerState == rtype::actor::Player::playerState::INVINCIBLE) {
        if (animationMilliseconds > 200) {
            if (this->_sprite->getOpacity() == 255) {
                this->_bottomSprite->setOpacity(125);
                this->_sprite->setOpacity(125);
            } 
            else {
                this->_bottomSprite->setOpacity(255);
                this->_sprite->setOpacity(255);
            }
            this->_invincibleAnimationClock.restart();
        }

        if (stateSeconds > 4) {
            this->_bottomSprite->setOpacity(255);
            this->_sprite->setOpacity(255);
            this->_invincibleClock.restart();
            this->_playerState = rtype::actor::Player::playerState::NORMAL;
        }
    }
}

void rtype::actor::Player::update(rtype::engine::Engine* engine)
{
    sf::Time time = this->_standClock.getElapsedTime();
    float seconds = time.asSeconds();

    if (seconds > 0.3) {
        this->_direction = IDLE;
        this->_sprite->updateFrameAnimation(rtype::client::utilities::AnimatedSprite::HORIZONTAL, rtype::client::utilities::AnimatedSprite::direction::LEFT, sf::Vector2i((int)(32.8 * 2), 0), sf::Vector2i((int)(32.8 * 2), 0), 0.1);
        this->_bottomSprite->updateFrameAnimation(rtype::client::utilities::AnimatedSprite::HORIZONTAL, rtype::client::utilities::AnimatedSprite::direction::LEFT, sf::Vector2i((int)(32.8 * 2), 0), sf::Vector2i((int)(32.8 * 2), 0), 0.1);
    }
    std::cout << "[MISSILES] Updating..." << std::endl;
    updateMissiles(engine);
    std::cout << "[MISSILES] Updated" << std::endl;

    this->_sprite->setPosition(sf::Vector2f(this->_position.first, this->_position.second));
    this->_bottomSprite->setPosition(sf::Vector2f(this->_position.first, this->_position.second));

    this->_usernameText[0]->setPosition(sf::Vector2f(this->_position.first, this->_position.second - 50));
    this->_usernameText[1]->setPosition(sf::Vector2f(this->_position.first, this->_position.second - 30));

    this->updatePlayerState();
    this->_sprite->updateAnimation();
    this->_bottomSprite->updateAnimation();

}

void rtype::actor::Player::draw(rtype::engine::Engine* engine)
{
    this->_sprite->draw(engine);
    this->_bottomSprite->draw(engine);

    for (size_t i = 0; i < this->_allMissiles.size(); i++)
        this->_allMissiles[i]->draw(engine);
}

int rtype::actor::Player::getHealth(void)
{
    return this->_health;
}

int rtype::actor::Player::getID(void)
{
    return this->_id;
}

std::string rtype::actor::Player::getName()
{
    return this->_name;
}

sf::Vector2f rtype::actor::Player::getPosition()
{
    return { this->_position.first, this->_position.second };
}

void rtype::actor::Player::setPosition(sf::Vector2f position)
{
    this->_position = { position.x, position.y };
}

void rtype::actor::Player::setPlayerName(std::string name)
{
    this->_name = name;
    this->_usernameText[0]->setText(name);
}

std::vector<std::unique_ptr<rtype::actor::Missile>>& rtype::actor::Player::getMissiles()
{
    return this->_allMissiles;
}

void rtype::actor::Player::setHealth(int healthNumber)
{
    if (healthNumber <= 0)
        this->_health = 0;
    else if (healthNumber >= 5)
        this->_health = 5;
    else
        this->_health = healthNumber;
}

void rtype::actor::Player::drawUsername(rtype::engine::Engine* engine)
{
    for (int i = 0; i < this->_usernameText.size(); i++)
        this->_usernameText[i]->draw(engine);
}

void rtype::actor::Player::takeDamage(int damage)
{
    if (this->_playerState == rtype::actor::Player::playerState::INVINCIBLE || this->_state == rtype::actor::Actor::DEAD)
        return;

    if (this->_health - damage <= 0) {
        this->_health = 0;
        this->_state = rtype::actor::Actor::DEAD;
    }
    else
        this->_health -= damage;


    this->_playerState = rtype::actor::Player::playerState::INVINCIBLE;
    this->_invincibleAnimationClock.restart();
    this->_invincibleClock.restart();
    #ifdef CLIENT
        this->_damageSound.playSound(rtype::engine::Engine::getInstance()->options.music.isSoundActive());
    #endif // CLIENT


}

void rtype::actor::Player::setDamage(int damage)
{
    this->_strength = damage;
}

int rtype::actor::Player::getDamage(void)
{
    return this->_strength;
}

float rtype::actor::Player::getReloadedPercentage(void)
{
    return this->_shootReloaded;
}

int rtype::actor::Player::getScore(void)
{
    return this->_score;
}

void rtype::actor::Player::addScore(int scoreAdded)
{
    this->_score += scoreAdded;
}

void rtype::actor::Player::subScore(int scoreSubstracted)
{
    if (this->_score - scoreSubstracted <= 0)
        this->_score = 0;
    else
        this->_score -= scoreSubstracted;
}

void rtype::actor::Player::setScore(int score)
{
    if (score <= 0)
        this->_score = 0;
    else
        this->_score = score;
}

MoveDirection rtype::actor::Player::getDirection(void)
{
    return this->_direction;
}

void rtype::actor::Player::setDirection(MoveDirection direction)
{
    MoveDirection oldDirection = this->_direction;
    this->_direction = direction;

    if (this->_direction == DOWN || this->_direction == UP) {
        if (this->_direction == DOWN && this->_direction != oldDirection) {
            this->_sprite->updateFrameAnimation(rtype::client::utilities::AnimatedSprite::HORIZONTAL, rtype::client::utilities::AnimatedSprite::direction::LEFT, sf::Vector2i((int)(32.8 * 2), 0), sf::Vector2i(0, 0), 0.1);
            this->_bottomSprite->updateFrameAnimation(rtype::client::utilities::AnimatedSprite::HORIZONTAL, rtype::client::utilities::AnimatedSprite::direction::LEFT, sf::Vector2i((int)(32.8 * 2), 0), sf::Vector2i(0, 0), 0.1);
        }

        if (this->_direction == UP && this->_direction != oldDirection) {
            this->_sprite->updateFrameAnimation(rtype::client::utilities::AnimatedSprite::HORIZONTAL, rtype::client::utilities::AnimatedSprite::direction::RIGHT, sf::Vector2i((int)(32.8 * 2), 0), sf::Vector2i((int)(32.8 * 4), 0), 0.1);
            this->_bottomSprite->updateFrameAnimation(rtype::client::utilities::AnimatedSprite::HORIZONTAL, rtype::client::utilities::AnimatedSprite::direction::RIGHT, sf::Vector2i((int)(32.8 * 2), 0), sf::Vector2i((int)(32.8 * 4), 0), 0.1);
        }

        this->_standClock.restart();
    }
}

sf::Sprite& rtype::actor::Player::getSprite(void)
{
    return this->_sprite->getSprite();
}

bool rtype::actor::Player::isMissileCollision(rtype::actor::Missile *missile)
{
    if (this->_sprite->getSprite().getGlobalBounds().intersects(missile->getSprite().getGlobalBounds()) && this->_playerState == rtype::actor::Player::playerState::NORMAL)
        return true;
    return false;
}

bool rtype::actor::Player::isActorCollision(rtype::actor::Actor* actor)
{
    if (this->_sprite->getSprite().getGlobalBounds().intersects(actor->getSprite().getGlobalBounds()) && this->_playerState == rtype::actor::Player::playerState::NORMAL)
        return true;
    return false;
}

bool rtype::actor::Player::isAsteroidCollision(rtype::actor::Asteroid* asteroid)
{
    if (this->_sprite->getSprite().getGlobalBounds().intersects(asteroid->getBounds()) && this->_playerState == rtype::actor::Player::playerState::NORMAL && asteroid->getState() == rtype::actor::Asteroid::ALIVE)
        return true;
    return false;
}

bool rtype::actor::Player::isPowerUpCollision(std::unique_ptr<rtype::powerup::APowerUpItem> &powerup)
{
    return (this->_sprite->getSprite().getGlobalBounds().intersects(powerup->getSprite().getSprite().getGlobalBounds()) &&
    this->_state == rtype::actor::Actor::actorState::ALIVE && powerup->hasSpawned() && powerup->isVisible());
}

rtype::actor::Actor::actorState rtype::actor::Player::getState(void)
{
    return this->_state;
}

void rtype::actor::Player::setEntityID(std::string ID)
{
    if (ID.length() > 10)
        return;
    this->_entityID = ID;
}

std::string rtype::actor::Player::getEntityID(void)
{
    return this->_entityID;
}

void rtype::actor::Player::setPlayerColor(sf::Color color)
{
    this->_sprite->setColor(color);
}

void rtype::actor::Player::setPowerUpAction(std::string powerUpName)
{
    if (this->_powerup)
        return;
    this->_powerup = rtype::powerup::PowerUpActionFactory::getPowerUpAction(powerUpName);
}
rtype::actor::Player::playerState rtype::actor::Player::getStatus(void)
{
    return this->_playerState;
}
