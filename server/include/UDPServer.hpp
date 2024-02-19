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
//  File:            Server.hpp                                                      //
//  Description:     This is the header of the "Server.cpp" file                     //
//                                                                                   //
// ********************************************************************************* //

#include <string>
#include <iostream>
#include <signal.h>
#include <mutex>
#include <map>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/random.hpp>

#include "Room.hpp"
#include "DataStruct.hpp"

using namespace boost::placeholders;

#ifndef UDPSERVER_HPP_
    #define UDPSERVER_HPP_

    /**
     * @class UDPServer class 
     * @brief This class is used to create a UDP server, and to handle the communication with the client.
    */
    class UDPServer
    {
        public:

            /**
             * @brief This is the constructor of the class.
             * @param void
             * @return void
             * 
            */
            UDPServer();
            ~UDPServer();

            /**
             * @brief This function is used to run the server.
             * @param void
             * @return void
            */
            void run(void);

            /**
             * @brief This function is used to debug the server.
             * @details It will print the port and the IP address used by the server.
             * @param void
             * @return void
            */
            void debug(void);

            /**
             * @brief This function create the endpoint from the host Name given.
             * @param hostName : The host name of the server / machine
             * @param port : The port of the server / machine
             * @return endpoint : Return the endpoint from the HostName
            */
            boost::asio::ip::udp::endpoint getEndpointFromHostName(std::string hostName, size_t port);

            /**
             * @brief This function is used to start the reception of data from the client.
             * @param void
             * @return void
            */
            void startReceive(void);

            /**
             * @brief this function is used to send data to the client asynchronously.
             * @param error : This is the boost::system::error_code used to handle errors.
             * @param bytes_transferred : This is the number of bytes sent to the client.
            */
            void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);

            /**
             * @brief This function is used when the data has been sent to the client.
             * @details It is normally called by the handleReceive function.
             * 			It will ensure that the data has been sent to the client.
             * @param message : This is the message to send to the client.
             * @param error : This is the boost::system::error_code used to handle errors.
             * @param bytes_transferred : This is the number of bytes sent to the client.
            */
            void handleSend(std::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes_transferred);
            

            /**
             * @brief This function is used to generate an ID with a specific length.
             * @param length : This is the length of the ID you want.
             * @return std::string : Return the ID generated.
            */
            static std::string generateID(size_t length);

            /**
             * @brief This function is used to close the server.
             * @param void
             * @return void
            */
            void closeServer(void);

            /**
             * @brief This function is used to handle the signal SIGINT (ctrl + c).
             * @param void
             * @return void
            */
            void signalHandler(int signum);

            /**
             * @brief This function is used to get the local endpoint of the server.
             * @param void
             * @return boost::asio::ip::udp::endpoint : Return the local endpoint of the server.
            */
            boost::asio::ip::udp::endpoint getLocalEndpoint(void);

            /**
             * @brief This function is used to get the remote endpoint of the server.
             * @param void
             * @return boost::asio::ip::udp::endpoint : Return the remote endpoint of the server.
            */
            boost::asio::ip::udp::endpoint getRemoteEndpoint(void);

            /**
            * @brief This function is used to manage the bytes received from the client and check which command was send.
            *		  Then launch the function associate to the command.
            * @param data : This is the data received from the client.
            * @param remoteEndpoint : This is the endpoint of the client.
            * @return void
            */
            void CommandManager(boost::asio::ip::udp::endpoint remoteEndpoint);

            /**
             * @brief This function is used to create a room. The room is create in a Thread.
             * @param remoteEndpoint : This is the endpoint of the client.
             * @return void
            */
            void createRoom(boost::asio::ip::udp::endpoint remoteEndpoint);
            
            /**
             * @brief This function is used to join a room. The room is join in a Thread.
             * @param remoteEndpoint : This is the endpoint of the client.
             * @return bool : Return true if the room has been joined, false otherwise.
            */
            void joinRoom(boost::asio::ip::udp::endpoint remoteEndpoint);

            /**
             * @brief This function is used to list all the rooms available.
             * @param remoteEndpoint : This is the endpoint of the client.
             * @return void
            */
            void listRoom(boost::asio::ip::udp::endpoint remoteEndpoint);

        private:
            // Methods

            /**
             * @brief This function is used to get the Port from a string.
             * @param str : This is the string to get the Port from.
             * @return std::string : Return the Port from the string.
            */
            size_t getPortFromString(std::string str);

            /**
             * @brief This function is used to join the thread that are inactif.
             * @return void
            */
            void joinThreads(void);

            /**
             * @brief This function is used to check if the rooms are still active.
             * @return void
            */
            void checkRooms(void);

            DataStruct _dataStruct;

            // Variables
            int _port;
            boost::asio::io_context _io_context;
            boost::asio::ip::udp::socket _socket;
            boost::asio::ip::udp::endpoint _remoteEndpoint;
            boost::asio::ip::udp::endpoint _localEndpoint;
            std::string _hostName;
            size_t _threadCount;
            std::vector<std::shared_ptr<Room>> _rooms;
            std::thread _checkRoomsThread;
            bool _checkingRooms;
    };

    /**
     * @brief This type is used to handle the commands received from the client.
    */
    typedef void (UDPServer::*CommandHandler)(boost::asio::ip::udp::endpoint);

#endif /* !UDPSERVER_HPP_ */
