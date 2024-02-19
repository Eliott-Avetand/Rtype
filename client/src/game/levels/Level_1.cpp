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
//  File:            Level_1.cpp                                                     //
//  Description:     File for the Level_1 Class                                      //
//                                                                                   //
// ********************************************************************************* //

#include "Level_1.hpp"
#include "MainMenu.hpp"
#include "PauseMenu.hpp"
#include "EndingScreen.hpp"
#include "TransitionScreen.hpp"
#include "GameOverScreen.hpp"

rtype::client::Level_1 *rtype::client::Level_1::_level1instance{nullptr};
std::mutex rtype::client::Level_1::_mutex;

void rtype::client::Level_1::init(rtype::engine::Engine* engine)
{
    std::cout << "Initializing the Level_1 class..." << std::endl;

    std::vector<int> positionFactor = { 50, 40, 30, 20 };

    _level1instance = this;
    this->_actualLevel = engine->gameDatas.getActualLevel();

    _boss = nullptr;
    this->_networkThread = nullptr;

    if (engine->getClient()->getIsOnline()) {
        this->_playerID = engine->getClient()->askPlayerId(20);
        this->_playerNumber = engine->getClient()->askPlayerCount(20);

        if (this->_playerID == -1 || this->_playerNumber <= 0 || this->_playerNumber > 4) {
            engine->getClient()->askLeave();
            engine->changeState(rtype::client::MainMenu::getInstance());
            return;
        }
    }
    else {
        this->_playerID = 0;
        this->_playerNumber = 1;
    }

    this->initBackground(engine);
    this->initMusic(engine);
    this->initBoss(engine);

    this->_breedNumber = 0;

    this->_threadActive = true;
    this->_threadCreated = false;
    this->_sessionIsClosed = false;

    this->_levelFinish = false;
    this->_levelStatus = LevelState::INGAME;

    this->_timer.restart();

    this->_asteroidsTime = rtype::client::utilities::RandomNumbers::randomFloat(2, 5);
    this->_asteroidsClock.restart();

    this->_isPingDisplayed = false;
    this->_pingText = std::make_unique<rtype::client::utilities::CustomText>("", FONT_PATH, 20, sf::Color::White, sf::Vector2f(0, 20));

    if (!this->_pingText) {
        engine->getClient()->askLeave();
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");
    }

    this->_pingText->setOrigin(0, this->_pingText->getGlobalBounds().height / 2);

    this->initInput();

    for (int i = 0; i < this->_playerNumber; i++) {
        sf::Vector2f pos = { 200, engine->options.getWindowHeight() * (positionFactor[this->_playerNumber - 1] * float(i + 1)) / 100 - engine->options.getWindowHeight() * 10 / 100 };

        this->_players.push_back(std::make_unique<rtype::actor::Player>("", i));

        if (!this->_players[i]) {
            if (engine->getClient()->getIsOnline()) {
                engine->getClient()->askLeave();
            }
            throw rtype::settings::Errors("Not enough memories", "MemoryError:");
        }

        this->_players[i]->setPosition(pos);

        rtype::actor::Player *playerPtr = dynamic_cast<rtype::actor::Player *>(this->_players[i].get());
        if (!playerPtr)
            throw rtype::settings::Errors("Can't dynamic cast player", "MemoryError:");

        if (i == this->_playerID)
            playerPtr->setPlayerColor(engine->gameDatas.getPlayerColor());
        else {
            std::array<int, 3> colors = engine->getClient()->askGetPlayerColor(i, 20);

            if (colors[0] == -1 || colors[1] == -1 || colors[2] == -1) {
                engine->getClient()->askLeave();
                engine->changeState(rtype::client::MainMenu::getInstance());
                return;
            }

            playerPtr->setPlayerColor(sf::Color(colors[0], colors[1], colors[2]));
        }

        std::string name = "";
        if (engine->getClient()->getIsOnline())
        playerPtr->setPlayerName(name);
    }

    this->loadEnemies(engine);

    this->_players[this->_playerID]->addScore(engine->gameDatas.getScore());

    if (engine->getClient()->getIsOnline()) {
        if (engine->getClient()->askPlayerIsPlaying(20) == -1) {
            engine->getClient()->askLeave();
            engine->changeState(rtype::client::MainMenu::getInstance());
            return;
        }

        engine->getClient()->getSocket().non_blocking(true);
    }

    std::cout << "Level_1 class initialized." << std::endl;
}

void rtype::client::Level_1::initInput()
{
    inputHandler.bindCommand(sf::Keyboard::Z, std::make_unique<rtype::action::MoveUpCommand>());
    inputHandler.bindCommand({sf::Joystick::Axis::Y, rtype::action::joystickAxis::UP}, std::make_unique<rtype::action::MoveUpCommand>());

    inputHandler.bindCommand(sf::Keyboard::S, std::make_unique<rtype::action::MoveDownCommand>());
    inputHandler.bindCommand({ sf::Joystick::Axis::Y, rtype::action::joystickAxis::DOWN }, std::make_unique<rtype::action::MoveDownCommand>());

    inputHandler.bindCommand(sf::Keyboard::D, std::make_unique<rtype::action::MoveRightCommand>());
    inputHandler.bindCommand({ sf::Joystick::Axis::X, rtype::action::joystickAxis::RIGHT }, std::make_unique<rtype::action::MoveRightCommand>());

    inputHandler.bindCommand(sf::Keyboard::Q, std::make_unique<rtype::action::MoveLeftCommand>());
    inputHandler.bindCommand({ sf::Joystick::Axis::X, rtype::action::joystickAxis::LEFT }, std::make_unique<rtype::action::MoveLeftCommand>());

    inputHandler.bindCommand(sf::Keyboard::Space, std::make_unique<rtype::action::ShootCommand>());
    inputHandler.bindCommand(rtype::action::joystickButton::A, std::make_unique<rtype::action::ShootCommand>());

    inputHandler.registerBindedKeys();
}

