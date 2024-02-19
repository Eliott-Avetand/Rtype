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
//  File:            Room.cpp                                                        //
//  Description:     File containing all Room's class methods                        //
//                                                                                   //
//                                                                                   //
// ********************************************************************************* //

#include "../include/Room.hpp"
#include <cstdlib>
#include "UDPServer.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

std::map<CommandCode, handleCommmandRoom> myMap = {
    {CommandCode::LEAVE, &Room::handleLeave},
    {CommandCode::INFO, &Room::handleInfo},
    {CommandCode::NAME, &Room::handleName},
    {CommandCode::PLAYERNAME, &Room::handlePlayerName},
    {CommandCode::START, &Room::handleStart},
    {CommandCode::READY, &Room::handleReady},
    {CommandCode::STOP, &Room::handleStop},
    {CommandCode::PAUSE, &Room::handlePause},
    {CommandCode::RESUME, &Room::handleResume},
    {CommandCode::PCOUNT, &Room::handlePlayerCount},
    {CommandCode::SHOOT, &Room::handleShoot},
    {CommandCode::MOVE, &Room::handleMove},
    {CommandCode::DIE, &Room::handleDie},
    {CommandCode::ID, &Room::handlePlayerID},
    {CommandCode::ISPLAYING, &Room::handlePlayerIsPlaying},
    {CommandCode::ISLOADING, &Room::handlePlayerIsLoading},
    {CommandCode::SETPLAYERCOLOR, &Room::handleSetPlayerColor},
    {CommandCode::GETPLAYERCOLOR, &Room::handleGetPlayerColor},
    {CommandCode::BREED, &Room::handleBreeds},
    {CommandCode::BREEDNUMBER, &Room::handleBreedsNumbers},
    {CommandCode::PING, &Room::handlePing},
    {CommandCode::PONG, &Room::handlePong}
};

Room::Room(boost::asio::ip::udp::endpoint roomEndpoint, std::string id, boost::asio::ip::udp::endpoint creatorEndpoint)
    : _context(), _roomSocket(this->_context)
{
    this->_roomID = id;
    this->_playerCount = 0;
    this->_levelID = 1;
    this->_state = WAITING;
    this->_gameInstance = nullptr;
    this->_Hz = 60;
    this->_isError = false;

    this->_clock.restart();
    this->_waitingClock.restart();
    this->_lag = 0;
    this->_keepTime = _clock.getElapsedTime();

    this->_maxPlayerCount = 4;

    std::cout << this << std::endl;

    // this->_players.push_back(new Player());
    // this->_clients.push_back(new Client(creatorEndpoint));

    this->_roomSocket.open(boost::asio::ip::udp::v4());
    this->_roomEndpoint = roomEndpoint;
    this->_roomSocket.bind(this->_roomEndpoint);
    this->_roomEndpoint = this->_roomSocket.local_endpoint();
    //this->_roomSocket.set_option(boost::asio::socket_base::send_buffer_size(128192 * 2));

    this->addClient(new Client(creatorEndpoint, 0));
    ////////////////////////////////////////////////////
    DataStruct data = {
        .command = OK,
    };

    data.currentRoom.playerCount = 1;
    data.currentRoom.port = this->_roomEndpoint.port();
    data.currentRoom.playerCount = this->_playerCount;

    for (int i = 0; i != 5; i++) {
        data.currentRoom.password[i] = this->_roomID[i];
    }
    ////////////////////////////////////////////////////
    std::cout << "Sending room info to client" << std::endl;
    this->sendDataToClients(data);
    std::cout << "Room info sent to client" << std::endl;
    //

    // Launch thread
    std::thread t(&Room::run, this);
    this->_roomThread = std::move(t);
    this->_threadID = this->_roomThread.get_id();
}

Room::~Room(){}

std::ostream& operator<<(std::ostream& os, const Room& r)
{
    os << "[ROOM] Room ID: " << r.getID() << "\n";
    os << "Room IP: " << r.getRoomEndpoint().address().to_string() << "\n";
    os << "Room Port: " << r.getRoomEndpoint().port() << "\n";
    os << "Room State: " << r.getState() << "\n";
    os << "Room Player Count: " << r.getPlayerCount() << "\n";
    os << "Room Chosen Level: " << r.getChosenLevel() << "\n";
    return os;
}

void Room::printInfo(void)
{
    std::string stateSTR = "";

    switch (this->_state) {
        case WAITING:
            stateSTR = "WAITING";
            break;
        case PLAYING:
            stateSTR = "PLAYING";
            break;
        case ENDED:
            stateSTR = "ENDED";
            break;
        case CLOSED:
            stateSTR = "CLOSED";
            break;
    }
    std::cout << "[ROOM] Room ID:\t\t" << this->_roomID << "\n";
    std::cout << "[ROOM] Room IP:\t\t" << this->_roomEndpoint.address().to_string() << "\n";
    std::cout << "[ROOM] Room Port:\t\t" << this->_roomEndpoint.port() << "\n";
    std::cout << "[ROOM] Room State:\t\t" << stateSTR << "\n";
    std::cout << "[ROOM] Room Player Count:\t" << this->_playerCount << "\n";
    std::cout << "[ROOM] Room Chosen Level:\t" << this->_levelID << std::endl;
}

