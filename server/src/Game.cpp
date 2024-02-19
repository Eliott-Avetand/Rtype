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
//  File:            Game.cpp                                                        //
//  Description:     Handle the game instance in the server                          //
//                                                                                   //
//                                                                                   //
// ********************************************************************************* //

#include "Game.hpp"
#include "UDPServer.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

Game::Game(unsigned short levelID, size_t playersCount, Room *room)
{
    std::vector<int> positionFactor = { 50, 40, 30, 20 };

	this->_levelID = levelID;
    this->_breedNumber = 0;
    this->_playerNumber = playersCount;
    this->_room = room;
    this->_levelStatus = LevelState::INGAME;

    this->_boss = nullptr;
    this->_isBoss = false;
    this->_resetVariable = false;

    this->_asteroidsTime = rtype::client::utilities::RandomNumbers::randomFloat(2, 5);
    this->_asteroidsClock.restart();

    for (int i = 0; i < this->_playerNumber; i++) {
        sf::Vector2f pos = { 200, 1080 * (positionFactor[this->_playerNumber - 1] * float(i + 1)) / 100 - 1080 * 10 / 100 };

        this->_players.push_back(std::make_unique<rtype::actor::Player>("", i));

        if (!this->_players[i])
            throw rtype::settings::Errors("Not enough memories", "MemoryError:");

        this->_players[i]->setPosition(pos);
    }

    this->createEnemies();
    this->initBoss();
}

Game::~Game()
{

}

void Game::initBoss()
{
    try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json("data/level_" + std::to_string(this->_levelID) + "/level.json", root);

        this->_isBoss = root.get_child("boss").data() == "true";
    }
    catch (std::exception const& error) {
        throw rtype::settings::Errors("Errors in file", "Level.json");
    }
}

bool Game::checkNextGameLevel(void)
{
    try {
        boost::property_tree::ptree root;

        boost::property_tree::read_json("data/level_" + std::to_string(this->_levelID + 1) + "/level.json", root);
        return true;
    }
    catch (std::exception const& err) {
        return false;
    }
}

void Game::createEnemies(void)
{
    try {
        std::vector<std::map<std::string, std::string>> datas;
        boost::property_tree::ptree root;

        boost::property_tree::read_json("data/level_" + std::to_string(this->_levelID) + "/enemies.json", root);
        for (boost::property_tree::ptree::value_type& parent : root.get_child("")) {
            std::map<std::string, std::string> enemy;

            for (boost::property_tree::ptree::value_type& data : root.get_child(parent.first)) {
                enemy[data.first] = data.second.data();
            }
            datas.push_back(enemy);
        }
        for (int i = 0; i < datas.size(); i++) {
            _breeds.push_back(std::make_unique<rtype::actor::Breed>(std::stoi(datas[i]["maxHealth"]),
                datas[i]["name"], std::stoi(datas[i]["occurences"]), datas[i]["sprite"], std::stoi(datas[i]["timeToSpawn"]), datas[i]["pattern"]));
            this->_breedNumber++;
        }
    }
    catch (std::exception const& err) {
        if (this->_room)
            this->_room->setErrors(true);
    }
}

void Game::checkGameStatus(void)
{
    if (this->_levelStatus != LevelState::INGAME)
        return;

    // Check if all players are dead

    bool allAreDead = true;

    for (int i = 0; i < this->_playerNumber; i++) {
        if (this->_players[i]->getHealth() > 0) {
            allAreDead = false;
            break;
        }
    }

    if (allAreDead && this->_levelStatus == LevelState::INGAME) {
        this->_levelStatus = LevelState::LOOSE;
        if (this->_room)
            this->_room->sendLevelStatus(this->_levelStatus);
        return;
    }

    // Check if all enemies are dead

    bool hasAllSpawed = true;

    for (int i = 0; i < this->_breeds.size(); i++) {
        if (this->_breeds[i]->getOccurences() > 0) {
            hasAllSpawed = false;
            break;
        }
    }

    if (this->_enemies.size() <= 0 && hasAllSpawed && this->_levelStatus == LevelState::INGAME) {
        if (this->_isBoss && !this->_boss) {
            this->_boss = std::make_unique<rtype::actor::BossDobkeratops>(nullptr);
            if (this->_room) {
                this->_room->sendBossSpawn();
                this->_room->sendBossHeartInfos(0, this->_boss->getHearts()[0]->_id);
                this->_room->sendBossHeartInfos(1, this->_boss->getHearts()[1]->_id);
                this->_room->sendBossHeartInfos(2, this->_boss->getHearts()[2]->_id);
            }
            return;
        }
        else if (this->_isBoss && !this->_boss->isDead()) {
            return;
        }

        for (int i = 0; i < this->_playerNumber; i++) {
            if (this->_boss)
                this->_players[i]->addScore(this->_boss->getScore());
            if (this->_room) {
                this->_room->sendPlayerScore(this->_players[i]->getScore(), i);
            }
        }

        if (this->checkNextGameLevel())
            this->_levelStatus = LevelState::WIN;
        else
            this->_levelStatus = LevelState::ENDING;

        std::cout << "LEVEL FINI" << this->_levelStatus << std::endl;

        if (this->_room) {
            this->_room->sendLevelStatus(this->_levelStatus);
        }
        return;
    }
}