void rtype::client::Level_1::initBackground(rtype::engine::Engine *engine)
{
    std::string planetsName = "gas_planet";
    int planetStyle = 1;
    sf::Vector2f position = { 800, 800 };
    float size = 2;
    float speed = 1;
    bool canMove = true;

    try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json("data/level_" + std::to_string(this->_actualLevel) + "/level.json", root);
        
        std::string data = root.get_child("background").get_child("name").data();

        this->_background =  std::make_unique<rtype::client::utilities::BackgroundSpaceGenerator>(data);

        if (!this->_background)
            throw rtype::settings::Errors("Not enough memories", "MemoryError:");

        for (boost::property_tree::ptree::value_type& planetDatas : root.get_child("background").get_child("planets")) {

            try {
                planetsName = planetDatas.second.get_child("name").get_value<std::string>();
                planetStyle = planetDatas.second.get_child("style").get_value<int>();
                position.x = planetDatas.second.get_child("positions").get_child("x").get_value<float>();
                position.y = planetDatas.second.get_child("positions").get_child("y").get_value<float>();
                speed = planetDatas.second.get_child("speed").get_value<float>();
                size = planetDatas.second.get_child("size").get_value<float>();
                canMove = planetDatas.second.get_child("canMove").get_value<bool>();

                this->_background->addPlanet(planetFilePath.find(planetsName)->second, planetStyle, sf::Vector2f(position.x, position.y), size, speed, canMove);
            }
            catch (std::exception const& error) {
                this->_background->addPlanet(GAS_PLANET_TEXTURE, 1, sf::Vector2f(500, 500), 3, 1, true);
            }
        }

        this->_background->toggleAsteroidsSpawn(false);
        this->_background->manageAsteroidsSpawnTime(0.1, 1.5);
        this->_background->toggleBackgroundMove(true);
        this->_background->togglePlanetsSpawn(true);
    }
    catch (std::exception const &error) {
        std::cerr << error.what() << std::endl;
        engine->changeState(rtype::client::MainMenu::getInstance());
    }
}

void rtype::client::Level_1::initBoss(rtype::engine::Engine *engine)
{
    try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json("data/level_" + std::to_string(this->_actualLevel) + "/level.json", root);

        _isBoss = root.get_child("boss").data() == "true";
    }
    catch (std::exception const& error) {
        std::cerr << error.what() << std::endl;
        engine->changeState(rtype::client::MainMenu::getInstance());
    }
}

void rtype::client::Level_1::initMusic(rtype::engine::Engine* engine)
{
    try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json("data/level_" + std::to_string(this->_actualLevel) + "/level.json", root);

        engine->options.music.changeMusic(root.get_child("music").data());

        if (!engine->options.music.isMusicActive()) {

            engine->options.music.toggleMusic(false);

        }
    }
    catch (std::exception const& error) {
        std::cerr << error.what() << std::endl;
        engine->changeState(rtype::client::MainMenu::getInstance());
    }
}

bool rtype::client::Level_1::checkNextGameLevel(rtype::engine::Engine *engine)
{
    try {
        boost::property_tree::ptree root;

        boost::property_tree::read_json("data/level_" + std::to_string(engine->gameDatas.getActualLevel() + 1) + "/enemies.json", root);
        return true;
    }
    catch (std::exception const& err) {
        return false;
    }
}