RoomState Room::getState(void) const {
    return this->_state;
}

void Room::setState(RoomState state) {
    this->_state = state;
}

std::string Room::getID(void) const {
    return this->_roomID;
}

boost::asio::ip::udp::endpoint Room::getRoomEndpoint() const {
    return this->_roomEndpoint;
}

void Room::setRoomEndpoint(boost::asio::ip::udp::endpoint roomEndpoint) {
    this->_roomEndpoint = roomEndpoint;
}

size_t Room::getPlayerCount(void) const {
    return this->_playerCount;
}

void Room::setPlayerCount(size_t playerCount) {
    this->_playerCount = playerCount;
}

std::thread::id Room::getAssociatedThreadID(void) const {
    return this->_threadID;
}

void Room::setAssociatedThreadID(std::thread::id associatedThreadID) {
    this->_threadID = associatedThreadID;
}

size_t Room::getChosenLevel(void) const {
    return this->_levelID;
}

void Room::setChosenLevel(size_t chosenLevel) {
    this->_levelID = chosenLevel;
}

bool Room::allPlayersAreInGame(void)
{
    for (int i = 0; i < this->_clients.size(); i++) {
        if (!this->_clients[i]->getIsPlaying())
            return false;
    }
    return true;
}

std::vector<Client *> Room::getClients(void) const {
    return this->_clients;
}

void Room::setClients(std::vector<Client *> clients) {
    this->_clients = clients;
}

void Room::addClient(Client *client) {
    this->_start = std::chrono::steady_clock::now();
    this->_clients.push_back(client);
    this->_playerCount++;

}

void Room::removeClient(Client *client) {
    this->_clients.erase(std::remove(this->_clients.begin(), this->_clients.end(), client), this->_clients.end());
    this->_playerCount--;
}

void Room::setErrors(bool error)
{
    this->_isError = error;
}

Client *Room::getClientByEndpoint(boost::asio::ip::udp::endpoint endpoint)
{
    for (auto client : this->_clients) {
        if (client->getEndpoint() == endpoint)
            return client;
    }
    return nullptr;
}

bool Room::checkNextGameLevel(void)
{
    try {
        boost::property_tree::ptree root;

        boost::property_tree::read_json("data/level_" + std::to_string(this->_levelID) + "/level.json", root);
        return true;
    } catch (std::exception const& e) {
        return false;
    }
}

void Room::playing()
{
    double MS_PER_UPDATE = ((double)1 / (double)this->_Hz);
    sf::Time currentTime = this->_clock.getElapsedTime();


    if (this->_gameInstance == nullptr) {
        if (this->_levelID == 1) {
            this->_gameInstance = std::make_unique<Game>(this->_levelID, this->_playerCount, this);
            if (!this->_gameInstance)
                throw rtype::settings::Errors("Memory Error", "Not enough mermories");
        }
        else {
            if (this->checkNextGameLevel()) {
                this->_gameInstance =  std::make_unique<Game>(this->_levelID, this->_playerCount, this);
                if (!this->_gameInstance)
                    throw rtype::settings::Errors("Memory Error", "Not enough mermories");
            }
        }
        this->_waitingClock.restart();
    }

    if (this->_waitingClock.getElapsedTime().asSeconds() > 45 && !this->allPlayersAreInGame()) {
        this->sendDataToClients({.command = CommandCode::CLOSING});
        this->closeRoom();
    }

    this->_roomSocket.non_blocking(true);

    boost::system::error_code error;

    DataStruct dataRecv = {
    .command = (CommandCode)9999,
    };
    //std::cout << "[Attente] dans Room::playing" << std::endl;
    this->_roomSocket.receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_clientEndpoint, 0, error);
    //std::cout << "[R�ponse] dans Room::playing" << std::endl;

    dataRecv.clientEndpoint = this->_clientEndpoint;

    for (const auto& item : myMap) {
        if (item.first == dataRecv.command) {
            handleCommmandRoom handler = item.second;
            (this->*handler)(dataRecv);
            break;
        }
    }

    double elapsed = (double)currentTime.asSeconds() - (double)this->_keepTime.asSeconds();
    this->_keepTime = currentTime;
    this->_lag += elapsed;

    if (this->_gameInstance && this->allPlayersAreInGame() && !this->_isError) {

        while (this->_lag >= MS_PER_UPDATE) {
            this->_gameInstance->update();
            this->_lag -= MS_PER_UPDATE;
        }

        auto current = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = current - this->_start;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds).count();
        if (ms >= 500 && this->_gameInstance->getGameStatus() == LevelState::INGAME) {
            this->sendPingToClients();
            this->_start = std::chrono::steady_clock::now();
        }

        if (this->_test.getElapsedTime().asMilliseconds() >= 100) {

            if (this->_gameInstance->getBoss()) {

                if (!this->_gameInstance->getBoss()->isDead() && !this->_gameInstance->getBoss()->isDying()) {
                    this->sendBossMissile();
                    this->sendBossHeartMissile();
                    this->sendBossTailPosition();
                }
            }
            else {
                this->sendEnemies();
                this->sendPlayersPosition();
            }

            this->_test.restart();
        }

        if (this->_gameInstance->getGameStatus() != LevelState::INGAME) {
            for (auto& client : this->_clients) {
                client->setIsPlaying(false);
            }

            this->_waitingClock.restart();

            this->_gameInstance.reset();
            this->_gameInstance = nullptr;
            this->_levelID++;
        }
    }
}