void Game::update()
{
    if (!this->_resetVariable) {
        _timer.restart();
        this->_resetVariable = true;
    }

    if (this->_levelStatus == INGAME) {

        // Spawn of Asteroids

        //if (this->_asteroidsClock.getElapsedTime().asSeconds() > this->_asteroidsTime) {

        //    this->_asteroids.push_back(std::make_unique<rtype::actor::Asteroid>());

        //    if (!this->_asteroids.back())
        //        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

        //    this->_asteroidsTime = rtype::client::utilities::RandomNumbers::randomFloat(2, 5);
        //    this->_asteroids.back()->setId(UDPServer::generateID(10));
        //    this->_asteroidsClock.restart();
        //}

         // Update All Asteroids

        //for (int i = 0; i < this->_asteroids.size(); i++) {
        //    this->_asteroids[i]->update(nullptr);

        //    if (this->_asteroids[i]->getState() == rtype::actor::Asteroid::DESTROYED || this->_asteroids[i]->getPositions().x < -200)
        //        this->_asteroids.erase(this->_asteroids.begin() + i);
        //}

        // Check if asteroids collide a player

        //for (int playerNumber = 0; playerNumber < this->_playerNumber; playerNumber++) {
        //    std::vector<std::unique_ptr<rtype::actor::Missile>>& playerMissile = this->_players[playerNumber]->getMissiles();

        //    for (int asteroidsNumber = 0; asteroidsNumber < this->_asteroids.size(); asteroidsNumber++) {

        //        if (this->_players[playerNumber]->isAsteroidCollision(this->_asteroids[asteroidsNumber].get())) {
        //            this->_asteroids[asteroidsNumber]->destroy();
        //            if (this->_room) {
        //                this->_room->sendAsteroidsDestroy(this->_asteroids[asteroidsNumber]->getId());
        //                this->_room->sendPlayerAsteroidDamage(this->_asteroids[asteroidsNumber]->getId(), playerNumber);
        //            }
        //            this->_players[playerNumber]->takeDamage(1);
        //        }

        //        for (int missileNumber = 0; missileNumber < playerMissile.size(); missileNumber++) {
        //            if (this->_asteroids[asteroidsNumber]->isMissileCollision(playerMissile[missileNumber].get())) {
        //                this->_asteroids[asteroidsNumber]->destroy();
        //                this->_players[playerNumber]->addScore(10);
        //                if (this->_room) {
        //                    this->_room->sendAsteroidsDestroy(this->_asteroids[asteroidsNumber]->getId());
        //                    this->_room->sendPlayerScore(this->_players[playerNumber]->getScore(), playerNumber);
        //                }
        //                playerMissile.erase(playerMissile.begin() + missileNumber);
        //                break;
        //            }
        //        }
        //    }
        //}

        for (int breedNumber = 0; breedNumber < this->_breeds.size(); breedNumber++) {
            if (this->_breeds[breedNumber]->getTimeToSpawn() < _timer.getElapsedTime().asSeconds()) {
                if (this->_breeds[breedNumber]->getClockSpawn().getElapsedTime().asSeconds() > 0.5) {
                    if (this->_breeds[breedNumber]->getOccurences() > 0) {
                        _enemies.push_back(_breeds[breedNumber]->newMonster());
                        if (!this->_enemies.back())
                            continue;
                        rtype::actor::Monster *enemyPtr = dynamic_cast<rtype::actor::Monster *>(_enemies.back().get());
                        if (!enemyPtr)
                            throw rtype::settings::Errors("Can't dynamic cast enemy", "MemoryError:");
                        enemyPtr->setBreedIndex(breedNumber);
                        this->_enemies.back()->setEntityID(UDPServer::generateID(10));
                        sf::Vector2f spawnPoint = enemyPtr->getPattern()->getSpawnPoint();
                        _enemies.back()->setPosition(spawnPoint);
                    }
                    this->_breeds[breedNumber]->getClockSpawn().restart();
                }
            }
        }

        for (int playerNumber = 0; playerNumber < this->_playerNumber; playerNumber++) {
            std::vector<std::unique_ptr<rtype::actor::Missile>>& playerMissile = this->_players[playerNumber]->getMissiles();

            for (int playerMissileCounter = 0; playerMissileCounter < playerMissile.size(); playerMissileCounter++) {

                for (int i = 0; i < this->_enemies.size(); i++) {
                    if (this->_enemies[i]->isMissileCollision(playerMissile[playerMissileCounter].get())) {

                        playerMissile.erase(playerMissile.begin() + playerMissileCounter);
                        this->_enemies[i]->takeDamage(this->_players[playerNumber]->getDamage());
                        this->_players[playerNumber]->addScore(this->_enemies[i]->getScore());
                        if (this->_room) {
                            this->_room->sendPlayerScore(this->_players[playerNumber]->getScore(), playerNumber);
                            this->_room->sendEnemiesDamage(this->_enemies[i]->getEntityID(), playerNumber);
                        }
                        break;
                    }
                }
                if (this->_boss) {
                    if (this->_boss->isTailCollision(playerMissile[playerMissileCounter].get())) {
                        playerMissile.erase(playerMissile.begin() + playerMissileCounter);
                        break;
                    }
                    else if (this->_boss->isMissileCollision(playerMissile[playerMissileCounter].get(), this->_room, playerNumber)) {
                        playerMissile.erase(playerMissile.begin() + playerMissileCounter);
                        break;
                    }
                }
            }
        }

        // Check if enemies missiles collied an player
        for (int i = 0; i < this->_enemies.size(); i++) {
            if (!this->_enemies[i])
                continue;
            rtype::actor::Monster *monster = dynamic_cast<rtype::actor::Monster *>(this->_enemies[i].get());
            if (!monster)
                throw rtype::settings::Errors("Can't dynamic cast enemy", "MemoryError:");

            if (monster->canShoot()) {
                monster->shoot();
                if (this->_room)
                    this->_room->sendEnemiesShooting(this->_enemies[i]->getEntityID());
            }

            std::vector<std::unique_ptr<rtype::actor::Missile>>& ennemyMissile = this->_enemies[i]->getMissiles();

            for (int playerNumber = 0; playerNumber < this->_playerNumber; playerNumber++) {

                for (int ennemiesMissileCounter = 0; ennemiesMissileCounter < ennemyMissile.size(); ennemiesMissileCounter++) {
                    if (this->_players[playerNumber]->isMissileCollision(ennemyMissile[ennemiesMissileCounter].get())) {
                        ennemyMissile.erase(ennemyMissile.begin() + ennemiesMissileCounter);
                        this->_players[playerNumber]->takeDamage(this->_enemies[i]->getDamage());
                        if (this->_room)
                            this->_room->sendPlayerDamage(this->_enemies[i]->getEntityID(), playerNumber);
                        break;
                    }
                }
            }
        }

        // Update Enemies

        for (int i = 0; i < _enemies.size(); i++)
            this->_enemies[i]->update(nullptr);

        // Update Players

        for (int playerNumber = 0; playerNumber < _playerNumber; playerNumber++)
            this->_players[playerNumber]->update(nullptr);

        // Check if Player collide an Enemy

        for (int playerNumber = 0; playerNumber < this->_playerNumber; playerNumber++) {

            for (int i = 0; i < this->_enemies.size(); i++) {

                if (this->_players[playerNumber]->isActorCollision(this->_enemies[i].get())) {

                    this->_players[playerNumber]->takeDamage(this->_enemies[i]->getDamage());

                    if (this->_room)
                        this->_room->sendPlayerDamage(this->_enemies[i]->getEntityID(), playerNumber);
                }
            }

            // Check if player collide boss

            if (_boss) {
                if (_boss->isPlayerCollision(_players[playerNumber])) {
                    _players[playerNumber]->takeDamage(_boss->getDamage());
                    if (this->_room)
                        this->_room->sendPlayerBossDamage(playerNumber);
                }
            }

        }

        if (_boss) {
            if (!_boss->isSpawned()) {
                _boss->spawn(nullptr);
                _boss->update(this->_room);
            }
            else {
                _boss->update(this->_room);
            }
        }

        // Check if an enemies is dead
        for (unsigned short i = 0; i < this->_enemies.size(); i++) {
            if ((this->_enemies[i]->getHealth() <= 0 && this->_enemies[i]->getState() == rtype::actor::Actor::DEAD) || this->_enemies[i]->getPosition().x < -100) {
                this->_enemies.erase(this->_enemies.begin() + i);
            }
        }

        // Perform command on ennemies
        for (unsigned short i = 0; i < this->_enemies.size(); i++) {
            if (!this->_enemies[i])
                continue;
            rtype::actor::Monster *enemyPtr = dynamic_cast<rtype::actor::Monster *>(this->_enemies[i].get());
            if (!enemyPtr)
                throw rtype::settings::Errors("Can't dynamic cast enemy", "MemoryError:");
            enemyPtr->getPattern()->perform(_enemies[i]);
        }

    }

    this->checkGameStatus();
}

