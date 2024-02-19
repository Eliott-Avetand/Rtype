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
//  File:            Room.hpp                                                        //
//  Description:     File containing the Room class architecture                     //
//                                                                                   //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <iostream>
#include <thread>
#include <map>
#include <chrono>

#include <boost/asio.hpp>

#include "Client.hpp"
#include "DataStruct.hpp"
#include "Game.hpp"

/**
 * @brief This class is used to store room's informations
 * 
 * @details This class is used to store room's informations
*/
class Room
{
    public:
        // Room(boost::asio::ip::udp::endpoint roomEndpoint, boost::asio::ip::udp::socket *socket, boost::asio::ip::udp::endpoint creatorEndpoint, std::thread::id threadID, std::string roomID);
        
        /**
         * @brief This is the constructor of the Room class.
         * 
         * @details This is the constructor of a room.
         *          It will create a room with the given parameters.
         *          It will then create a thread to handle the room.
         * 
         * @param roomEndpoint : The ip address of the room
         * @param ID : The ID of the room (used to join the room)
         * @param creatorEndpoint : The endpoint of the creator of the room
        */
        Room(boost::asio::ip::udp::endpoint roomEndpoint, std::string ID, boost::asio::ip::udp::endpoint creatorEndpoint);
        ~Room();

        /**
         * @brief This function is used to get the state of the room
         * 
         * @return RoomState : The state of the room
        */
        RoomState getState(void) const;

        /**
         * @brief This function is used to get the data of the room
         * 
         * @return std::string : The data of the room
        */
        std::string getData(void);
        
        /**
         * @brief This function is used to get the ID of the room
         * 
         * @return std::string : The ID of the room
        */
        std::string getID(void) const;

        /**
         * @brief This function is used to get the number of players in the room
         * 
         * @return char : The number of players in the room
        */
        size_t getPlayerCount(void) const;

        /**
         * @brief This function is used to get the id of the thread associated with the room
         * 
         * @return std::thread::id : The id of the thread associated with the room
        */
        std::thread::id getAssociatedThreadID() const;

        /**
         * @brief This function is used to get the level chosen by the players
         * 
         * @return size_t : The level chosen by the players
        */
        size_t getChosenLevel(void) const;

        bool allPlayersAreInGame(void);

        /**
         * @brief This function is used to get all the clients in the room
         * 
         * @return std::vector<Client *> : The clients in the room
        */
        std::vector<Client *> getClients() const;

        /**
         * @brief This function is used to get the endpoint of the room
         * 
         * @return boost::asio::ip::udp::endpoint : The endpoint of the room
        */
        boost::asio::ip::udp::endpoint getRoomEndpoint() const;

        /**
         * @brief This function is used to get the state of the room
         * 
         * @param state : The state of the room
        */
        void setState(RoomState state);

        /**
         * @brief This function is used to set the endpoint of the room
         * 
         * @param roomEndpoint : The endpoint of the room
        */
        void setRoomEndpoint(boost::asio::ip::udp::endpoint roomEndpoint);

        /**
         * @brief This function is used to set the number of players in the room
         * 
         * @param playerCount : The number of players in the room
        */
        void setPlayerCount(size_t playerCount);

        /**
         * @brief This function is used to set the id of the thread associated with the room
         * 
         * @param associatedThreadID : The id of the thread associated with the room
        */
        void setAssociatedThreadID(std::thread::id associatedThreadID);

        /**
         * @brief This function is used to set the level chosen by the players
         * 
         * @param chosenLevel : The level chosen by the players
        */
        void setChosenLevel(size_t chosenLevel);

        /**
         * @brief This function is used to set the clients in the room
         * 
         * @param clients : The clients in the room
        */
        void setClients(std::vector<Client *> clients);

        
        // std::vector<Player> getPlayers() const;
        // void setPlayers(std::vector<Player> players);
        // void addPlayer(Player player);
        // void removePlayer(Player player);

        /**
         * @brief This function is used to run the room
         * 
         * @details This function is used to run the room.
         *          It will print the room's informations.
         *          it will handle the commands received by the clients.
        */
        void run(void);
        
        /**
         * @brief This function is used to send data to the clients in the room
         * 
         * @param data : The data to send to the clients
        */
        void sendDataToClients(DataStruct data);