void Room::waiting()
{
    boost::system::error_code error;

    this->_roomSocket.non_blocking(true);

    DataStruct dataRecv = {
    .command = (CommandCode)9999,
    };

    if (this->_clients.size() == 0)
        this->closeRoom();
    //std::cout << "[Attente] dans Room::waiting" << std::endl;
    this->_roomSocket.receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_clientEndpoint, 0, error);
    //std::cout << "[R�ponse] dans Room::waiting" << std::endl;
    dataRecv.clientEndpoint = this->_clientEndpoint;

    for (const auto& item : myMap) {
        if (item.first == dataRecv.command) {
            handleCommmandRoom handler = item.second;
            (this->*handler)(dataRecv);
            break;
        }
    }

    auto current = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = current - this->_start;

    if (elapsed_seconds.count() >= 2) {
        this->handleInfo(dataRecv);
        this->_start = std::chrono::steady_clock::now();
    }
}

void Room::run(void)
{
    //std::mutex m;

    this->_state = WAITING;

    this->_roomSocket.non_blocking(true);

    this->_start = std::chrono::steady_clock::now();
    auto loopStart = std::chrono::steady_clock::now();
    
    while (1) {

        if (this->_state == WAITING)
            this->waiting();

        if (this->_state == PLAYING) {
            this->playing();
        }

        if (this->_isError) {
            this->sendClosing();
        }

        if (this->_state == CLOSED) {
            std::cout << "[ROOM] Room is closed " << std::endl;
            break;
        }
    }
    return;
}

void Room::sendDataToClients(DataStruct data)
{
    std::mutex m;

    m.lock();

    for (auto client : this->_clients) {
        try {
            boost::system::error_code err;

            size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&data, sizeof(data)), client->getEndpoint(), 0, err);

            if (err) {
                std::cout << err.message() << std::endl;
                m.unlock();
                return;
            }
        } catch (std::exception const& e) {
            std::cerr << " ########################################################## [sendDataToClient] " << e.what() << std::endl;
            m.unlock();
        }
    }

    m.unlock();
    return;
}

void Room::closeRoom(void)
{
    this->_state = CLOSED;
    for (auto client : this->_clients)
        delete client;
    this->_clients.clear();

    std::cout << "[ROOM] Room " << this->getID() << " is closing..." << std::endl;

    if (this->_roomSocket.is_open())
        this->_roomSocket.close();
    if (this->_roomThread.joinable())
        this->_roomThread.join();
}

std::string Room::getData(void)
{
    std::string data;

    return data;

}

void Room::handleLeave(DataStruct data)
{
    std::cout << "[ROOM] Client " << this->_clientEndpoint.address().to_string() << " has left the room" << std::endl;
    ////////////////////////////////////////////////////////////////
    DataStruct dataSend = {
        .command = CommandCode::OK,
    };
    ////////////////////////////////////////////////////////////////
    try {
        size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
    } catch (std::exception const& e) {
        std::cerr << " ########################################################## [handleLeave] " << e.what() << std::endl;
    }
    this->removeClient(this->getClientByEndpoint(this->_clientEndpoint));

    // for (int i = 0; i < this->_clients.size(); i++) {
    //     this->_clients[i]->setPlayerID(i);
    // }

    this->handleInfo(data);

    if (this->_clients.size() == 0) {
        this->_state = CLOSED;
    }
}

void Room::handleName(DataStruct data)
{
    for (auto &client : this->_clients) {
        if (client->getEndpoint() == data.clientEndpoint) {
            std::string str(data.pseudo);
            client->setUsername(str);
            ////////////////////////////////////////////////////////////////
            DataStruct dataSend = {
                .command = CommandCode::OK,
            };
            // ////////////////////////////////////////////////////////////////
            break;
        }
    }
}

void Room::handleInfo(DataStruct data)
{

    std::mutex m;

    m.lock();

    try {
        ////////////////////////////////////////////////////////////////
        DataStruct dataSend = {
            .command = CommandCode::INFO,
            .currentRoom = {
                .playerCount = static_cast<char>(this->_playerCount),
                .state = this->_state,
            },
        };

        for (auto& client : this->_clients) {
            if (client->getUsername() == "_____") {

                std::cout << client->getUsername() << std::endl;

                DataStruct dataS = {
                    .command = CommandCode::CLOSING,
                };

                this->_roomSocket.send_to(boost::asio::buffer(&dataS, sizeof(DataStruct)), client->getEndpoint());

                this->removeClient(client);
            }
        }

        if (this->_clients.size() == 0) {
            this->_state = CLOSED;
            m.unlock();
            return;
        }

        for (int i = 0; i < this->_roomID.length(); i++)
            dataSend.currentRoom.password[i] = this->_roomID[i];

        for (int i = 0 ; i < this->_clients.size(); i++) {
            dataSend.currentRoom.players[i] = this->_clients[i]->getPlayerData();
        }

        ////////////////////////////////////////////////////////////////
        this->sendDataToClients(dataSend);
        m.unlock();
    } catch (std::exception &e) {
        std::cerr << "[handleInfo] " << e.what() << std::endl;
        m.unlock();
    }
    m.unlock();
    return;
}