void rtype::client::Level_1::loadEnemies(rtype::engine::Engine *engine)
{
    try {
        if (!engine->getClient()->getIsOnline()) {
            std::vector<std::map<std::string, std::string>> datas;
            boost::property_tree::ptree root;

            boost::property_tree::read_json("data/level_" + std::to_string(this->_actualLevel) + "/enemies.json", root);
            for (boost::property_tree::ptree::value_type &parent : root.get_child("")) {
                std::map<std::string, std::string> enemy;

                for (boost::property_tree::ptree::value_type &data : root.get_child(parent.first)) {
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
        else {
            int breedNumber = engine->getClient()->askBreedsNumbers(20);

            if (breedNumber == -1) {
                engine->getClient()->askLeave();
                engine->changeState(rtype::client::MainMenu::getInstance());
            }

            for (int i = 0; i < breedNumber; i++) {
                BreedData breed = engine->getClient()->askBreeds(i, 20);

                if (breed.occurences == -1) {
                    engine->getClient()->askLeave();
                    engine->changeState(rtype::client::MainMenu::getInstance());
                }

                _breeds.push_back(std::make_unique<rtype::actor::Breed>(breed.maxHealth,
                    breed.name, breed.occurences, breed.sprite, 1));
            }
        }
    }
    catch (std::exception const& error) {
        std::cerr << error.what() << std::endl;
        engine->changeState(rtype::client::MainMenu::getInstance());
    }
}

void rtype::client::Level_1::loadBoss()
{
}

void rtype::client::Level_1::cleanup()
{
    std::cout << "Cleaning the Level_1 class..." << std::endl;

    this->_powerUpItems.clear();
    //this->_players.clear();
    //this->_enemies.clear();

    if (engine::Engine::getInstance()->getClient()->getIsOnline()) {
        std::mutex* mutex = new std::mutex;

        if (!mutex)
            throw rtype::settings::Errors("Not enough memories", "MemoryError:");

        mutex->lock();

        this->_threadActive = false;

        mutex->unlock();

        if (mutex)
            delete mutex;

        if (this->_networkThread) {

            if (this->_networkThread->joinable())
                this->_networkThread->join();

            delete this->_networkThread;
        }
    }

    releaseInstance();
    std::cout << "Level_1 class cleaned up." << std::endl;
}

void rtype::client::Level_1::threadNetworkReceiver(rtype::engine::Engine* engine)
{
    if (this->_levelStatus == LevelState::ENDING)
        return;

    DataStruct data;

    boost::asio::ip::udp::endpoint senderEndpoint;
    boost::system::error_code error;

    std::mutex* mutex = new std::mutex;

    if (!mutex)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    bool find = false;
    int enemiesNumber = 0;
    
    size_t byteReceive = engine->getClient()->getSocket().receive_from(boost::asio::buffer(&data, sizeof(DataStruct)), senderEndpoint, 0, error);

    mutex->lock();

    if (byteReceive != sizeof(DataStruct)) {
        mutex->unlock();

        if (mutex)
            delete mutex;
        return;
    }

    if (data.currentPlayer.playerID > 4) {
        mutex->unlock();

        if (mutex)
            delete mutex;
        return;
    }

    switch (data.command) {
        case CommandCode::MOVE:
            std::cout << "MOVE" << std::endl;

            std::cout << "ID: " << data.currentPlayer.playerID << std::endl;
            std::cout << "POSITIONS X: " << data.currentPlayer.positions.first << " POSITIONS Y: " << data.currentPlayer.positions.second << std::endl;
            std::cout << "MOUVEMENT" << data.currentPlayer.moveDirection << std::endl;


            if (data.currentPlayer.playerID > 3 || data.currentPlayer.playerID < 0) {
                std::cout << "OUAIS C'EST PAS UN BON PLAYER ID: " << data.currentPlayer.playerID << std::endl;
                mutex->unlock();

                if (mutex)
                    delete mutex;
                return;
            }

            if (data.currentPlayer.playerID == this->_playerID)
                break;

            std::cout << "PLAYER IS UPDATE AVEC SA POS" << std::endl;

            this->_players[data.currentPlayer.playerID]->setPosition(sf::Vector2f(data.currentPlayer.positions.first, data.currentPlayer.positions.second));
            this->_players[data.currentPlayer.playerID]->setDirection(data.currentPlayer.moveDirection);
            std::cout << "________________________________________" << std::endl;
            break;
        case CommandCode::SHOOT:
            std::cout << "SHOOT" << std::endl;

            std::cout << "ID: " << data.currentPlayer.playerID << std::endl;
            std::cout << "POSITIONS X: " << data.currentPlayer.positions.first << " POSITIONS Y: " << data.currentPlayer.positions.second << std::endl;

            if (data.currentPlayer.playerID > 3 || data.currentPlayer.playerID < 0) {
                std::cout << "OUAIS C'EST PAS UN BON PLAYER ID: " << data.currentPlayer.playerID << std::endl;
                mutex->unlock();

                if (mutex)
                    delete mutex;
                return;
            }
            this->_players[data.currentPlayer.playerID]->getMissiles().push_back(std::make_unique<rtype::actor::Missile>(sf::Vector2f(data.currentPlayer.positions.first, data.currentPlayer.positions.second)));
            std::cout << "LE PLAYER AVEC ID: " << data.currentPlayer.playerID << "A SHOOT" << std::endl;

            if (!this->_players[data.currentPlayer.playerID]->getMissiles().back()) {
                std::cout << "BON BAH LE MISSILE A PAS ETE PUSH" << std::endl;
                mutex->unlock();

                if (mutex)
                    delete mutex;
                throw rtype::settings::Errors("Not enough memories", "MemoryError:");
            }
            std::cout << "________________________________________" << std::endl;
            break;
        case CommandCode::CLOSING:
            this->_sessionIsClosed = true;
            break;
        case CommandCode::PLAYERDAMAGE:
            std::cout << "PLAYERDAMAGE DESTROY" << std::endl;
            if (data.currentPlayer.playerID > 3 || data.currentPlayer.playerID < 0) {
                mutex->unlock();

                if (mutex)
                    delete mutex;
                return;
            }

            for (int actualEnemyNumber = 0; actualEnemyNumber < this->_enemies.size(); actualEnemyNumber++) {

                if (std::string(data.currentRoom.enemyInfos.enemyID) == this->_enemies[actualEnemyNumber]->getEntityID()) {
                    this->_players[data.currentPlayer.playerID]->takeDamage(this->_enemies[actualEnemyNumber]->getDamage());
                    break;
                }
            }
            break;
        case CommandCode::PLAYERSCORE:
            std::cout << "PLAYERSCORE DESTROY" << std::endl;
            if (data.currentPlayer.playerID > 3 || data.currentPlayer.playerID < 0) {
                mutex->unlock();
                

                if (mutex)
                    delete mutex;
                return;
            }

            this->_players[data.currentPlayer.playerID]->setScore(data.currentPlayer.score);
            break;
        case CommandCode::ENEMYDAMAGE:
            std::cout << "ENEMYDAMAGE DESTROY" << std::endl;
            if (data.currentPlayer.playerID > 3 || data.currentPlayer.playerID < 0) {
                mutex->unlock();

                if (mutex)
                    delete mutex;
                return;
            }

            for (int actualEnemyNumber = 0; actualEnemyNumber < this->_enemies.size(); actualEnemyNumber++) {


                if (std::string(data.currentRoom.enemyInfos.enemyID) == this->_enemies[actualEnemyNumber]->getEntityID()) {
                    this->_enemies[actualEnemyNumber]->takeDamage(this->_players[data.currentPlayer.playerID]->getDamage());
                    break;
                }
            }
            break;
        case CommandCode::ENEMYSHOOT:
            std::cout << "ENEMYSHOOT DESTROY" << std::endl;
            for (int actualEnemyNumber = 0; actualEnemyNumber < this->_enemies.size(); actualEnemyNumber++) {
                if (std::string(data.currentRoom.enemyInfos.enemyID) == this->_enemies[actualEnemyNumber]->getEntityID()) {
                    this->_enemies[actualEnemyNumber]->shoot();
                    break;
                }
            }
            break;
        case CommandCode::ENEMY:
            std::cout << "ENEMY DESTROY" << std::endl;
            find = false;

            for (int actualEnemyNumber = 0; actualEnemyNumber < this->_enemies.size(); actualEnemyNumber++) {

                if (std::string(data.currentRoom.enemyInfos.enemyID) == this->_enemies[actualEnemyNumber]->getEntityID()) {
                    find = true;
                    this->_enemies[actualEnemyNumber]->setPosition({ data.currentRoom.enemyInfos.positions.first, data.currentRoom.enemyInfos.positions.second });
                    if (data.currentRoom.enemyInfos.isAlive == false) {
                        this->_enemies[actualEnemyNumber]->takeDamage(999);
                    }
                    break;
                }
            }

            if (find == false) {
                if (this->_breeds[data.currentRoom.enemyInfos.breedIndex]->getOccurences() > 0)
                    this->_enemies.push_back(this->_breeds[data.currentRoom.enemyInfos.breedIndex]->newMonster());
                if (!this->_enemies.back()) {
                    mutex->unlock();

                    if (mutex)
                        delete mutex;
                    throw rtype::settings::Errors("Not enough memories", "MemoryError:");
                }

                this->_enemies.back()->setEntityID(std::string(data.currentRoom.enemyInfos.enemyID));
                this->_enemies.back()->setPosition({ data.currentRoom.enemyInfos.positions.first, data.currentRoom.enemyInfos.positions.second });
            }
            break;
        case CommandCode::ASTEROID:
            find = false;

            for (int actualAsteroidNumber = 0; actualAsteroidNumber < this->_asteroids.size(); actualAsteroidNumber++) {

                if (std::string(data.currentRoom.asteroidInfos.asteroidID) == this->_asteroids[actualAsteroidNumber]->getId()) {
                    find = true;
                    this->_asteroids[actualAsteroidNumber]->setPosition({ data.currentRoom.asteroidInfos.positions.first, data.currentRoom.asteroidInfos.positions.second });
                    if (data.currentRoom.asteroidInfos.isDestroyed == true)
                        this->_asteroids[actualAsteroidNumber]->setState(rtype::actor::Asteroid::EXPLOSED);
                    break;
                }
            }

            if (find == false) {
                this->_asteroids.push_back(std::make_unique<rtype::actor::Asteroid>(sf::Vector2f(data.currentRoom.asteroidInfos.positions.first, data.currentRoom.asteroidInfos.positions.second), data.currentRoom.asteroidInfos.size, data.currentRoom.asteroidInfos.speed));

                if (!this->_asteroids.back()) {
                    mutex->unlock();

                    if (mutex)
                        delete mutex;
                    throw rtype::settings::Errors("Not enough memories", "MemoryError:");
                }

                this->_asteroids.back()->setId(std::string(data.currentRoom.asteroidInfos.asteroidID));
            }
            break;
        case CommandCode::ASTEROIDDESTROY:
            std::cout << "ASTEROIDS DESTROY" << std::endl;
            for (int actualAsteroidNumber = 0; actualAsteroidNumber < this->_asteroids.size(); actualAsteroidNumber++) {

                if (std::string(data.currentRoom.asteroidInfos.asteroidID) == this->_asteroids[actualAsteroidNumber]->getId()) {
                    this->_asteroids[actualAsteroidNumber]->destroy();
                    break;
                }
            }
            break;
        case CommandCode::PLAYERASTEROIDDAMAGE:
            std::cout << "ASTEROIDS PLAYER DAMAGE" << std::endl;
            if (data.currentPlayer.playerID > 3 || data.currentPlayer.playerID < 0) {
                mutex->unlock();

                if (mutex)
                    delete mutex;
                return;
            }

            for (int actualAsteroidNumber = 0; actualAsteroidNumber < this->_asteroids.size(); actualAsteroidNumber++) {

                if (std::string(data.currentRoom.asteroidInfos.asteroidID) == this->_asteroids[actualAsteroidNumber]->getId()) {
                    this->_players[data.currentPlayer.playerID]->takeDamage(1);
                    break;
                }
            }
            break;
        case CommandCode::BOSSSPAWN:
            std::cout << "BOSSSPAWN COMMAND" << std::endl;
            if (!this->_boss) {
                this->_boss = std::make_unique<rtype::actor::BossDobkeratops>(engine);

                if (!this->_boss) {
                    mutex->unlock();

                    if (mutex)
                        delete mutex;
                    throw rtype::settings::Errors("Not enough memories", "MemoryError:");
                }
            }
            break;
        case CommandCode::BOSSHEARTINFO:
            std::cout << "BOSSHEARTINFO COMMAND" << std::endl;

            if (this->_boss) {
                this->_boss->getHearts()[data.currentRoom.bossInfos.heartIndex]->_id = std::string(data.currentRoom.bossInfos.heartId);
            }
            break;
        case CommandCode::BOSSDAMAGE:
            std::cout << "BOSSDAMAGE COMMAND" << std::endl;
            if (this->_boss) {

                for (int i = 0; i < this->_boss->getHearts().size(); i++) {
                    if (std::string(data.currentRoom.bossInfos.heartId) == this->_boss->getHearts()[i]->_id) {
                        if (!this->_boss->getHearts()[i]->isDying() && !this->_boss->getHearts()[i]->isDead())
                            this->_boss->getHearts()[i]->takeDamage(engine, this->_boss->nbrHeartsAlive() == 0, 1);
                    }
                }
            }
            break;
        case CommandCode::BOSSMISSILE:
            std::cout << "BOSSMISSILE COMMAND" << std::endl;
            if (this->_boss) {

                find = false;

                std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& allMissiles = this->_boss->getMissile();

                for (int i = 0; i < allMissiles.size(); i++) {

                    if (allMissiles[i]->getId() == data.currentRoom.bossInfos.missileDatas.missileID) {
                        find = true;

                        allMissiles[i]->setPosition(sf::Vector2f(data.currentRoom.bossInfos.missileDatas.positions.first, data.currentRoom.bossInfos.missileDatas.positions.second));
                        break;
                    }

                }

                if (!find) {

                    this->_boss->shoot();
                    this->_boss->getMissile().back()->setId(std::string(data.currentRoom.bossInfos.missileDatas.missileID));
                    this->_boss->getMissile().back()->setPosition(sf::Vector2f(data.currentRoom.bossInfos.missileDatas.positions.first, data.currentRoom.bossInfos.missileDatas.positions.second));
                }
            }
            break;
        case CommandCode::BOSSHEARTDESTROY:
            std::cout << "BOSSHEARTDESTROY COMMAND" << std::endl;
            if (this->_boss) {
                for (int i = 0; i < this->_boss->getHearts().size(); i++) {
                    if (std::string(data.currentRoom.bossInfos.heartId) == this->_boss->getHearts()[i]->_id)
                        this->_boss->getHearts()[i]->takeDamage(engine, this->_boss->nbrHeartsAlive() == 0, 999);
                }
            }
            break;
        case CommandCode::BOSSHEARTMISSILE:
            std::cout << "BOSSHEARTMISSILE COMMAND" << std::endl;
            if (this->_boss) {

                find = false;

                int heartIndex = 0;

                for (int i = 0; i < this->_boss->getHearts().size(); i++) {
                    if (std::string(data.currentRoom.bossInfos.heartId) == this->_boss->getHearts()[i]->_id) {
                        heartIndex = i;
                        break;
                    }
                }

                std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& allMissiles = this->_boss->getHearts()[heartIndex]->getMissile();

                for (int i = 0; i < allMissiles.size(); i++) {

                    if (allMissiles[i]->getId() == data.currentRoom.bossInfos.missileDatas.missileID) {
                        find = true;

                        allMissiles[i]->setPosition(sf::Vector2f(data.currentRoom.bossInfos.missileDatas.positions.first, data.currentRoom.bossInfos.missileDatas.positions.second));
                        break;
                    }

                }

                if (!find) {

                    this->_boss->getHearts()[heartIndex]->shoot();
                    this->_boss->getHearts()[heartIndex]->getMissile().back()->setId(std::string(data.currentRoom.bossInfos.missileDatas.missileID));
                    this->_boss->getHearts()[heartIndex]->getMissile().back()->setPosition(sf::Vector2f(data.currentRoom.bossInfos.missileDatas.positions.first, data.currentRoom.bossInfos.missileDatas.positions.second));
                }

            }
            break;
        case CommandCode::BOSSHEARTMISSILEDESTROYED:
            std::cout << "BOSSHEARTMISSILEDESTROYED COMMAND" << std::endl;
            if (this->_boss) {
                int heartIndex = 0;

                for (int i = 0; i < this->_boss->getHearts().size(); i++) {
                    if (std::string(data.currentRoom.bossInfos.heartId) == this->_boss->getHearts()[i]->_id) {
                        heartIndex = i;
                        break;
                    }
                }

                std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& allMissiles = this->_boss->getHearts()[heartIndex]->getMissile();

                for (int i = 0; i < allMissiles.size(); i++) {

                    if (allMissiles[i]->getId() == data.currentRoom.bossInfos.missileDatas.missileID) {
                        allMissiles[i]->destroy();
                        break;
                    }

                }
            }
            break;
        case CommandCode::BOSSMISSILEDESTROYED:
            std::cout << "BOSSMISSILEDESTROYED COMMAND" << std::endl;
            if (this->_boss) {

                std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& allMissiles = this->_boss->getMissile();

                for (int i = 0; i < allMissiles.size(); i++) {

                    if (allMissiles[i]->getId() == data.currentRoom.bossInfos.missileDatas.missileID) {
                        allMissiles[i]->destroy();
                        break;
                    }

                }
            }
            break;
        case CommandCode::PLAYERBOSSDAMAGE:
            std::cout << "PLAYERBOSSDAMAGE COMMAND" << std::endl;
            if (data.currentPlayer.playerID > 3 || data.currentPlayer.playerID < 0) {
                mutex->unlock();

                if (mutex)
                    delete mutex;
                return;
            }

            this->_players[data.currentPlayer.playerID]->takeDamage(1);
            break;
        case CommandCode::BOSSTAILS:
            std::cout << "BOSS TAILS COMMAND" << std::endl;
            if (this->_boss) {
                for (int i = 0; i < this->_boss->getTails().size(); i++) {
                    if (i == data.currentRoom.bossInfos.tailIndex) {
                        this->_boss->getTails()[i]->setPosition(sf::Vector2f(data.currentRoom.bossInfos.tailPositions.first, data.currentRoom.bossInfos.tailPositions.second));
                        break;
                    }
                }
            }
            break;
        case CommandCode::PING:
            engine->getClient()->awnserPong();
            break;
        case CommandCode::LATENCY:
            engine->getClient()->setLatency(data.currentPlayer.latency);
            this->_pingText->setText(std::string("ms " + std::to_string(engine->getClient()->getLatency())));
            this->_pingText->setOrigin(0, this->_pingText->getGlobalBounds().height / 2);
            break;
        case CommandCode::LEVELSTATUS:
            this->_levelStatus = data.currentRoom.levelState;
            break;
        default:
            std::cout << "Received unknown command" << std::endl;
            break;
    }
    mutex->unlock();
    if (mutex)
        delete mutex;
}

void rtype::client::Level_1::threadNetworkSender(rtype::engine::Engine* engine)
{
    std::mutex* mutex = new std::mutex;

    if (!mutex)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");

    mutex->lock();

    static sf::Vector2f lastPosition = this->_players[_playerID]->getPosition();
    sf::Vector2f playerPosition = this->_players[_playerID]->getPosition();

    if (lastPosition != playerPosition) {
        engine->getClient()->askMove(playerPosition.x, playerPosition.y, this->_players[_playerID]->getDirection());
        lastPosition = playerPosition;
    }

    mutex->unlock();

    if (mutex)
        delete mutex;
}

void rtype::client::Level_1::networkHandler(rtype::engine::Engine* engine)
{
    try {
        while (this->_threadActive) {
            this->threadNetworkReceiver(engine);
            this->threadNetworkSender(engine);
        }
    }
    catch (std::exception const& err) {
        return;
    }
}

void rtype::client::Level_1::networkReceiver(rtype::engine::Engine *engine)
{
    if (!_threadCreated)  {

        this->_networkThread = new std::thread(&rtype::client::Level_1::networkHandler, this, engine);

        if (!this->_networkThread)
            throw rtype::settings::Errors("Not enough memories", "MemoryError:");
        this->_threadCreated = true;
    }
}

void rtype::client::Level_1::networkSender(rtype::engine::Engine *engine)
{

}

void rtype::client::Level_1::processInput(sf::Event event, rtype::engine::Engine *engine)
{
    if (!sf::Joystick::isConnected(0)) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            engine->pushState(rtype::client::PauseMenu::getInstance());
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
            if (!this->_isPingDisplayed)
                this->_isPingDisplayed = true;
        }
        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Tab) {
            if (this->_isPingDisplayed)
                this->_isPingDisplayed = false;
        }
    }
    else {
        if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == rtype::action::joystickButton::START)
            engine->pushState(rtype::client::PauseMenu::getInstance());
        if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == rtype::action::joystickButton::SELECT) {
            if (!this->_isPingDisplayed)
                this->_isPingDisplayed = true;
        }
        if (event.type == sf::Event::JoystickButtonReleased && event.joystickButton.button == rtype::action::joystickButton::SELECT) {
            if (this->_isPingDisplayed)
                this->_isPingDisplayed = false;
        }
    }
}