        /**
         * @brief This function is used to send data to the clients in the room except the client given in parameter
         * 
         * @param data : The data to send to the clients
         * @param client : The client to not send the data to
        */
        void sendDataToOtherClients(DataStruct data, boost::asio::ip::udp::endpoint client);

        /**
         * @brief This function is used to close the room
         * 
         * @details This function is used to close the room.
         *          It will close the socket and the thread associated with the room.
        */
        void closeRoom(void);

        /**
         * @brief This function is used to print the room's informations
        */
        void printInfo(void);

        /**
         * @brief This function is used to add a client to the room
         * 
         * @param client : The client to add to the room
        */
        void addClient(Client *client);

        /**
         * @brief This function is used to remove a client from the room
         * 
         * @param client : The client to remove from the room
        */
        void removeClient(Client *client);

        /**
         * @brief This function is used to get a client by its endpoint
         * 
         * @param endpoint : The endpoint of the client
         * @return Client * : The client
        */
        Client *getClientByEndpoint(boost::asio::ip::udp::endpoint endpoint);

        void setErrors(bool error);

        bool checkNextGameLevel();

        void waiting();
        void playing();

        void handleLeave(DataStruct data);
        void handleInfo(DataStruct data);
        void handleName(DataStruct data);
        void handlePlayerName(DataStruct data);
        void handleStart(DataStruct data);
        void handleReady(DataStruct data);
        void handleStop(DataStruct data);
        void handlePause(DataStruct data);
        void handleResume(DataStruct data);
        void handlePlayerCount(DataStruct data);
        void handleShoot(DataStruct data);
        void handleMove(DataStruct data);
        void handleDie(DataStruct data);
        void handlePlayerID(DataStruct data);
        void handlePlayerIsPlaying(DataStruct data);
        void handlePlayerIsLoading(DataStruct data);
        void handleEnemiesCount(DataStruct data);
        void handleBreeds(DataStruct data);
        void handleBreedsNumbers(DataStruct data);
        void handlePing(DataStruct data);
        void handlePong(DataStruct data);
        void handleSetPlayerColor(DataStruct data);
        void handleGetPlayerColor(DataStruct data);

        void sendPingToClients(void);

        void sendClosing(void);

        void sendPlayersPosition(void);

        void sendEnemies(void);
        void sendEnemiesDamage(std::string entityID, int playerID);
        void sendEnemiesShooting(std::string entityID);

        void sendPlayerScore(int score, int playerID);
        void sendPlayerDamage(std::string enemyID, int playerID);

        void sendLevelStatus(LevelState levelStatus);

        void sendAsteroids(void);
        void sendAsteroidsDestroy(std::string entityID);
        void sendPlayerAsteroidDamage(std::string entityID, int playerID);

        void sendBossSpawn(void);
        void sendBossMissile(void);
        void sendBossMissileDestroyed(std::string missileId);
        void sendBossHeartInfos(int heartIndex, std::string heartId);
        void sendBossHeartMissile(void);
        void sendBossHeartMissileDestroyed(std::string heartId, std::string missileId);
        void sendBossTailPosition(void);
        void sendBossDamage(std::string heartId, int playerID);
        void sendBossHeartDestroy(std::string heartId);
        void sendPlayerBossDamage(int playerID);

        friend std::ostream& operator<<(std::ostream& os, const Room& r);

    private:

        // Network
        boost::asio::io_context _context;
        boost::asio::ip::udp::socket _roomSocket;
        boost::asio::ip::udp::endpoint _roomEndpoint;

        boost::asio::ip::udp::endpoint _clientEndpoint;

        // Thread
        std::thread _roomThread;
        std::thread::id _threadID;

        // Room DeltaTime
        double _lag;


        // Room info
        std::string _roomID;
        int _Hz;
        sf::Time _keepTime;
        sf::Clock _clock;
        sf::Clock _test;
        sf::Clock _waitingClock;
        std::chrono::steady_clock::time_point _start;
        bool _isError;

        RoomState _state;
        size_t _playerCount;
        unsigned short _levelID;
        size_t _maxPlayerCount;
        std::vector<Client *> _clients;

        std::unique_ptr<Game> _gameInstance;
};

typedef void (Room::*handleCommmandRoom)(DataStruct data);