void Room::handleReady(DataStruct data)
{
    for (auto &client : this->_clients) {
        if (client->getEndpoint() == data.clientEndpoint) {
            if (client->getIsReady() == true)
                client->setIsReady(false);
            else
                client->setIsReady(true);
            break;
        }
    }
}

void Room::handleStart(DataStruct data)
{
    try {

        if (this->_state != WAITING)
            return;

         for (auto& client : this->_clients) {
             //if (client->getIsReady() == false) {
             //    return;
             //}
            if (client->getUsername() == "_____")
                return;
         }



        ////////////////////////////////////////////////////////////////
        DataStruct dataSend = {
            .command = CommandCode::START,
        };
        ////////////////////////////////////////////////////////////////
        this->sendDataToClients(dataSend);
        this->_state = PLAYING;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Room::handleStop(DataStruct data)
{

}

void Room::handlePause(DataStruct data)
{

}

void Room::handleResume(DataStruct data)
{

}

void Room::handlePlayerName(DataStruct data)
{
    ////////////////////////////////////////////////////////////////

    for (auto& client : this->_clients) {
        if (client->getPlayerID() == data.currentPlayer.playerID) {

            DataStruct dataSend = {
                .command = CommandCode::OK,
            };

            strcpy(dataSend.currentPlayer.pseudo, client->getUsername().c_str());

            ////////////////////////////////////////////////////////////////
            try {
                size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
            } catch (std::exception const& e) {
                std::cerr << " ########################################################## [handlePlayerName] " << e.what() << std::endl;
            }

            break;
        }
    }
    return;
}

void Room::handlePlayerCount(DataStruct data)
{
    ////////////////////////////////////////////////////////////////

    char pcount = this->getPlayerCount();

    DataStruct dataSend = {
        .command = CommandCode::OK,
        .currentRoom = {
                .playerCount = static_cast<char>(this->_playerCount)
        },
    };

    ////////////////////////////////////////////////////////////////
    try {
        size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
    } catch (std::exception const& e) {
        std::cerr << " ########################################################## [handlePlayerCount] " << e.what() << std::endl;
    }
}

void Room::handleShoot(DataStruct data)
{
    std::pair<float, float> position = {0.0f, 0.0f};
    int id = 0;

    for (auto& client : this->_clients) {
        if (client->getEndpoint() == data.clientEndpoint) {
            if (this->_gameInstance) {

                std::vector<std::unique_ptr<rtype::actor::Actor>>& allPlayers = this->_gameInstance->getPlayers();
                id = client->getPlayerID();
                if (id >= 0 && id < 4)
                    position = { allPlayers[id]->getPosition().x,  allPlayers[id]->getPosition().y };

            }
            break;
        }
    }

    ////////////////////////////////////////////////////////////////
    DataStruct dataSend = {
        .command = CommandCode::SHOOT,
        .currentPlayer = {
            .playerID = id,
            .positions = position,
        }
    };
    ////////////////////////////////////////////////////////////////

    this->sendDataToOtherClients(dataSend, this->_clientEndpoint);

    if (_gameInstance) {

        std::vector<std::unique_ptr<rtype::actor::Actor>>& allPlayers = this->_gameInstance->getPlayers();
        
        if (id >= allPlayers.size()) {
            return;
        }
        allPlayers[id]->shoot();
    }

}

void Room::handlePlayerID(DataStruct data)
{
    char playerID = -1;

    for (auto& client : this->_clients) {
        if (client->getEndpoint() == data.clientEndpoint) {
            playerID = client->getPlayerID();
            break;
        }
    }

    ////////////////////////////////////////////////////////////////
    DataStruct dataSend = {
        .command = CommandCode::OK,
        .currentPlayer = {
            .playerID = playerID,
        }
    };
    ////////////////////////////////////////////////////////////////

    try {

    size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
    } catch (std::exception const& e) {
        std::cerr << " ########################################################## [handlePlayerID] " << e.what() << std::endl;
    }
}

void Room::handleMove(DataStruct data)
{
    for (auto &client : this->_clients) {
        if (client->getEndpoint() == this->_clientEndpoint) {

            if (client->getPositions().first == data.currentPlayer.positions.first && data.currentPlayer.positions.second == client->getPositions().second)
                break;

            if (this->_gameInstance) {

                std::vector<std::unique_ptr<rtype::actor::Actor>>& allPlayers = this->_gameInstance->getPlayers();

                allPlayers[client->getPlayerID()]->setPosition(sf::Vector2f(data.currentPlayer.positions.first, data.currentPlayer.positions.second));
                allPlayers[client->getPlayerID()]->setDirection(data.currentPlayer.moveDirection);
            }
            break;
        }
    }
}

void Room::handleDie(DataStruct data)
{

}

void Room::sendDataToOtherClients(DataStruct data, boost::asio::ip::udp::endpoint clientEndpoint)
{
    boost::system::error_code error;

    for (auto &client : this->_clients) {
        if (client->getEndpoint() == clientEndpoint)
            continue;
        if (client->getIsPlaying()) {
            try {
                size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&data, sizeof(data)), client->getEndpoint(), 0, error);
            } catch (std::exception const &e) {
                std::cerr << " ########################################################## [SendDataToOtherClient] " << e.what() << std::endl;
            }
        }
    }
}