void rtype::client::Level_1::processPlayerInput(rtype::engine::Engine *engine)
{
    if (!sf::Joystick::isConnected(0)) {
        for (sf::Keyboard::Key key : inputHandler.getBindedKeys()) {
            if (sf::Keyboard::isKeyPressed(key)) {
                inputHandler.handleInput(key)->perform(this->_players[this->_playerID]);
            }
        }
    }
    else {
        for (int joystickButton : inputHandler.getJoystickBindedButtons()) {
            if (sf::Joystick::isButtonPressed(0, joystickButton)) {
                inputHandler.handleInput(joystickButton)->perform(this->_players[this->_playerID]);
            }
        }

        for (std::pair<sf::Joystick::Axis, int> joystickAxis : inputHandler.getJoystickBindedAxis()) {
            if (joystickAxis.second < 0 && sf::Joystick::getAxisPosition(0, joystickAxis.first) <= joystickAxis.second / 2)
                inputHandler.handleInput(joystickAxis)->perform(this->_players[this->_playerID]);
            if (joystickAxis.second > 0 && sf::Joystick::getAxisPosition(0, joystickAxis.first) >= joystickAxis.second / 2) {
                inputHandler.handleInput(joystickAxis)->perform(this->_players[this->_playerID]);
            }
        }
    }
}

