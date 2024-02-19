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
//  File:            Server.cpp                                                      //
//  Description:     This is the Server main cpp file which contains all             //
//					 functions for the server to init and start                      //
//                                                                                   //
// ********************************************************************************* //

#include "../include/UDPServer.hpp"
#include "Engine.hpp"

UDPServer::UDPServer()
    :	_port(0),
        _localEndpoint(),
        _socket(this->_io_context) // create the socket
{
    std::cout << "[SERVER] Server is starting..." << std::endl;
    srand(time(NULL));
    this->_hostName = boost::asio::ip::host_name(); // Get the name of the machine (hostname)
    this->_localEndpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), this->_port); // create the endpoint (ip address and port)

    this->_socket.open(boost::asio::ip::udp::v4());
    this->_socket.bind(this->_localEndpoint);
    this->_localEndpoint = this->_socket.local_endpoint();

    this->debug();
    this->_checkingRooms = true;
    this->_checkRoomsThread = std::thread(&UDPServer::checkRooms, this);
    std::cout << "[SERVER] Server is ready to receive data" << std::endl;

    // this->startReceive(); // TODO Decider si on le met ici ou dans le run()
}

UDPServer::~UDPServer()
{
}

void UDPServer::run(void)
{
    this->startReceive(); // TODO Je crois qu'on est obligé de le mettre avant le run() du contexte sinon ça marche pas
}

void UDPServer::debug(void)
{
    // get the ip address of the server and prints it
    std::cout << "[DEBUG] Host Name:\t\t" << this->_hostName << std::endl;

    std::cout << "[DEBUG] Server listens to IP:\t" << this->_localEndpoint.address() << std::endl;
    //
    boost::asio::ip::udp::resolver resolver(this->_io_context);
    boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), this->_hostName, std::to_string(this->_localEndpoint.port()));
    boost::asio::ip::udp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::ip::udp::resolver::iterator end;
    boost::asio::ip::udp::endpoint endpoint = *iter;

    // Parcoure toutes les adresses IP possibles
    while (iter != end) {
        boost::asio::ip::address addr = iter->endpoint().address();

        if (addr.is_v4() && !addr.is_loopback()) {
            std::cout << "[DEBUG] Correct IP:\t\t" << addr.to_string() << std::endl;
            std::cout << "[DEBUG] Server is listening on:\t" << addr.to_string() << ':' << this->_localEndpoint.port() << std::endl;
            break;
        }
        iter++;
    }
    
    std::cout << "[DEBUG] Server Port:\t\t" << this->_localEndpoint.port() << std::endl;
}

boost::asio::ip::udp::endpoint UDPServer::getEndpointFromHostName(std::string hostName, size_t port)
{
    boost::asio::ip::udp::resolver resolver(this->_io_context);
    boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), this->_hostName, std::to_string(port));
    boost::asio::ip::udp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::ip::udp::resolver::iterator end;
     boost::asio::ip::udp::endpoint endpoint = *iter;

    while (iter != end)
    {
        boost::asio::ip::address addr = iter->endpoint().address();
        // std::cout << "[SERVER] IP: " << addr.to_string() << std::endl;
        if (addr.is_v4() && !addr.is_loopback()) {
            // std::cout << "[SERVER] Possible IP: " << addr.to_string() << std::endl;
            break; // TODO A voir si on break ou pas (si on break on prend que la première adresse IP)
        }
        iter++;
    }

    return endpoint;
}

void UDPServer::joinThreads(void)
{

    if (this->_rooms.size() <= 0) {
        std::cout << "[SERVER] No room to close" << std::endl;
        this->joinThreads();
    }

    std::mutex mutex;
    mutex.lock();
    for (auto & room : this->_rooms) {
        if (room.get()->getState() == CLOSED) {
            std::cout << "[SERVER] Closing Room" << room.get()->getID() << ". . ." << std::endl;
            room.get()->closeRoom();
            this->_rooms.erase(std::remove(this->_rooms.begin(), this->_rooms.end(), room), this->_rooms.end());
        }
    }
    mutex.unlock();
    this->joinThreads();
}