EnemyData Game::getEnemyDatas(int index)
{
    if (!this->_enemies[index])
        throw rtype::settings::Errors("Enemy is a nullptr", "MemoryError:");
    rtype::actor::Monster *enemyPtr = dynamic_cast<rtype::actor::Monster *>(this->_enemies[index].get());
    if (!enemyPtr)
        throw rtype::settings::Errors("Can't dynamic cast enemy", "MemoryError:");
    EnemyData dataE = {
        .breedIndex = enemyPtr->getBreedIndex(),
        .positions = {this->_enemies[index]->getPosition().x, this->_enemies[index]->getPosition().y},
    };

    strcpy(dataE.enemyID, this->_enemies[index]->getEntityID().c_str());
    if (this->_enemies[index]->getHealth() <= 0)
        dataE.isAlive = false;
    else
        dataE.isAlive = true;

    return dataE;
}

AsteroidsDatas Game::getAsteroidDatas(int index)
{
    AsteroidsDatas data;

    strcpy(data.asteroidID, this->_asteroids[index]->getId().c_str());
    data.positions = { this->_asteroids[index]->getPositions().x, this->_asteroids[index]->getPositions().y };
    data.speed = this->_asteroids[index]->getSpeed();
    data.size = this->_asteroids[index]->getSize();
    if (this->_asteroids[index]->getState() != rtype::actor::Asteroid::ALIVE)
        data.isDestroyed = true;
    else
        data.isDestroyed = false;

    return data;
}