void Room::handlePlayerIsLoading(DataStruct data) {
    for (auto& client : this->_clients) {
        if (client->getEndpoint() == this->_clientEndpoint) {
            client->setIsPlaying(false);
            break;
        }
    }

    ////////////////////////////////////////////
    DataStruct dataSend = {
        .command = CommandCode::OK,
    };
    ////////////////////////////////////////////
    try {
        size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
    } catch (std::exception const& e) {
        std::cerr << " ########################################################## [handlePlayerIsLoading] " << e.what() << std::endl;
    }
}

void Room::handlePlayerIsPlaying(DataStruct data) {
    for (auto &client : this->_clients) {
        if (client->getEndpoint() == this->_clientEndpoint) {
            client->setIsPlaying(true);
            break;
        }
    }

    ////////////////////////////////////////////
    DataStruct dataSend = {
        .command = CommandCode::OK,
    };
    ////////////////////////////////////////////

    try {
        size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
    } catch (std::exception const& e) {
        std::cerr << " ########################################################## [PlayerIsPlaying]" << e.what() << std::endl;
    }
}

void Room::sendPlayerScore(int score, int playerID)
{
    if (this->_gameInstance) {

        DataStruct dataSend = {
            .command = PLAYERSCORE,
            .currentPlayer = {
                .playerID = playerID,
                .score = score,
            },
        };
        this->sendDataToClients(dataSend);
    
    }
    else {
        return;
    }

}

void Room::sendPlayerDamage(std::string enemyID, int playerID)
{
    if (this->_gameInstance) {

        std::vector<std::unique_ptr<rtype::actor::Actor>>& allEnemies = this->_gameInstance->getEnemies();

        EnemyData enemyInfo;

        for (int i = 0; i < allEnemies.size(); i++) {
            if (allEnemies[i]->getEntityID() == enemyID) {
                enemyInfo = this->_gameInstance->getEnemyDatas(i);
                break;
            }
        }

        DataStruct dataSend = {
            .command = PLAYERDAMAGE,
            .currentRoom = {
                .enemyInfos = enemyInfo,
            },
            .currentPlayer = {
                .playerID = playerID,
            },
        };
        this->sendDataToClients(dataSend);
    }
    else
        return;
}

void Room::sendClosing(void)
{
    this->sendDataToClients({.command = CLOSING});
}

void Room::sendEnemies(void)
{
    if (this->_gameInstance) {
        std::vector<std::unique_ptr<rtype::actor::Actor>>& allEnemies = this->_gameInstance->getEnemies();

        EnemyData enemyInfo;

        for (int i = 0; i < allEnemies.size(); i++) {

            enemyInfo = this->_gameInstance->getEnemyDatas(i);

            DataStruct dataSend = {
                .command = ENEMY,
                .currentRoom = {
                    .enemyInfos = enemyInfo,
                    .enemyCount = static_cast<int>(allEnemies.size()),
                },
            };

            this->sendDataToClients(dataSend);

        }
    }
    else
        return;
}

void Room::sendEnemiesDamage(std::string entityID, int playerID)
{
    if (this->_gameInstance) {

        std::vector<std::unique_ptr<rtype::actor::Actor>>& allEnemies = this->_gameInstance->getEnemies();

        EnemyData enemy;

        for (int i = 0; i < allEnemies.size(); i++) {
            if (allEnemies[i]->getEntityID() == entityID) {
                enemy = this->_gameInstance->getEnemyDatas(i);
                break;
            }
        }

        DataStruct dataSend = {
            .command = ENEMYDAMAGE,
            .currentRoom = {
                .enemyInfos = enemy,
                .enemyCount = static_cast<int>(allEnemies.size()),
            },
            .currentPlayer = {
                .playerID = playerID,
            },
        };
        this->sendDataToClients(dataSend);
    }
    else
        return;
}

void Room::handleEnemiesCount(DataStruct data)
{
    if (this->_gameInstance) {
        std::vector<std::unique_ptr<rtype::actor::Actor>>& allEnemies = this->_gameInstance->getEnemies();

        ////////////////////////////////////////////////////////////////
        DataStruct dataSend = {
            .command = OK,
            .currentRoom = {
                .enemyCount = static_cast<int>(allEnemies.size()),
            },
        };

        ////////////////////////////////////////////////////////////////

        try {
            size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
        } catch (std::exception const& e) {
            std::cerr << " ########################################################## [handleEnemiesCount]" << e.what() << std::endl;
        }
    }
    else
        return;
}

