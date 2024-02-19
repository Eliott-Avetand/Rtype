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
//  File:            main.cpp                                                        //
//  Description:     This is the main file of the project.                           //
//                                                                                   //
// ********************************************************************************* //

#ifndef UDPCLIENT_HPP
    #define UDPCLIENT_HPP

    #include <boost/array.hpp>
    #include <boost/asio.hpp>

    #include <iostream>
    #include <cstddef>
    #include <bitset>
    #include <map>
    #include <thread>
    #include <mutex>

    #include "DataStruct.hpp"

    /**
     * @brief This class is used to communicate with the server
    */
    class UDPClient
    {
        public:
            /**
             * @brief Construct a new UDPClient object
             * 
             * @param host a string containing IP address on which the client will connect
             * @param port a string containing the port on which the client will connect
            */
            UDPClient(const std::string& host, const std::string& port);

            ~UDPClient();

            /**
             * @brief Setter for the the host
             * 
             * @param host a string containing IP address on which the client will connect
            */
            void setServerHost(const std::string& host);

            /**
             * @brief Setter for the the port
             * 
             * @param port a string containing the port on which the client will connect
            */
            void setServerPort(const unsigned short& port);

            /**
             * @brief Setter for the the port
             * 
             * @param port a string containing the port on which the client will connect
            */
            std::string getServerHost(void);

            /**
             * @brief Setter for the online status
             *
             * @param boolean which contain if the client is online or not
            */
            void setIsOnline(const bool isOnline);

            /**
             * @brief Getter for the online status
             *
             * @return boolean which contain if the client is online or not
            */
            bool getIsOnline(void) const;

            /**
             * @brief Setter for the the port
             * 
             * @param port a string containing the port on which the client will connect
            */
            unsigned short getServerPort(void);

            /**
             * @brief This function is a getter for the room list
             * 
             * @return std::vector<RoomData> a vector containing the room list
            */
            std::vector<RoomInfo> getRoomList(void) const;

            /**
             * @brief This function is a getter for the operation succedeed
             * 
             * @return true if the operation succedeed
             * @return false if the operation failed
            */
            bool getOperationSuccedeed(void) const;

            boost::asio::ip::udp::socket& getSocket(void);
            boost::asio::ip::udp::endpoint& getRoomEndpoint(void);

            void resetOperationSuccedeed(void);

            /**
             * @brief This function is a setter for the operation succedeed
             * 
             * @param pseudo a string containing the pseudo of the client
             * 
            */
            void setPseudo(const std::string& pseudo);

            /**
             * @brief This function is a getter for the pseudo
             * 
             * @return std::string a string containing the pseudo of the client
            */
            std::string getPseudo(void) const;


            /**
             * @brief Setter for the the port
             * 
             * @param port a string containing the port on which the client will connect
            */
            void setRoomPort(unsigned short port);

            /**
             * @brief Setter for the room network
             * 
             * @details This function will set the room network with the data received from the server
            */
            void setRoomNetwork(void);

            /**
             * @brief Get the Latency object
             * 
             * @return int: the latency in milliseconds between the client and the server
             */
            int getLatency(void) const;

            /**
             * @brief Set the Latency object
             * 
             * @param latency: The latency get by the server in millseconds
             */
            void setLatency(int latency);

            /**
             * @brief Answer to the server's ping
             *
             * @details This function will permit the server to know the ping of the client
            */
            void awnserPong(void);

            /**
             * @brief This function resets the server endpoint
             * 
             * @details This function will reset the server endpoint with the new ip address
             *          Because we DON'T KNOW WHY THE ENDPOINT IS CONSUMED AFTER A SEND
            */
            void resetServerEndpoint(void);

            /**
             * @brief Ask the server to create a room
             * 
             * @details This function will create the DataStruct with the CREATE command
             *          The function will block until the timer is expired or the server send a response
             *          It will then fill the structure DataStruct with the datas received from the server
             * @return true if the operation succedeed. False otherwise
            */
            bool askCreate(void);

            /**
             * @brief Ask the server to leave a room
             * 
             * @details This function will create the DataStruct with the LEAVE command
             *          The function will block until the timer is expired or the server send a response
             *          It will then fill the structure DataStruct with the datas received from the server
            */
            void askLeave(void);

            /**
             * @brief Ask the server to join a room
             * 
             * @details This function will create the DataStruct with the JOIN command
             *          The function will block until the timer is expired or the server send a response
             *          It will then fill the structure DataStruct with the datas received from the server
             * 
             * @param pseudo a string containing the pseudo of the player
             * @param roomID a string containing the ID of the room
             * 
            */
            void askJoin(std::string pseudo, std::string roomID);
            void askJoin(void);

            /**
             * @brief Ask the server to list the rooms
             * 
             * @details This function will create the DataStruct with the LIST command
             *          The function will block until the timer is expired or the server send a response
             *          It will then fill the structure DataStruct with the datas received from the server
            */
            void askList(void);

            /**
             * @brief Ask the server the name of the player
             * 
             * @details This function will create the DataStruct with the NAME command
             *         The function will block until the timer is expired or the server send a response
             *        It will then fill the structure DataStruct with the datas received from the server
            */
            void askName(void);

            std::string askPlayerName(int playerID, int askTry);

            /**
             * @brief Ask the server to start the game
             * 
             * @details This function will create the DataStruct with the START command
             *          The function will block until the timer is expired or the server send a response
             *          It will then fill the structure DataStruct with the datas received from the server
            */
            void askStart(void);

            /**
             * @brief Ask the server to stop the game
             * 
             * @details This function will create the DataStruct with the STOP command
             *          The function will block until the timer is expired or the server send a response
             *          It will then fill the structure DataStruct with the datas received from the server
            */
            void askStop(void);

            /**
             * @brief Ask the server to pause the game
             * 
             * @details This function will create the DataStruct with the PAUSE command
             *          The function will block until the timer is expired or the server send a response
             *          It will then fill the structure DataStruct with the datas received from the server
            */
            void askPause(void);

            /**
             * @brief Ask the server to resume the game
             * 
             * @details This function will create the DataStruct with the RESUME command
             *          The function will block until the timer is expired or the server send a response
             *          It will then fill the structure DataStruct with the datas received from the server
            */
            void askResume(void);

            /**
             * @brief Ask the server to shoot from a player
             * 
             * @details This function will create the DataStruct with the MOVE command
             *          The function will block until the timer is expired or the server send a response
             *          It will then fill the structure DataStruct with the datas received from the server
            */
            void askShoot(float x, float y);

            /**
             * @brief Ask the server to move a player
             * 
             * @details This function will create the DataStruct with the MOVE command
             *          The function will block until the timer is expired or the server send a response
             *          It will then fill the structure DataStruct with the datas received from the server
            */
            void askMove(float x, float y, MoveDirection moveDirection);

            /**
             * @brief Ask the server to die
             * 
             * @details This function will create the DataStruct with the DIE command
             *          The function will block until the timer is expired or the server send a response
             *          It will then fill the structure DataStruct with the datas received from the server
            */
            void askDie(void);

            /**
             * @brief Ask the player's count to the server
             *
             * @details This function will ask to the server the player counts
            */
            int askPlayerCount(int askTry);

            /**
             * @brief Ask the player's ID to the server
             *
             * @details This function will ask to the server the player's ID
            */
            int askPlayerId(int askTry);

            /**
             * @brief Ask the player's status in game to the server
             *
             * @details This function will ask to the server the player's status in game
            */
            int askPlayerIsPlaying(int askTry);

            /**
             * @brief Ask the player's status in game to the server
             *
             * @details This function will ask to the server the player's status in game
            */
            int askPlayerIsLoading(int askTry);

            /**
             * @brief Ask all breed's status in game to the server
             *
             * @details This function will ask to the server all breeds in game
             *
             * @return An array of all Breeds
             */
            BreedData askBreeds(int breedIndex, int askTry);

            /**
             * @brief Ask all breed's status in game to the server
             *
             * @details This function will ask to the server all breeds in game
             *
             * @return An array of all Breeds
             */
            int askBreedsNumbers(int askTry);

            int askEnemyNumbers(int askTry);

            std::array<int, 3> askGetPlayerColor(int playerID, int askTry);
            int askSetPlayerColor(std::array<int, 3> colors, int askTry);
            //void sendPlayerColor(int red, int green, int blue, int askTry);

        private:
            DataStruct _data;
            boost::asio::io_context _ioContext;
            boost::asio::ip::udp::socket _socket;
            boost::asio::ip::udp::endpoint _endpoint;
            std::string _pseudo;
            bool _operationSuccedeed;
            bool _isOnline;

            int _latency;

            // SERVER
            std::string _serverHost;
            unsigned short _serverPort;
            boost::asio::ip::udp::endpoint _serverEndpoint;

            // ROOM
            boost::asio::ip::udp::endpoint _roomEndpoint;
            unsigned short _roomPort;

            std::vector<RoomInfo> _roomList;
    };

    /**
     * @brief This type is used to handle the commands received from the client.
    */
    typedef void (UDPClient::*askCommand)(void);

#endif /* UDPCLIENT_HPP */