void rtype::client::Level_1::pause(void)
{
}

void rtype::client::Level_1::resume(void)
{
}

void rtype::client::Level_1::checkOnlineGameStatus(rtype::engine::Engine* engine)
{
    if (this->_levelFinish)
        return;

    if (this->_levelStatus == LevelState::WIN) {
        this->_threadActive = false;
        this->_levelFinish = true;
        engine->gameDatas.setScore(this->_players[this->_playerID]->getScore());
        engine->pushState(rtype::client::TransitionScreen::getInstance());
    }
    if (this->_levelStatus == LevelState::LOOSE) {
        this->_levelFinish = true;
        engine->gameDatas.setScore(this->_players[this->_playerID]->getScore());
        engine->pushState(rtype::client::GameOverScreen::getInstance());
    }
    if (this->_levelStatus == LevelState::ENDING) {
        this->_levelFinish = true;
        engine->gameDatas.setScore(this->_players[this->_playerID]->getScore());
        engine->getClient()->askLeave();
        engine->pushState(rtype::client::EndingScreen::getInstance());
        rtype::client::EndingScreen::getInstance()->initPlayerPosition(this->_players[this->_playerID]->getPosition());
    }
}

void rtype::client::Level_1::checkOfflineGameStatus(rtype::engine::Engine* engine)
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
        engine->gameDatas.setScore(this->_players[this->_playerID]->getScore());
        engine->pushState(rtype::client::GameOverScreen::getInstance());
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
        if (_isBoss && !_boss) {
            _boss = std::make_unique<rtype::actor::BossDobkeratops>(engine);
            return;
        } else if (_isBoss && !_boss->isDead())
            return;
        if (_boss)
            this->_players[this->_playerID]->addScore(this->_boss->getScore());
        engine->gameDatas.setScore(this->_players[this->_playerID]->getScore());
        if (this->checkNextGameLevel(engine)) {
            this->_levelStatus = LevelState::WIN;
            engine->pushState(rtype::client::TransitionScreen::getInstance());
        }
        else {
            this->_levelStatus = LevelState::ENDING;
            engine->pushState(rtype::client::EndingScreen::getInstance());
            rtype::client::EndingScreen::getInstance()->initPlayerPosition(this->_players[this->_playerID]->getPosition());
        }
        return;
    }
}