void Room::handleBreeds(DataStruct data)
{
    DataStruct dataSend = {
        .command = OK,
    };

    size_t byteSend = 0;

    try {
        if (this->_gameInstance) {
            std::vector<std::unique_ptr<rtype::actor::Breed>>& allBreeds = this->_gameInstance->getBreeds();

            if (data.currentRoom.askedBreed > allBreeds.size() - 1) {
                dataSend.command = KO;
                try {
                    byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
                } catch (std::exception const& e) {
                    std::cerr << " ########################################################## [handleBreeds]" << e.what() << std::endl;
                }
                return;
            }

            BreedData breedData;

            strcpy(breedData.sprite, allBreeds[data.currentRoom.askedBreed]->getSpritePath().c_str());
            strcpy(breedData.name, allBreeds[data.currentRoom.askedBreed]->getName().c_str());
            breedData.maxHealth = allBreeds[data.currentRoom.askedBreed]->getHealth();
            breedData.occurences = allBreeds[data.currentRoom.askedBreed]->getOccurences();

            ////////////////////////////////////////////////////////////////
            DataStruct dataSend = {
                .command = OK,
                .currentRoom = {
                    .breed = breedData,
                },
            };

            ////////////////////////////////////////////////////////////////

            byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
        }
        else {
            dataSend.command = KO;
            byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);

            return;
        }
    } catch (std::exception const& e) {
        std::cerr << " ########################################################## [handleBreeds] " << e.what() << std::endl;
    }
}

void Room::handleBreedsNumbers(DataStruct data)
{
    if (this->_gameInstance) {
        std::vector<std::unique_ptr<rtype::actor::Breed>>& allBreeds = this->_gameInstance->getBreeds();

        ////////////////////////////////////////////////////////////////
        DataStruct dataSend = {
            .command = OK,
            .currentRoom = {
                .breedCount = static_cast<char>(allBreeds.size()),
            },
        };

        ////////////////////////////////////////////////////////////////

        try {
            size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
        } catch (std::exception const& e) {
            std::cerr << " ########################################################## [handleBreedsNumbers] " << e.what() << std::endl;
        }

    }
    else
        return;
}

void Room::sendEnemiesShooting(std::string entityID)
{
    if (this->_gameInstance) {

        std::vector<std::unique_ptr<rtype::actor::Actor>>& allEnemies = this->_gameInstance->getEnemies();

        EnemyData enemy;

        for (int i = 0; i < allEnemies.size(); i++) {
            if (allEnemies[i]->getEntityID() == entityID) {
                enemy = this->_gameInstance->getEnemyDatas(i);
                break;
            }
        }

        DataStruct dataSend = {
            .command = ENEMYSHOOT,
            .currentRoom = {
                .enemyInfos = enemy,
                .enemyCount = static_cast<int>(allEnemies.size()),
            },
        };
        this->sendDataToClients(dataSend);
    }
    else
        return;
}

void Room::sendLevelStatus(LevelState levelStatus)
{
    if (this->_gameInstance) {

        DataStruct dataSend = {
            .command = LEVELSTATUS,
            .currentRoom = {
                .levelState = levelStatus
            },
        };
        this->sendDataToClients(dataSend);
    }
    else
        return;
}

void Room::handlePing(DataStruct data)
{
    DataStruct dataSend = {
        .command = CommandCode::PONG,
    };
    try {
        size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
    } catch (std::exception const& e) {
        std::cerr << " ########################################################## [handlePing]" << e.what() << std::endl;
    }
}

void Room::handlePong(DataStruct data)
{
    for (auto& client : this->_clients) {
        if (client->getEndpoint() == this->_clientEndpoint) {
            client->updateLatency();

            DataStruct dataSend = {
               .command = CommandCode::LATENCY,
                .currentPlayer = {
                     .latency = client->getLatency(),
                },
            };

            try {
                this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
            } catch (std::exception const& e) {
                std::cerr << " ########################################################## [handlePong] " << e.what() << std::endl;
            }

            break;
        }
    }
}

void Room::sendPingToClients(void)
{

    DataStruct dataSend = {
        .command = CommandCode::PING,
    };

    for (auto& client : this->_clients) {
        if (client->getIsPlaying()) {
            client->setTimestampPing();
            try {
                size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), client->getEndpoint());
            } catch (std::exception const& e) {
                std::cerr << " ########################################################## [sendPingToClient] " << e.what() << std::endl;
            }
        }
    }
}

void Room::sendAsteroids(void)
{
    if (this->_gameInstance) {
        std::vector<std::unique_ptr<rtype::actor::Asteroid>>& allAsteroids = this->_gameInstance->getAsteroids();

        AsteroidsDatas asteroidInfos;

        for (int i = 0; i < allAsteroids.size(); i++) {
            asteroidInfos = this->_gameInstance->getAsteroidDatas(i);

            if (asteroidInfos.isDestroyed)
                continue;

            DataStruct dataSend = {
                .command = ASTEROID,
                .currentRoom = {
                    .asteroidInfos = asteroidInfos,
                },
            };

            this->sendDataToClients(dataSend);
        }
    }
    else
        return;
}