void UDPServer::startReceive(void)
{
    try {
        std::mutex m;
        m.lock();
        this->_io_context.reset();
    
        auto binded_function = boost::bind(&UDPServer::handleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
        this->_socket.async_receive_from(boost::asio::buffer(&this->_dataStruct, sizeof(DataStruct)), this->_remoteEndpoint, binded_function);

        this->_io_context.run();
        m.unlock();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

size_t UDPServer::getPortFromString(std::string str)
{
    std::string strPort = "";

    // TODO when the protocol will be defined

    size_t port = std::stoi(strPort);
    return port;
}

void UDPServer::CommandManager(boost::asio::ip::udp::endpoint remoteEndpoint)
{
    std::mutex mutex;

    mutex.lock();

    std::map<CommandCode, CommandHandler> myMap;
        myMap[CREATE] = &UDPServer::createRoom;
        myMap[JOIN] = &UDPServer::joinRoom;
        myMap[LIST] = &UDPServer::listRoom;

    for (const auto& item : myMap) {
        if (item.first == this->_dataStruct.command) {
            CommandHandler handler = item.second;
            (this->*handler)(remoteEndpoint);
            break;
        }
    }
    mutex.unlock();
}

void UDPServer::createRoom(boost::asio::ip::udp::endpoint clientEndpoint)
{
    

    try {
        
        std::mutex m;

        m.lock();

        DataStruct data = {
            .command = KO,
        };
        if (this->_rooms.size() >= 9) {
            size_t byteSend = this->_socket.send_to(boost::asio::buffer(&data, sizeof(DataStruct)), clientEndpoint);
            std::cout << "[DEBUG] " << byteSend << " bytes send" << std::endl;
            m.unlock();
            return;
        }
        std::string roomID = this->generateID(5);

        this->_rooms.push_back(std::make_shared<Room>(this->getEndpointFromHostName(this->_hostName, 0), roomID, clientEndpoint));
        std::cout << "[SERVER] New room created" << std::endl;
        for (int i = 0; i < this->_rooms.size(); i++) {
            if (this->_rooms[i].get()->getID() == roomID)  {
                this->_rooms[i].get()->printInfo();
            }
        }

        m.unlock();

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        this->_rooms.end()->get()->closeRoom();
    }

    // this->startReceive(); // TODO voir si on met le startReceive ici ou dans le handleReceive
}

void UDPServer::listRoom(boost::asio::ip::udp::endpoint clientEndpoint)
{
    std::cout << "[SERVER] List Room" << std::endl;
    //////////////////////////////////////////////////////////////////////////

    std::mutex m;

    m.lock();

    DataStruct data = {
        .command = OK,
        .roomCount = static_cast<short>(this->_rooms.size()),
    };

    std::string roomName = "";
    for (int i = 0; i != this->_rooms.size(); i++) {
        RoomInfo room = {
            .playerCount = static_cast<char>(this->_rooms[i].get()->getPlayerCount()),
        };
        roomName = "ROOM " + std::to_string(i + 1);
        room.roomNameLength = roomName.length();

        for (int j = 0; j != roomName.length(); j++) {
            room.roomName[j] = roomName[j];
        }
        data.rooms[i] = room;
    }
    //////////////////////////////////////////////////////////////////////////

    size_t byteSend = this->_socket.send_to(boost::asio::buffer(&data, sizeof(DataStruct)), clientEndpoint);
    std::cout << "[DEBUG] " << byteSend << " bytes send" << std::endl;
    m.unlock();
}

void UDPServer::joinRoom(boost::asio::ip::udp::endpoint remoteEndpoint)
{
    std::cout << "[DEBUG] In joinRoom" << std::endl;

    std::mutex m;

    m.lock();
    std::string roomName = this->_dataStruct.currentRoom.password;

    boost::system::error_code error;

    for (auto &room : this->_rooms) {
        if (room.get()->getID() == roomName) {

            std::cout << "[DEBUG] Find Room, can connect client" << std::endl;

            if (room->getPlayerCount() >= 4) {
                std::cout << "[DEBUG] Room is full" << std::endl;
                DataStruct data = {
                    .command = KO,
                };
                size_t byteSend = this->_socket.send_to(boost::asio::buffer(&data, sizeof(data)), remoteEndpoint);
                std::cout << "[DEBUG] " << byteSend << " bytes send" << std::endl;
                m.unlock();
                return;
            }

            if (room->getState() == RoomState::PLAYING) {
                std::cout << "[DEBUG] Room is ingame" << std::endl;
                DataStruct data = {
                    .command = KO,
                };
                size_t byteSend = this->_socket.send_to(boost::asio::buffer(&data, sizeof(data)), remoteEndpoint);
                std::cout << "[DEBUG] " << byteSend << " bytes send" << std::endl;
                m.unlock();
                return;
            }

            unsigned short p = room.get()->getRoomEndpoint().port();

            DataStruct data = {
                .command = OK,
                .port = p,
            };

            data.currentRoom.playerCount = room.get()->getPlayerCount();

            room->addClient(new Client(remoteEndpoint, room->getPlayerCount()));
            size_t byteSend = this->_socket.send_to(boost::asio::buffer(&data, sizeof(data)), remoteEndpoint);
            std::cout << "[DEBUG] " << byteSend << " bytes send" << std::endl;
            m.unlock();
            return;
        }
    }

    // Send user Failed to join room
    std::cout << "[DEBUG] Room not found" << std::endl;

    DataStruct data = {
        .command = KO,
    };

    size_t byteSend = this->_socket.send_to(boost::asio::buffer(&data, sizeof(data)), remoteEndpoint);
    std::cout << "[DEBUG] " << byteSend << " bytes send" << std::endl;
    m.unlock();
}

void UDPServer::handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    try {
        if (!error || error == boost::asio::error::message_size) {
        std::cout << "[SERVER] Received " << bytes_transferred << " bytes from " << this->_remoteEndpoint.address().to_string() << std::endl;
        if (bytes_transferred > 0)
            this->CommandManager(this->_remoteEndpoint);
        this->startReceive();
        } else {
            std::cout << "[ERROR]\t" << error.message() << std::endl;
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

void UDPServer::handleSend(std::shared_ptr<std::string> message, const boost::system::error_code& error, std::size_t bytes_transferred)
{
    startReceive();
}

std::string UDPServer::generateID(size_t length)
{
    std::string id = "";
    std::string possibleChars = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";

    for (size_t i = 0; i < length; i++)
        id += possibleChars[rand() % possibleChars.length()];

    return id;
}

void UDPServer::closeServer(void)
{
    std::mutex mutex;

    DataStruct closingStruct = {
        .command = CommandCode::CLOSING,
    };

    this->_checkingRooms = false;

    mutex.lock();

    for (auto &room : this->_rooms) {
        room.get()->sendDataToClients(closingStruct);
        room.get()->closeRoom();
    }
    this->_rooms.clear();

    mutex.unlock();

    this->_checkRoomsThread.join();

    std::cout << "[SERVER] Closing socket" << std::endl;
    if (this->_socket.is_open())
        this->_socket.close();
}

void UDPServer::signalHandler(int signum)
{
    std::cout << "[SERVER] Closing server" << std::endl;
    this->closeServer();
    std::cout << "[SERVER] ************ Server closed ************" << std::endl;
    exit(EXIT_SUCCESS);
}

boost::asio::ip::udp::endpoint UDPServer::getLocalEndpoint(void)
{
    return this->_localEndpoint;
}

boost::asio::ip::udp::endpoint UDPServer::getRemoteEndpoint(void)
{
    return this->_remoteEndpoint;
}

void UDPServer::checkRooms(void)
{

    std::mutex m;

    std::this_thread::sleep_for(std::chrono::milliseconds(4000));

    while (this->_checkingRooms) {

        if (m.try_lock()) {

            #ifdef WIN32
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            #endif

            for (int i = 0; i < this->_rooms.size(); i++) {

                if (this->_rooms[i]->getState() == CLOSED) {

                    this->_rooms[i]->closeRoom();
                    std::cout << "[DEBUG] Erasing Room index: " << i << std::endl;
                    this->_rooms.erase(this->_rooms.begin() + i);
                    std::cout << "[DEBUG] " << this->_rooms.size() << "Remaining" << std::endl;
                    break;
                }
            }

            m.unlock();
        }
    }
    std::cout << "[SERVER] Thread finished" << std::endl;
}