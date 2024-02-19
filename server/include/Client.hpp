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
//  File:            Client.hpp                                                      //
//  Description:     File containing all Client's class methods                      //
//                                                                                   //
//                                                                                   //
// ********************************************************************************* //

#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <numeric>
#include <cmath>
#include "DataStruct.hpp"

/**
 * @brief This class is used to store client's informations
 * 
 * @details This class is used to store client's informations
 * 
 * NONE: The client is not connected
 * CONNECTED: The client is connected
 * DISCONNECTED: The client is disconnected
*/
typedef enum {
    NONE,
    CONNECTED,
    DISCONNECTED
} ClientState;

/**
 * @brief This class is used to store client's informations
 * 
 * @details This class is used to store client's informations
*/
class Client {
    public:
        /**
         * @brief Construct a new Client object
         * 
         * @details This function will construct a new Client object
         * 
         * @param endpoint a boost::asio::ip::udp::endpoint containing the client's IP address
         * @param username a std::string containing the client's username
         * @param playerID a size_t containing the client's playerID
         * 
        */
        Client(boost::asio::ip::udp::endpoint endpoint/*, std::string username*/, size_t playerID);
        ~Client();

        // Getters

        /**
         * @brief Get the Endpoint variable
         * 
         * @return boost::asio::ip::udp::endpoint a boost::asio::ip::udp::endpoint containing the client's IP address
        */
        boost::asio::ip::udp::endpoint getEndpoint() const;

        /**
         * @brief Get the Username variable
         * 
         * @return std::string a string containing the client's username
        */
        std::string getUsername() const;

        /**
         * @brief Get the State variable
         * 
         * @return ClientState a ClientState enum containing the client's state
        */
        ClientState getState() const;

        /**
         * @brief Get the PlayerID variable
         * 
         * @return size_t a size_t containing the client's playerID
        */
        size_t getPlayerID() const;

        /**
         * @brief Get the IsReady variable
         * 
         * @return bool containing the client's ready state
        */
        bool getIsReady() const;

        /**
         * @brief Get the IsAlive variable
         * 
         * @return bool containing the client's alive state
        */
        bool getIsAlive() const;

        /**
         * @brief Get the Positions variable
         * 
         * @return std::pair<float, float> containing the client's positions
        */
        std::pair<float, float> getPositions() const;

        /**
         * @brief Get the MoveDirection variable
         * 
         * @return MoveDirection enum containing the client's move direction
        */
        MoveDirection getMoveDirection() const;

        PlayerData getPlayerData(void) const;

        bool getIsPlaying(void) const;

        /**
         * @brief Get the Timestamp Ping object
         * 
         * @return long long: The value of the timestampPing variable
         */
        long long getTimestampPing(void) const;

        /**
         * @brief Get the Latency variable
         * 
         * @return int: The value of the latency between the client and the server
        */
        int getLatency(void) const;

        /**
         * @brief Get the Averagelatency object
         * 
         * @return int: the average latency of the client (calculated with the last 10 pings)
         */
        int getAveragelatency(void) const;

        /**
         * @brief Get the Max Latency object
         * 
         * @return int: the max latency of the client (calculated with all the ping received)
         */
        int getMaxLatency(void) const;

        /**
         * @brief Get the Latency std Deviation object
         * 
         * @return int: the standard deviation of the latency of the client (calculated with the last 10 pings)
         */
        int getLatencystdDeviation(void) const;

        /**
         * @brief Get the Speed object
         * 
         * @return int: the current speed of the client
         */
        int getSpeed(void) const;

        // Setters

        /**
         * @brief Set the Endpoint variable
         * 
         * @param endpoint a boost::asio::ip::udp::endpoint containing the client's IP address
        */
        void setEndpoint(boost::asio::ip::udp::endpoint endpoint);

        /**
         * @brief Set the Username variable
         * 
         * @param username a std::string containing the client's username
        */
        void setUsername(std::string username);

        std::string getUsername(void);

        /**
         * @brief Set the State variable
         * 
         * @param state a ClientState enum containing the client's state
        */
        void setState(ClientState state);

        /**
         * @brief Set the PlayerID variable
         * 
         * @param playerID a size_t containing the client's playerID
        */
        void setPlayerID(size_t playerID);

        /**
         * @brief Set the IsReady variable
         * 
         * @param isReady a bool containing the client's ready state
        */
        void setIsReady(bool isReady);

        /**
         * @brief Set the IsAlive variable
         * 
         * @param isAlive a bool containing the client's alive state
        */
        void setIsAlive(bool isAlive);

        /**
         * @brief Set the Positions variable
         * 
         * @param positions a std::pair<float, float> containing the client's positions
        */
        void setPositions(std::pair<float, float> positions);

        /**
         * @brief Set the MoveDirection variable
         * 
         * @param moveDirection a MoveDirection enum containing the client's move direction
        */
        void setMoveDirection(MoveDirection moveDirection);

        /**
         * @brief Set the IsPlaying variable
         * 
         * @param isPLaying: The new state (true or false) of the client
         */
        void setIsPlaying(bool isPlaying);

        /**
         * @brief Set the Timestamp Ping object (Can say that we reset the timestaamp to now)
         * 
        */
        void setTimestampPing(void);

        /**
         * @brief Update the latency of the client in milliseconds (Ping)
         * 
        */
        void updateLatency(void);

        /**
         * @brief Set the Speed object
         * 
         * @param newSpeed: the new value of the client's speed
         */
        void setSpeed(int newSpeed);

        void setColors(std::array<int, 3> colors);

        std::array<int, 3> getColors(void);

    private:
        // Network
        boost::asio::ip::udp::endpoint _endpoint;
        long long _timestampPing;
        int _latency;
        int _latencyMax;
        std::vector<int> _latencyList;
        std::array<int, 3> _colors;
        int _latencyAverage;
        int _latencyVariance;
        int _latencystdDeviation;

        // Client info
        std::string _username;
        ClientState _state;
        size_t _playerID;

        bool _isReady;
        bool _isAlive;
        bool _isPlaying;

        std::pair<float, float> _positions;
        MoveDirection _moveDirection;
        int _speed;
};