void Room::sendAsteroidsDestroy(std::string EntityID)
{
    if (this->_gameInstance) {
        std::vector<std::unique_ptr<rtype::actor::Asteroid>>& allAsteroids = this->_gameInstance->getAsteroids();

        AsteroidsDatas asteroidInfos;

        for (int i = 0; i < allAsteroids.size(); i++) {
            if (EntityID == allAsteroids[i]->getId()) {
                asteroidInfos = this->_gameInstance->getAsteroidDatas(i);
                break;
            }

        }

        DataStruct dataSend = {
            .command = ASTEROIDDESTROY,
            .currentRoom = {
                .asteroidInfos = asteroidInfos,
            },
        };

        this->sendDataToClients(dataSend);
    }
    else
        return;
}

void Room::sendPlayerAsteroidDamage(std::string entityID, int playerID)
{
    if (this->_gameInstance) {
        std::vector<std::unique_ptr<rtype::actor::Asteroid>>& allAsteroids = this->_gameInstance->getAsteroids();

        AsteroidsDatas asteroidInfos;

        for (int i = 0; i < allAsteroids.size(); i++) {
            if (entityID == allAsteroids[i]->getId()) {
                asteroidInfos = this->_gameInstance->getAsteroidDatas(i);
                break;
            }
        }

        DataStruct dataSend = {
            .command = PLAYERASTEROIDDAMAGE,
            .currentRoom = {
                .asteroidInfos = asteroidInfos,
            },
            .currentPlayer = {
                .playerID = playerID,
            },
        };

        this->sendDataToClients(dataSend);
    }
    else
        return;
}

void Room::handleSetPlayerColor(DataStruct data)
{
    for (auto& client : this->_clients) {
        if (client->getEndpoint() == data.clientEndpoint) {
            client->setColors(data.currentPlayer.playerColors);
            ////////////////////////////////////////////////////////////////
            DataStruct dataSend = {
                .command = CommandCode::OK,
            };
            // ////////////////////////////////////////////////////////////////
            
            try {
                size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
            } catch (std::exception const& e) {
                std::cerr << " ########################################################## [handleSetPlayerColor] " << e.what() << std::endl;
            }

            break;
        }
    }
}

void Room::handleGetPlayerColor(DataStruct data)
{
    for (auto& client : this->_clients) {
        if (client->getEndpoint() == data.clientEndpoint) {
            for (auto& searchClient : this->_clients) {
                if (searchClient->getPlayerID() == data.currentPlayer.playerID) {

                    ////////////////////////////////////////////////////////////////
                    DataStruct dataSend = {
                        .command = CommandCode::OK,
                        .currentPlayer {
                            .playerColors = searchClient->getColors(),
                        },
                    };
                    try {
                        size_t byteSend = this->_roomSocket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_clientEndpoint);
                    } catch (std::exception const& e) {
                        std::cerr << " ########################################################## [handleGetPlayerColor]" << e.what() << std::endl;
                    }

                    return;
                }
            }
            // ////////////////////////////////////////////////////////////////
        }
    }
}

void Room::sendBossTailPosition(void)
{
    if (this->_gameInstance) {
        std::vector<std::unique_ptr<rtype::client::utilities::AnimatedSprite>>& allTails = this->_gameInstance->getBoss()->getTails();
        
        dobkeratopsDatas data;
        
        for (int i = 0; i < allTails.size(); i++) {

            data.tailPositions = this->_gameInstance->getBossTailsPosition(i);
            data.tailIndex = i;

            DataStruct dataSend = {
                .command = BOSSTAILS,
                .currentRoom = {
                    .bossInfos = data,
                },
            };

            this->sendDataToClients(dataSend);
        }
    }
    else
        return;
}

void Room::sendBossHeartMissileDestroyed(std::string heartId, std::string missileId)
{
    if (this->_gameInstance) {
        std::vector<std::unique_ptr<rtype::actor::DobkeratopsHeart>>& allHearts = this->_gameInstance->getBoss()->getHearts();

        dobkeratopsDatas bossData;

        for (int i = 0; i < allHearts.size(); i++) {

            if (heartId == allHearts[i]->_id) {
                strcpy(bossData.heartId, allHearts[i]->_id.c_str());
                std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& allMissiles = allHearts[i]->getMissile();

                for (int missile = 0; missile < allMissiles.size(); missile++) {
                    
                    if (missileId == allMissiles[missile]->getId()) {

                        bossData.missileDatas = this->_gameInstance->getBossHeartMissileDatas(allHearts[i]->_id, missile);

                        DataStruct dataSend = {
                            .command = BOSSHEARTMISSILEDESTROYED,
                            .currentRoom = {
                                .bossInfos = bossData,
                            },
                        };

                        this->sendDataToClients(dataSend);

                        return;
                    }
                }
            }
        }
    }
    else
        return;
}