void rtype::client::Level_1::updateOnline(rtype::engine::Engine* engine)
{
    // Lock mutex
    std::mutex* mutex = new std::mutex;
    if (!mutex)
        throw rtype::settings::Errors("Not enough memories", "MemoryError:");
    mutex->lock();

    // Go to main menu if session is closed by the server
    if (this->_sessionIsClosed) {
        mutex->unlock();
        if (mutex)
            delete mutex;
        engine->changeState(rtype::client::MainMenu::getInstance());
        return;
    }

    // Update asteroids
    for (int i = 0; i < this->_asteroids.size(); i++) {
        this->_asteroids[i]->update(engine);
        if (this->_asteroids[i]->getState() == rtype::actor::Asteroid::DESTROYED || this->_asteroids[i]->getPositions().x < -300)
            this->_asteroids.erase(this->_asteroids.begin() + i);
    }

    // Check if missiles colides Actors
    for (int playerNumber = 0; playerNumber < this->_playerNumber; playerNumber++) {
        std::vector<std::unique_ptr<rtype::actor::Missile>>& playerMissile = this->_players[playerNumber]->getMissiles();

        if (playerMissile.size() == 0)
            break;
        for (int playerMissileCounter = 0; playerMissileCounter < playerMissile.size(); playerMissileCounter++) {
            if (!playerMissile[playerMissileCounter])
                break;

            for (int i = 0; i < this->_enemies.size(); i++) {
                if (this->_enemies[i]->isMissileCollision(playerMissile[playerMissileCounter].get())) {
                    playerMissile.erase(playerMissile.begin() + playerMissileCounter);
                    break;
                }
            }

            for (int asteroidsNumber = 0; asteroidsNumber < this->_asteroids.size(); asteroidsNumber++) {

                if (this->_asteroids[asteroidsNumber]->isMissileCollision(playerMissile[playerMissileCounter].get())) {
                    playerMissile.erase(playerMissile.begin() + playerMissileCounter);
                    break;
                }
            }

            if (_boss) {
                if (this->_boss->isTailCollision(playerMissile[playerMissileCounter].get())) {
                    playerMissile.erase(playerMissile.begin() + playerMissileCounter);
                    break;
                }
                else if (_boss->isMissileCollision(playerMissile[playerMissileCounter].get(), engine)) {
                    playerMissile.erase(playerMissile.begin() + playerMissileCounter);
                    break;
                }
            }
        }
    }

    // Handle boss spawn and update
    if (this->_boss) {
        if (!this->_boss->isSpawned()) {
            this->_boss->spawn(engine);
            this->_boss->update(engine);
        }
        else {
            this->_boss->update(engine);
            this->_background->setBackgroundSpeed(2);
        }
    }

    // Update game UI
    this->_gameUI.update(engine, this->_players[this->_playerID].get());

    // Update players
    for (int playerNumber = 0; playerNumber < _playerNumber; playerNumber++)
        this->_players[playerNumber]->update(engine);

    // Update enemies
    for (int i = 0; i < _enemies.size(); i++)
        this->_enemies[i]->update(engine);

    // Handle player movement
    for (auto key : this->_keysPressed) {
        inputHandler.handleInput(key)->perform(this->_players[this->_playerID]);
        this->_keysPressed.clear();
    }

    // Handle end of the game
    this->checkOnlineGameStatus(engine);

    // Unlock mutex
    mutex->unlock();
    if (mutex)
        delete mutex;
}

