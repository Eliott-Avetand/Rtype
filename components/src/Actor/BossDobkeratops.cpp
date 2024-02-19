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
//  File:            BossDobkeratops.cpp                                             //
//  Description:     First boss of the rtype                                         //
//                                                                                   //
// ********************************************************************************* //
#include "BossDobkeratops.hpp"
#include "RandomNumbers.hpp"

#ifdef SERVER
#include "UDPServer.hpp"
#include "Room.hpp"
#endif // SERVER


rtype::actor::BossDobkeratops::BossDobkeratops(rtype::engine::Engine *engine) :
    _soundRoar(BOSS_ROAR)
{
    _sprite = std::make_unique<rtype::client::utilities::AnimatedSprite>("client/assets/sprites/monsters/boss.png", sf::Vector2f(2500.0f, 500.0f), sf::Vector2f(162.0f, 213.0f), rtype::client::utilities::AnimatedSprite::HORIZONTAL, sf::Vector2i(static_cast<int>(162), static_cast<int>(213)), sf::Vector2i(static_cast<int>(162 * 4), static_cast<int>(213 * 8)), 0.1f, true);
    _sprite->setScale(4.0f, 4.0f);
    _dobkeratopsHeart.push_back(std::make_unique<DobkeratopsHeart>(sf::Vector2f(2500.0f, 250.0f), sf::Vector2f(200.0f, 36.0f), sf::Vector2i(static_cast<int>(146), static_cast<int>(1926)), sf::Vector2i(static_cast<int>(146), static_cast<int>(1926))));
    _dobkeratopsHeart.push_back(std::make_unique<DobkeratopsHeart>(sf::Vector2f(2600.0f, 600.0f), sf::Vector2f(113.0f, 36.0f), sf::Vector2i(static_cast<int>(234), static_cast<int>(2053)), sf::Vector2i(static_cast<int>(234), static_cast<int>(2053))));
    _dobkeratopsHeart.push_back(std::make_unique<DobkeratopsHeart>(sf::Vector2f(2450.0f, 880.0f), sf::Vector2f(200.0f, 36.0f), sf::Vector2i(static_cast<int>(146), static_cast<int>(2100)), sf::Vector2i(static_cast<int>(146), static_cast<int>(2100))));
    
    #ifdef SERVER
    _dobkeratopsHeart[0]->_id = UDPServer::generateID(10);
    _dobkeratopsHeart[1]->_id = UDPServer::generateID(10);
    _dobkeratopsHeart[2]->_id = UDPServer::generateID(10);
    #endif // SERVER

    _balls = {
        {SHORT, {sf::Vector2f(14, 14), sf::Vector2f(574, 2031)}},
        {MEDIUM, {sf::Vector2f(16, 16), sf::Vector2f(548, 2016)}},
        {BIG, {sf::Vector2f(19, 19), sf::Vector2f(518, 2000)}},
        {END, {sf::Vector2f(19, 19), sf::Vector2f(483, 2044)}}
    };
    _tailCreationClock = sf::Clock();
    _tailClock = sf::Clock();
    _moveClock = sf::Clock();
    _shootClock = sf::Clock();
    _dyingClock = sf::Clock();
    _explosionClock = sf::Clock();
    _missileClock = sf::Clock();
    _tailSpeed = .4;
    _tailCounter = 0;
    _position = _sprite->getPosition();
    _damage = 1;
    _score = 5000;
    _state = SPAWN;
    #ifdef CLIENT
        engine->options.music.changeMusic(BOSS_OST);
    #endif
}

int rtype::actor::BossDobkeratops::getDamage()
{
    return _damage;
}

bool rtype::actor::BossDobkeratops::isSpawned()
{
    return _state == ALIVE;
}

void rtype::actor::BossDobkeratops::setDamage(int damage)
{
    _damage = damage;
}

void rtype::actor::BossDobkeratops::setPosition(sf::Vector2f position)
{
    _sprite->setPosition(position);
}

sf::Vector2f rtype::actor::BossDobkeratops::getPosition(void)
{
    return _position;
}

int rtype::actor::BossDobkeratops::getScore()
{
    return _score;
}