std::vector<std::unique_ptr<rtype::actor::Actor>> &Game::getEnemies(void)
{
    return this->_enemies;
}

std::vector<std::unique_ptr<rtype::actor::Actor>>& Game::getPlayers(void)
{
    return this->_players;
}

std::vector<std::unique_ptr<rtype::actor::Breed>>& Game::getBreeds(void)
{
    return this->_breeds;
}

LevelState Game::getGameStatus(void)
{
    return this->_levelStatus;
}

std::vector<std::unique_ptr<rtype::actor::Asteroid>>& Game::getAsteroids(void)
{
    return this->_asteroids;
}

std::unique_ptr<rtype::actor::BossDobkeratops>& Game::getBoss(void)
{
    return this->_boss;
}

dobkeratopsMissileDatas Game::getBossHeartMissileDatas(std::string heartID, int index)
{
  dobkeratopsMissileDatas data;

  for (int i = 0; i < this->_boss->getHearts().size(); i++) {
      if (this->_boss->getHearts()[i]->_id == heartID) {
          std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& allMissiles = this->_boss->getHearts()[i]->getMissile();

          strcpy(data.missileID, allMissiles[index]->getId().c_str());
          data.positions = { allMissiles[index]->getPosition().x, allMissiles[index]->getPosition().y };
          data.isDestroyed = allMissiles[index]->isDestroyed();

          break;
      }
  }

  return data;
}

dobkeratopsMissileDatas Game::getBossMissileDatas(int index)
{
    dobkeratopsMissileDatas data;
    std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& allMissiles = this->_boss->getMissile();

    data.isDestroyed = allMissiles[index]->isDestroyed();
    strcpy(data.missileID, allMissiles[index]->getId().c_str());
    data.positions = { allMissiles[index]->getPosition().x, allMissiles[index]->getPosition().y };

    return data;
}

std::pair<float, float> Game::getBossTailsPosition(int index)
{
    std::vector<std::unique_ptr<rtype::client::utilities::AnimatedSprite>>& tail = this->_boss->getTails();

    return { tail[index]->getPosition().x, tail[index]->getPosition().y };
}