void rtype::client::Level_1::update(rtype::engine::Engine *engine)
{
    _background->update(engine);

    // All Actions OFFLINE
    if (!engine->getClient()->getIsOnline()) {
        if (this->_levelStatus == INGAME) {
            // Handle player movement
            for (auto key : this->_keysPressed) {
                inputHandler.handleInput(key)->perform(this->_players[this->_playerID]);
                this->_keysPressed.clear();
            }

            // Update power up
            for (uint8_t powerUpIndex = 0; powerUpIndex < this->_powerUpItems.size(); powerUpIndex++) {
                if (!this->_powerUpItems[powerUpIndex]->hasSpawned() || !this->_powerUpItems[powerUpIndex]->isVisible())
                    continue;
                this->_powerUpItems[powerUpIndex]->update(engine);
                if (this->_players[this->_playerID]->isPowerUpCollision(this->_powerUpItems[powerUpIndex])) {
                    this->_powerUpItems[powerUpIndex]->pickUp();
                    rtype::actor::Player *playerPtr = dynamic_cast<rtype::actor::Player *>(this->_players[this->_playerID].get());
                    if (!playerPtr)
                        throw rtype::settings::Errors("Can't dynamic cast player", "MemoryError:");
                    playerPtr->setPowerUpAction(this->_powerUpItems[powerUpIndex]->getPowerUpName());
                }
            }

            // Spawn of monster
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
                            sf::Vector2f spawnPoint = enemyPtr->getPattern()->getSpawnPoint();
                            enemyPtr->setPosition(spawnPoint);
                        }
                        this->_breeds[breedNumber]->getClockSpawn().restart();
                    }
                }
            }

            // Spawn of Asteroids
            if (this->_asteroidsClock.getElapsedTime().asSeconds() > this->_asteroidsTime) {

                this->_asteroids.push_back(std::make_unique<rtype::actor::Asteroid>());

                if (!this->_asteroids.back())
                    throw rtype::settings::Errors("Not enough memories", "MemoryError:");

                this->_asteroidsTime = rtype::client::utilities::RandomNumbers::randomFloat(2, 5);
                this->_asteroidsClock.restart();
            }

            // Update All Asteroids
            for (unsigned short i = 0; i < this->_asteroids.size(); i++) {
                this->_asteroids[i]->update(engine);
                if (this->_asteroids[i]->getState() == rtype::actor::Asteroid::DESTROYED || this->_asteroids[i]->getPositions().x < -200)
                    this->_asteroids.erase(this->_asteroids.begin() + i);
            }

            // Check if asteroids collide a player
            for (unsigned short playerNumber = 0; playerNumber < this->_playerNumber; playerNumber++) {
                std::vector<std::unique_ptr<rtype::actor::Missile>>& playerMissile = this->_players[playerNumber]->getMissiles();

                for (unsigned short asteroidsNumber = 0; asteroidsNumber < this->_asteroids.size(); asteroidsNumber++) {
                    if (this->_players[playerNumber]->isAsteroidCollision(this->_asteroids[asteroidsNumber].get())) {
                        this->_asteroids[asteroidsNumber]->destroy();
                        this->_players[playerNumber]->takeDamage(1);
                    }

                    for (int missileNumber = 0; missileNumber < playerMissile.size(); missileNumber++) {
                        if (this->_asteroids[asteroidsNumber]->isMissileCollision(playerMissile[missileNumber].get())) {
                            this->_asteroids[asteroidsNumber]->destroy();
                            this->_players[playerNumber]->addScore(10);
                            playerMissile.erase(playerMissile.begin() + missileNumber);
                            break;
                        }
                    }
                }
            }

            // Update enemies
            for (unsigned short i = 0; i < this->_enemies.size(); i++) {
                if (!this->_enemies[i])
                    continue;
                rtype::actor::Monster *enemyPtr = dynamic_cast<rtype::actor::Monster *>(_enemies[i].get());
                if (!enemyPtr)
                    throw rtype::settings::Errors("Can't dynamic cast enemy", "MemoryError:");
                enemyPtr->getPattern()->perform(_enemies[i]);
            }

            // Check if player shoot collide ennemies
            std::vector<std::unique_ptr<rtype::actor::Missile>>& playerMissile = this->_players[this->_playerID]->getMissiles();
            for (int playerMissileCounter = 0; playerMissileCounter < playerMissile.size(); playerMissileCounter++) {
                for (int i = 0; i < this->_enemies.size(); i++) {
                    if (this->_enemies[i]->isMissileCollision(playerMissile[playerMissileCounter].get())) {
                        playerMissile.erase(playerMissile.begin() + playerMissileCounter);

                        // Spawn power up
                        const float randomChance = rtype::client::utilities::RandomNumbers::randomFloat(0.f, 100.f);
                        if (randomChance < rtype::powerup::TripleShootItem::getSpawnChance()) {
                            this->_powerUpItems.push_back(std::make_unique<rtype::powerup::TripleShootItem>(this->_enemies[i]->getPosition()));
                            if (!this->_powerUpItems[this->_powerUpItems.size() - 1])
                                throw rtype::settings::Errors("Not enough memories", "MemoryError:");
                        }

                        this->_enemies[i]->takeDamage(this->_players[this->_playerID]->getDamage());
                        this->_players[this->_playerID]->addScore(this->_enemies[i]->getScore());
                        break;
                    }
                }
                if (_boss) {
                    if (this->_boss->isTailCollision(playerMissile[playerMissileCounter].get())) {
                        playerMissile.erase(playerMissile.begin() + playerMissileCounter);
                        break;
                    }
                    else if (_boss->isMissileCollision(playerMissile[playerMissileCounter].get(), engine)) {
                        playerMissile.erase(playerMissile.begin() + playerMissileCounter);
                        break;
                    }
                }
            }

            // Check if player collide ennemies
            for (int i = 0; i < this->_enemies.size(); i++) {
                if (this->_players[this->_playerID]->isActorCollision(this->_enemies[i].get())) {
                    this->_players[this->_playerID]->takeDamage(this->_enemies[i]->getDamage());
                }
            }

            // Check if player collide boss
            if (_boss) {
                if (_boss->isPlayerCollision(_players[_playerID]))
                    _players[_playerID]->takeDamage(_boss->getDamage());
            }


            // Check if enemies shoot is collide player
            for (int i = 0; i < this->_enemies.size(); i++) {
                std::vector<std::unique_ptr<rtype::actor::Missile>>& ennemyMissile = this->_enemies[i]->getMissiles();

                for (int ennemiesMissileCounter = 0; ennemiesMissileCounter < ennemyMissile.size(); ennemiesMissileCounter++) {
                    if (this->_players[this->_playerID]->isMissileCollision(ennemyMissile[ennemiesMissileCounter].get())) {
                        ennemyMissile.erase(ennemyMissile.begin() + ennemiesMissileCounter);
                        this->_players[this->_playerID]->takeDamage(this->_enemies[i]->getDamage());
                        break;
                    }
                }
            }

            for (int i = 0; i < this->_enemies.size(); i++) {
                if ((this->_enemies[i]->getHealth() <= 0 && this->_enemies[i]->getState() == rtype::actor::Actor::DEAD) || this->_enemies[i]->getPosition().x < -100) {
                    this->_enemies.erase(this->_enemies.begin() + i);
                }
            }

            for (int playerNumber = 0; playerNumber < _playerNumber; playerNumber++)
                this->_players[playerNumber]->update(engine);

            for (int i = 0; i < _enemies.size(); i++)
                this->_enemies[i]->update(engine);

            this->checkOfflineGameStatus(engine);

            if (_boss) {
                if (!_boss->isSpawned()) {
                    _boss->spawn(engine);
                    _boss->update(engine);
                }
                else {
                    _boss->update(engine);
                    this->_background->setBackgroundSpeed(2);
                }
            }
            this->_gameUI.update(engine, this->_players[this->_playerID].get());
        }

    }
    else
        this->updateOnline(engine);
}