void Room::sendBossHeartMissile()
{
    if (this->_gameInstance) {
        std::vector<std::unique_ptr<rtype::actor::DobkeratopsHeart>>& allHearts = this->_gameInstance->getBoss()->getHearts();

        dobkeratopsDatas bossData;

        for (int i = 0; i < allHearts.size(); i++) {
            strcpy(bossData.heartId, allHearts[i]->_id.c_str());
            std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& allMissiles = allHearts[i]->getMissile();

            for (int missile = 0; missile < allMissiles.size(); missile++) {
                bossData.missileDatas = this->_gameInstance->getBossHeartMissileDatas(allHearts[i]->_id, missile);

                DataStruct dataSend = {
                    .command = BOSSHEARTMISSILE,
                    .currentRoom = {
                        .bossInfos = bossData,
                    },
                };

                this->sendDataToClients(dataSend);
            }
        }
    }
    else
        return;
}

void Room::sendBossMissileDestroyed(std::string missileId)
{
    if (this->_gameInstance) {
        std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& allMissiles = this->_gameInstance->getBoss()->getMissile();

        dobkeratopsDatas bossData;

        for (int i = 0; i < allMissiles.size(); i++) {

            if (allMissiles[i]->getId() == missileId) {

                bossData.missileDatas = this->_gameInstance->getBossMissileDatas(i);

                DataStruct dataSend = {
                    .command = BOSSMISSILEDESTROYED,
                    .currentRoom = {
                        .bossInfos = bossData,
                    },
                };

                this->sendDataToClients(dataSend);
            }
        }
    }
    else
        return;
}

void Room::sendBossMissile()
{
    if (this->_gameInstance) {
        std::vector<std::unique_ptr<rtype::actor::MissileBoss>>& allMissiles = this->_gameInstance->getBoss()->getMissile();

        dobkeratopsDatas bossData;

        for (int i = 0; i < allMissiles.size(); i++) {

            bossData.missileDatas = this->_gameInstance->getBossMissileDatas(i);

            DataStruct dataSend = {
                .command = BOSSMISSILE,
                .currentRoom = {
                    .bossInfos = bossData,
                },
            };

            if (bossData.missileDatas.isDestroyed)
                continue;

            this->sendDataToClients(dataSend);
        
        }
    }
    else
        return;
}

void Room::sendPlayerBossDamage(int playerID)
{
    if (this->_gameInstance) {

        DataStruct dataSend = {
            .command = PLAYERBOSSDAMAGE,
            .currentPlayer = {
                .playerID = playerID,
            },
        };

        this->sendDataToClients(dataSend);
    }
    else
        return;
}

void Room::sendBossDamage(std::string heartId, int playerID)
{
    if (this->_gameInstance) {

        dobkeratopsDatas bossData;
        strcpy(bossData.heartId, heartId.c_str());

        DataStruct dataSend = {
            .command = BOSSDAMAGE,
            .currentRoom = {
                .bossInfos = bossData,
            },
            .currentPlayer = {
                .playerID = playerID,
            },
        };

        this->sendDataToClients(dataSend);
    }
    else
        return;
}

void Room::sendBossSpawn()
{
    if (this->_gameInstance) {

        DataStruct dataSend = {
            .command = BOSSSPAWN,
        };

        this->sendDataToClients(dataSend);
    }
    else
        return;
}

void Room::sendBossHeartInfos(int heartIndex, std::string heartId)
{
    if (this->_gameInstance) {
        dobkeratopsDatas bossData;

        bossData.heartIndex = heartIndex;
        strcpy(bossData.heartId, heartId.c_str());

        DataStruct dataSend = {
            .command = BOSSHEARTINFO,
            .currentRoom = {
                .bossInfos = bossData,
            },
        };

        this->sendDataToClients(dataSend);
    }
    else
        return;
}

void Room::sendBossHeartDestroy(std::string heartId)
{
    if (this->_gameInstance) {
        dobkeratopsDatas bossData;

        strcpy(bossData.heartId, heartId.c_str());

        DataStruct dataSend = {
            .command = BOSSHEARTDESTROY,
            .currentRoom = {
                .bossInfos = bossData,
            },
        };

        this->sendDataToClients(dataSend);

    }
    else
        return;
}

void Room::sendPlayersPosition(void)
{
    if (this->_gameInstance) {
        std::vector<std::unique_ptr<rtype::actor::Actor>>& players = this->_gameInstance->getPlayers();

        for (unsigned short i = 0; i < players.size(); i++) {
            PlayerData pData;

            rtype::actor::Player *playerPtr = dynamic_cast<rtype::actor::Player *>(players[i].get());
            if (!playerPtr)
                throw rtype::settings::Errors("Can't dynamic cast player", "MemoryError:");

            pData.playerID = playerPtr->getID();
            pData.positions = std::pair<float, float>(players[i]->getPosition().x, players[i]->getPosition().y);
            pData.moveDirection = players[i]->getDirection();

            DataStruct dataSend = {
                .command = MOVE,
                .currentPlayer = pData,
            };

            this->sendDataToClients(dataSend);
        }


    }
    else
        return;
}