bool rtype::actor::BossDobkeratops::isDead()
{
    return _state == DEAD;
}

bool rtype::actor::BossDobkeratops::isDying()
{
    return this->_state == DYING;
}

bool rtype::actor::BossDobkeratops::isTailCollision(rtype::actor::Missile *missile)
{
    for (int i = 0; i < _tail.size(); i++) {
        if (this->_tail[i]->getSprite().getGlobalBounds().intersects(missile->getSprite().getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

#ifdef SERVER
bool rtype::actor::BossDobkeratops::isMissileCollision(rtype::actor::Missile* missile, Room *room, int playerID)
{
    for (int i = 0; i < _dobkeratopsHeart.size(); i++) {
        if (_dobkeratopsHeart[i]->getSprite().getGlobalBounds().intersects(missile->getSprite().getGlobalBounds())
            && _state == ALIVE) {
            if (room)
                room->sendBossDamage(_dobkeratopsHeart[i]->_id, playerID);
            _dobkeratopsHeart[i]->takeDamage(nullptr, nbrHeartsAlive() == 0, 1);
            if (_dobkeratopsHeart[i]->isDying() && nbrHeartsAlive() == 0) {
                _state = DYING;
                _dyingClock.restart();
            }
            return true;
        }
    }
    return false;
}
#endif // SERVER

bool rtype::actor::BossDobkeratops::isMissileCollision(rtype::actor::Missile *missile, rtype::engine::Engine *engine)
{
    for (int i = 0; i < _dobkeratopsHeart.size(); i++) {
        if (_dobkeratopsHeart[i]->getSprite().getGlobalBounds().intersects(missile->getSprite().getGlobalBounds())
            && _state == ALIVE) {
            #ifdef CLIENT
            if (!engine->getClient()->getIsOnline()) {
                _dobkeratopsHeart[i]->takeDamage(engine, nbrHeartsAlive() == 0, 1);
            }
            #endif // CLIENT
            #ifdef SERVER
            _dobkeratopsHeart[i]->takeDamage(engine, nbrHeartsAlive() == 0, 1);
            #endif // SERVER
            if (_dobkeratopsHeart[i]->isDying() && nbrHeartsAlive() == 0) {
                #ifdef CLIENT
                    _soundRoar.playSound(engine->options.music.isSoundActive());
                #endif
                _state = DYING;
                _dyingClock.restart();
            }
            return true;
        }
    }
    return false;
}

bool rtype::actor::BossDobkeratops::isPlayerCollision(std::unique_ptr<rtype::actor::Actor> &actor)
{
    rtype::actor::Player *playerPtr = dynamic_cast<rtype::actor::Player *>(actor.get());
    if (!playerPtr)
        throw rtype::settings::Errors("Can't dynamic cast player", "MemoryError:");
    if (this->_sprite->getSprite().getGlobalBounds().intersects(actor->getSprite().getGlobalBounds()) && playerPtr->getStatus() == rtype::actor::Player::NORMAL)
        return true;
    for (int i = 0; i < _tail.size(); i++) {
        if (this->_tail[i]->getSprite().getGlobalBounds().intersects(actor->getSprite().getGlobalBounds()) && playerPtr->getStatus() == rtype::actor::Player::NORMAL)
            return true;
    }
    for (int i = 0; i < _allMissiles.size(); i++) {
        if (this->_allMissiles[i]->getSprite().getGlobalBounds().intersects(actor->getSprite().getGlobalBounds()) && playerPtr->getStatus() == rtype::actor::Player::NORMAL)
            return true;
    }
    for (int i = 0; i < _dobkeratopsHeart.size(); i++)
        if (_dobkeratopsHeart[i]->isPlayerCollision(actor))
            return true;
    return false;
}

void rtype::actor::BossDobkeratops::die(rtype::engine::Engine *engine)
{
    if (_dyingClock.getElapsedTime().asSeconds() < 3) {
        #ifdef CLIENT
            sf::View view = engine->window.getDefaultView();
            view.move(sf::Vector2f(
                rtype::client::utilities::RandomNumbers::randomFloat(-20, 20),
                rtype::client::utilities::RandomNumbers::randomFloat(-20, 20)
            ));
            engine->window.setView(view);
        #endif // CLIENT
        if (_explosionClock.getElapsedTime().asMilliseconds() > 50) {
            sf::Vector2f position = sf::Vector2f(
                rtype::client::utilities::RandomNumbers::randomInteger(_sprite->getPosition().x - _sprite->getWidth() / 2, _sprite->getPosition().x - _sprite->getWidth() / 2 + _sprite->getWidth()),
                rtype::client::utilities::RandomNumbers::randomInteger(_sprite->getPosition().y - _sprite->getHeight() / 2, _sprite->getPosition().y - _sprite->getHeight() / 2 + _sprite->getHeight())
            );
            _spriteExplosion.push_back(std::make_unique<rtype::client::utilities::AnimatedSprite>(EXPLOSION_SPRITE, sf::Vector2f(500.0f, 500.0f), sf::Vector2f(32.0f, 32.0f), rtype::client::utilities::AnimatedSprite::HORIZONTAL, sf::Vector2i(static_cast<int>(0), static_cast<int>(0)), sf::Vector2i(static_cast<int>(32.0f * 5), static_cast<int>(0.0f)), 0.1f, false));
            _spriteExplosion.back()->setPosition(position);
            _spriteExplosion.back()->setScale(3.0f, 3.0f);
        }
    }
}

void rtype::actor::BossDobkeratops::shoot(void)
{
    this->_allMissiles.push_back(std::make_unique<rtype::actor::MissileBoss>(sf::Vector2f(1500.0f, 500.0f)));

    if (!this->_allMissiles[_allMissiles.size() - 1])
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    #ifdef SERVER
        this->_allMissiles.back()->setId(UDPServer::generateID(10));
    #endif // SERVER
}

void rtype::actor::BossDobkeratops::spawn(rtype::engine::Engine *engine)
{
    sf::Time timeMove = _moveClock.getElapsedTime();
    sf::Time timeCreation = _tailCreationClock.getElapsedTime();
    float millisecondsCreation = timeCreation.asMilliseconds();
	float millisecondsMove = timeMove.asMilliseconds();

    if (millisecondsCreation > 100 && _tailCounter < 20 && _state == SPAWN) {
        std::vector<sf::Vector2f> data = _balls[static_cast<tailSprites>(_tailCounter < 19 ? rtype::client::utilities::RandomNumbers::randomInteger(0, 3) : 3)];
        _tail.push_back(std::make_unique<rtype::client::utilities::AnimatedSprite>("client/assets/sprites/monsters/boss.png", sf::Vector2f(2400.0f - (_tailCounter * 50), 850.0f), data[0], rtype::client::utilities::AnimatedSprite::HORIZONTAL, sf::Vector2i(static_cast<int>(data[1].x), static_cast<int>(data[1].y)), sf::Vector2i(static_cast<int>(data[1].x), static_cast<int>(data[1].y)), 0.1f, true));
        _tail.back()->setScale(4.0f, 4.0f);
        _tailCounter++;
        _tailCreationTime.push_back(sf::Clock());
        _tailCreationClock.restart();
    }
    if (_tailCounter >= 19 && millisecondsMove > 10 && _state == SPAWN) {
		_sprite->setPosition(sf::Vector2f(_sprite->getPosition().x - 5, _sprite->getPosition().y));
        for (int i = 0; i < _tail.size(); i++)
            _tail[i]->getSprite().setPosition(sf::Vector2f(_tail[i]->getPosition().x - 5, _tail[i]->getPosition().y));
		for (int i = 0; i < _dobkeratopsHeart.size(); i++)
            _dobkeratopsHeart[i]->setPosition(sf::Vector2f(_dobkeratopsHeart[i]->getPosition().x - 5, _dobkeratopsHeart[i]->getPosition().y));
        _moveClock.restart();
	}
    if (_sprite->getSprite().getPosition().x <= 1500 && _state == SPAWN) {
        #ifdef CLIENT
            _soundRoar.playSound(engine->options.music.isSoundActive());
        #endif // CLIENT
        _moveClock.restart();
        _state = STOPPED;
    }
    if (_state == STOPPED) {
        if (_moveClock.getElapsedTime().asSeconds() < 3) {
            #ifdef CLIENT
                sf::View view = engine->window.getDefaultView();
                view.move(sf::Vector2f(
                    rtype::client::utilities::RandomNumbers::randomFloat(-20, 20),
                    rtype::client::utilities::RandomNumbers::randomFloat(-20, 20)
                ));
                engine->window.setView(view);
            #endif
        } else {
            for (int i = 0; i < _dobkeratopsHeart.size(); i++)
                _dobkeratopsHeart[i]->setAlive();
            _state = ALIVE;
            #ifdef CLIENT
                engine->window.setView(engine->window.getDefaultView());
            #endif // CLIENT
        }
    }
}

#ifdef SERVER
void rtype::actor::BossDobkeratops::updateMissiles(Room* room)
{
    for (size_t i = 0; i < this->_allMissiles.size(); i++) {
        this->_allMissiles[i]->update();

        if (this->_allMissiles[i]->isDestroyed()) {
            if (room)
                room->sendBossMissileDestroyed(this->_allMissiles[i]->getId());
            this->_allMissiles.erase(this->_allMissiles.begin() + i);
        }
    }

    if (_shootClock.getElapsedTime().asSeconds() > 4 && _state == ALIVE) {
        if (_missileClock.getElapsedTime().asMilliseconds() > 50) {
            shoot();
            _missileClock.restart();
        }
        if (_allMissiles.size() >= 5)
            _shootClock.restart();
    }
}
#endif // SERVER

void rtype::actor::BossDobkeratops::updateMissiles(rtype::engine::Engine* engine)
{
    for (size_t i = 0; i < this->_allMissiles.size(); i++) {
        
        this->_allMissiles[i]->update();

        if (this->_allMissiles[i]->isDestroyed()) {

        #ifdef CLIENT

            this->_allMissiles.erase(this->_allMissiles.begin() + i);

        #endif // CLIENT

        }
    }

    #ifdef CLIENT
        if (!engine->getClient()->getIsOnline()) {
            if (_shootClock.getElapsedTime().asSeconds() > 4 && _state == ALIVE) {
                if (_missileClock.getElapsedTime().asMilliseconds() > 50) {
                    shoot();
                    _missileClock.restart();
                }
                if (_allMissiles.size() >= 5)
                    _shootClock.restart();
                }
            return;
        }
    #endif // CLIENT
}

#ifdef SERVER
void rtype::actor::BossDobkeratops::update(Room *room)
{
    sf::Time time = _tailClock.getElapsedTime();
    float seconds = time.asSeconds();

    if (_state == ALIVE || _state == SPAWN || _state == STOPPED) {
        for (int i = 0; i < _tail.size(); i++) {
            double tmp = _tailCreationTime[i].getElapsedTime().asSeconds();
            double sinusy = (double)i / 5.0f * sin(tmp);
            double sinusx = (double)i / 2.0f * sin(tmp);
            sf::Vector2f tailPosition = _tail[i]->getPosition();

            double x = tailPosition.x + sinusx;
            double y = tailPosition.y - sinusy;

            _tail[i]->setPosition(sf::Vector2f(x, y));
            _tail[i]->updateAnimation();
        }
    }
    this->updateMissiles(room);
    _sprite->updateAnimation();
    if (_state == DYING)
        die(nullptr);
    for (int i = 0; i < _spriteExplosion.size(); i++) {
        if (_spriteExplosion[i]->isAnimationFinish())
            _spriteExplosion.erase(_spriteExplosion.begin() + i);
        else
            _spriteExplosion[i]->updateAnimation();
    }
    if (_state == DYING && _dyingClock.getElapsedTime().asSeconds() > 3 && _spriteExplosion.empty()) {
        _state = DEAD;
    }
    for (int i = 0; i < _dobkeratopsHeart.size(); i++) {
        if (!_dobkeratopsHeart[i]->isDead())
            _dobkeratopsHeart[i]->update(room);
    }
}
#endif // SERVER


void rtype::actor::BossDobkeratops::update(rtype::engine::Engine* engine)
{
    sf::Time time = _tailClock.getElapsedTime();
    float seconds = time.asSeconds();

    if (_state == ALIVE || _state == SPAWN || _state == STOPPED) {
        for (int i = 0; i < _tail.size(); i++) {
            double tmp = _tailCreationTime[i].getElapsedTime().asSeconds();
            double sinusy = (double)i / 5.0f * sin(tmp);
            double sinusx = (double)i / 2.0f * sin(tmp);
            sf::Vector2f tailPosition = _tail[i]->getPosition();

            double x = tailPosition.x + sinusx;
            double y = tailPosition.y - sinusy;

            #ifdef CLIENT
                if (!engine->getClient()->getIsOnline()) {
                    _tail[i]->setPosition(sf::Vector2f(x, y));
                    _tail[i]->updateAnimation();
                }
            #endif // CLIENT
        }
    }
    this->updateMissiles(engine);
    _sprite->updateAnimation();
    if (_state == DYING)
        die(engine);
    for (int i = 0; i < _spriteExplosion.size(); i++) {
        if (_spriteExplosion[i]->isAnimationFinish())
            _spriteExplosion.erase(_spriteExplosion.begin() + i);
        else
            _spriteExplosion[i]->updateAnimation();
    }
    if (_state == DYING && _dyingClock.getElapsedTime().asSeconds() > 3 && _spriteExplosion.empty()) {
        _state = DEAD;
    }
    for (int i = 0; i < _dobkeratopsHeart.size(); i++) {
        if (!_dobkeratopsHeart[i]->isDead())
            _dobkeratopsHeart[i]->update(engine);
    }
}

std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& rtype::actor::BossDobkeratops::getMissile()
{
    return this->_allMissiles;
}

std::vector<std::unique_ptr<rtype::client::utilities::AnimatedSprite>>& rtype::actor::BossDobkeratops::getTails(void)
{
    return this->_tail;
}

std::vector<std::unique_ptr<rtype::actor::DobkeratopsHeart>>& rtype::actor::BossDobkeratops::getHearts()
{
    return this->_dobkeratopsHeart;
}

void rtype::actor::BossDobkeratops::draw(rtype::engine::Engine* engine)
{
    if (_state != DEAD) {
        _sprite->draw(engine);
        for (int i = 0; i< _dobkeratopsHeart.size(); i++)
            _dobkeratopsHeart[i]->draw(engine);
        for (int i = 0; i < _tail.size(); i++)
            _tail[i]->draw(engine);
        for (size_t i = 0; i < this->_allMissiles.size(); i++)
            this->_allMissiles[i]->draw(engine);
        for (int i = 0; i < _spriteExplosion.size(); i++)
            _spriteExplosion[i]->draw(engine);
    }
}

rtype::actor::DobkeratopsHeart::DobkeratopsHeart(sf::Vector2f position, sf::Vector2f react, sf::Vector2i animationBegin, sf::Vector2i animationEnd) :
    _deathSound(BOSS_ROAR)
{
    _sprite = std::make_unique<rtype::client::utilities::AnimatedSprite>("client/assets/sprites/monsters/boss.png", position, react, rtype::client::utilities::AnimatedSprite::HORIZONTAL, animationBegin, animationBegin, 0.1f, true);
    _sprite->setScale(4.0f, 4.0f);
    _position = position;
    _health = 5;
    _state = SPAWN;
}

int rtype::actor::DobkeratopsHeart::getDamage()
{
    return _damage;
}

sf::Sprite &rtype::actor::DobkeratopsHeart::getSprite()
{
    return _sprite->getSprite();
}

void rtype::actor::DobkeratopsHeart::takeDamage(rtype::engine::Engine *engine, bool isLastHeart, int damage)
{
    if (_state == INVINCIBLE || _state == DEAD || _state == DYING)
        return;

    if (_health - damage <= 0) {
        _health = 0;
        #ifdef CLIENT
            if (!isLastHeart)
                _deathSound.playSound(engine->options.music.isSoundActive());
        #endif // CLIENT
        _state = DYING;
        _dyingClock.restart();
    } else {
        _state = INVINCIBLE;
        _animationClock.restart();
        _invincibleClock.restart();
        _health--;
    }
}

void rtype::actor::DobkeratopsHeart::setAlive()
{
    _state = ALIVE;
}

bool rtype::actor::DobkeratopsHeart::isDead()
{
    return _state == DEAD;
}

bool rtype::actor::DobkeratopsHeart::isDying()
{
    return _state == DYING;
}

bool rtype::actor::DobkeratopsHeart::isPlayerCollision(std::unique_ptr<rtype::actor::Actor> &actor)
{
    rtype::actor::Player *playerPtr = dynamic_cast<rtype::actor::Player *>(actor.get());
    if (!playerPtr)
        throw rtype::settings::Errors("Can't dynamic cast player", "MemoryError:");
    for (int i = 0; i < _allMissiles.size(); i++) {
        if (this->_allMissiles[i]->getSprite().getGlobalBounds().intersects(actor->getSprite().getGlobalBounds()) && playerPtr->getStatus() == rtype::actor::Player::NORMAL)
            return true;
    }
    return false;
}

std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& rtype::actor::DobkeratopsHeart::getMissile()
{
    return this->_allMissiles;
}

rtype::actor::DobkeratopsHeart::state rtype::actor::DobkeratopsHeart::getState()
{
    return _state;
}

void rtype::actor::DobkeratopsHeart::die(rtype::engine::Engine *engine)
{
    if (_dyingClock.getElapsedTime().asSeconds() < 3) {
        #ifdef CLIENT
            sf::View view = engine->window.getDefaultView();
            view.move(sf::Vector2f(
                rtype::client::utilities::RandomNumbers::randomFloat(-5, 5),
                rtype::client::utilities::RandomNumbers::randomFloat(-5, 5)
            ));
            engine->window.setView(view);
        #endif // CLIENT
        if (_explosionClock.getElapsedTime().asMilliseconds() > 50) {
            sf::Vector2f position = sf::Vector2f(
                rtype::client::utilities::RandomNumbers::randomInteger(_sprite->getPosition().x - _sprite->getWidth() / 2, _sprite->getPosition().x - _sprite->getWidth() / 2 + _sprite->getWidth()),
                rtype::client::utilities::RandomNumbers::randomInteger(_sprite->getPosition().y - _sprite->getHeight() / 2, _sprite->getPosition().y - _sprite->getHeight() / 2 + _sprite->getHeight())
            );
            _spriteExplosion.push_back(std::make_unique<rtype::client::utilities::AnimatedSprite>(EXPLOSION_SPRITE, sf::Vector2f(500.0f, 500.0f), sf::Vector2f(32.0f, 32.0f), rtype::client::utilities::AnimatedSprite::HORIZONTAL, sf::Vector2i(static_cast<int>(0), static_cast<int>(0)), sf::Vector2i(static_cast<int>(32.0f * 5), static_cast<int>(0.0f)), 0.1f, false));
            _spriteExplosion.back()->setPosition(position);
            _spriteExplosion.back()->setScale(3.0f, 3.0f);
        }
    }
}

void rtype::actor::DobkeratopsHeart::shoot(void)
{
    this->_allMissiles.push_back(std::make_unique<rtype::actor::MissileBoss>(_position));

    if (!this->_allMissiles[_allMissiles.size() - 1])
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    #ifdef SERVER
        this->_allMissiles.back()->setId(UDPServer::generateID(10));
    #endif // SERVER
}

sf::Vector2f rtype::actor::DobkeratopsHeart::getPosition(void)
{
    return _position;
}

void rtype::actor::DobkeratopsHeart::setPosition(sf::Vector2f position)
{
    _position = position;
    _sprite->setPosition(position);
}

#ifdef SERVER
void rtype::actor::DobkeratopsHeart::updateMissiles(Room *room)
{
    for (size_t i = 0; i < this->_allMissiles.size(); i++) {
        this->_allMissiles[i]->update();

        if (this->_allMissiles[i]->isDestroyed()) {
            if (room)
                room->sendBossHeartMissileDestroyed(this->_id, this->_allMissiles[i]->getId());
            this->_allMissiles.erase(this->_allMissiles.begin() + i);
        }
    }

    if (_shootClock.getElapsedTime().asSeconds() > rtype::client::utilities::RandomNumbers::randomInteger(2, 6) && _state == ALIVE) {
        if (_missileClock.getElapsedTime().asMilliseconds() > 50) {
            shoot();
            _missileClock.restart();
        }
        if (_allMissiles.size() >= 5)
            _shootClock.restart();
    }
}
#endif

int rtype::actor::BossDobkeratops::nbrHeartsAlive(void)
{
    int nbr = 0;

    for (int i = 0; i < this->_dobkeratopsHeart.size(); i++) {
        if (!this->_dobkeratopsHeart[i]->isDying() && !this->_dobkeratopsHeart[i]->isDead())
            nbr++;
    }

    return nbr;
}

void rtype::actor::DobkeratopsHeart::updateMissiles(rtype::engine::Engine* engine)
{
    for (size_t i = 0; i < this->_allMissiles.size(); i++) {
        this->_allMissiles[i]->update();

        if (this->_allMissiles[i]->isDestroyed()) {
            #ifdef CLIENT
                this->_allMissiles.erase(this->_allMissiles.begin() + i);
            #endif // CLIENT
        }
    }

    #ifdef CLIENT
        if (!engine->getClient()->getIsOnline()) {
            if (_shootClock.getElapsedTime().asSeconds() > rtype::client::utilities::RandomNumbers::randomInteger(2, 6) && _state == ALIVE) {
                if (_missileClock.getElapsedTime().asMilliseconds() > 50) {
                    shoot();
                    _missileClock.restart();
    }
                if (_allMissiles.size() >= 5)
                    _shootClock.restart();
            }
            return;
        }
    #endif // SERVER
}

void rtype::actor::DobkeratopsHeart::updateInvincible()
{
    if (_state == INVINCIBLE) {
        if (_animationClock.getElapsedTime().asMilliseconds() > 200) {
            if (this->_sprite->getOpacity() == 255)
                this->_sprite->setOpacity(125);
            else
                this->_sprite->setOpacity(255);
            _animationClock.restart();
        }
        if (_invincibleClock.getElapsedTime().asSeconds() > 3) {
            this->_sprite->setOpacity(255);
            this->_invincibleClock.restart();
            _state = ALIVE;
        }
    }
}

#ifdef SERVER
void rtype::actor::DobkeratopsHeart::update(Room *room)
{
    updateMissiles(room);
    updateInvincible();
    if (_state == DYING)
        die(nullptr);
    for (int i = 0; i < _spriteExplosion.size(); i++) {
        if (_spriteExplosion[i]->isAnimationFinish())
            _spriteExplosion.erase(_spriteExplosion.begin() + i);
        else
            _spriteExplosion[i]->updateAnimation();
    }
    if (_state == DYING && _dyingClock.getElapsedTime().asSeconds() > 2 && _spriteExplosion.empty()) {
        _state = DEAD;
        if (room)
            room->sendBossHeartDestroy(this->_id);
    }
}
#endif // SERVER


void rtype::actor::DobkeratopsHeart::update(rtype::engine::Engine *engine)
{
    updateMissiles(engine);
    updateInvincible();
    if (_state == DYING)
        die(engine);
    for (int i = 0; i < _spriteExplosion.size(); i++) {
        if (_spriteExplosion[i]->isAnimationFinish())
            _spriteExplosion.erase(_spriteExplosion.begin() + i);
        else
            _spriteExplosion[i]->updateAnimation();
    }
    if (_state == DYING && _dyingClock.getElapsedTime().asSeconds() > 2 && _spriteExplosion.empty()) {
        _state = DEAD;
    }
}

void rtype::actor::DobkeratopsHeart::draw(rtype::engine::Engine* engine)
{
    if (_state != DEAD) {
        _sprite->draw(engine);
        for (size_t i = 0; i < this->_allMissiles.size(); i++)
            this->_allMissiles[i]->draw(engine);
        for (int i = 0; i < _spriteExplosion.size(); i++)
            _spriteExplosion[i]->draw(engine);
    }
}