void rtype::client::Level_1::renderLevel(rtype::engine::Engine* engine)
{
    this->_background->display(engine);


    if (this->_levelStatus != LevelState::ENDING) {
        for (uint8_t i = 0; i < this->_powerUpItems.size(); i++)
            this->_powerUpItems[i]->render(engine);
        for (int i = 0; i < this->_playerNumber; i++) {
            if (this->_players[i]->getState() != rtype::actor::Actor::actorState::DEAD) {
                this->_players[i]->draw(engine);
                if (this->_isPingDisplayed) {
                    rtype::actor::Player *playerPtr = dynamic_cast<rtype::actor::Player *>(this->_players[i].get());
                    if (!playerPtr)
                        throw rtype::settings::Errors("Can't dynamic cast player", "MemoryError:");
                    playerPtr->drawUsername(engine);
                }
            }
        }

        for (int i = 0; i < _enemies.size(); i++) {
            if (this->_enemies[i]->getState() != rtype::actor::Actor::actorState::DEAD)
                this->_enemies[i]->draw(engine);
        }

        if (_boss)
            _boss->draw(engine);


        for (int asteroidsNumber = 0; asteroidsNumber < this->_asteroids.size(); asteroidsNumber++) {
            if (this->_asteroids[asteroidsNumber]->getState() != rtype::actor::Asteroid::DESTROYED)
                this->_asteroids[asteroidsNumber]->draw(engine);
        }


        if (_isPingDisplayed) {
            this->_pingText->draw(engine);
        }
    }
    this->_gameUI.draw(engine);

}

void rtype::client::Level_1::render(rtype::engine::Engine *engine)
{
    engine->window.clear();
    this->renderLevel(engine);
    engine->window.display();
}

void rtype::client::Level_1::updateObjectsPosition(sf::Vector2f deltaSize)
{
    (void)deltaSize;
}