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

#include "../include/UDPClient.hpp"

UDPClient::UDPClient(const std::string& host, const std::string& port)
    : _ioContext(), _socket(_ioContext, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0))
{
    // std::cout << "Creating endpoint with | HOST: " << host << " | PORT: " << port << std::endl;
    try {
        this->_serverHost = host;
        this->_serverPort = std::stoi(port);
        boost::asio::ip::udp::endpoint serverEndpoint(boost::asio::ip::address::from_string(host), std::stoi(port));
        this->_serverEndpoint = serverEndpoint;
        this->_roomPort = 0;
        this->_endpoint = this->_socket.local_endpoint();
        this->_isOnline = false;
        std::cout << "UDPClient created" << std::endl;
    } catch(const std::exception& e) {

    }
}

UDPClient::~UDPClient()
{
    _socket.close();
}

void UDPClient::setIsOnline(const bool isOnline)
{
    this->_isOnline = isOnline;
}

void UDPClient::setServerHost(const std::string& host)
{ this->_serverHost = host; }

void UDPClient::setServerPort(const unsigned short& port)
{ this->_serverPort = port; }

void UDPClient::setPseudo(const std::string& pseudo)
{ this->_pseudo = pseudo; }

bool UDPClient::getIsOnline(void) const
{
    return this->_isOnline;
}

std::string UDPClient::getPseudo(void) const
{ return this->_pseudo; }

std::string UDPClient::getServerHost(void)
{ return this->_serverHost; }

unsigned short UDPClient::getServerPort(void)
{ return this->_serverPort; }

std::vector<RoomInfo> UDPClient::getRoomList(void) const
{ return this->_roomList; }

bool UDPClient::getOperationSuccedeed(void) const
{ return this->_operationSuccedeed; }

boost::asio::ip::udp::socket& UDPClient::getSocket(void)
{ return this->_socket; }

boost::asio::ip::udp::endpoint& UDPClient::getRoomEndpoint(void)
{ return this->_serverEndpoint; }

int UDPClient::getLatency(void) const
{ return this->_latency; }

void UDPClient::setLatency(int latency)
{
    this->_latency = latency;
}

void UDPClient::setRoomPort(unsigned short port)
{
    std::cout << "OLD Room port: " << this->_roomPort << std::endl;
    this->_roomPort = port;
    std::cout << "NEW Room port: " << this->_roomPort << std::endl;
}

void UDPClient::setRoomNetwork(void)
{
    boost::asio::ip::udp::endpoint roomEndpoint(this->_serverEndpoint.address(), this->_roomPort);
    this->_roomEndpoint = roomEndpoint;
}

bool UDPClient::askCreate(void)
{
    std::cout << "[CLIENT] Dans la fonction askCreate" << std::endl;

    DataStruct dataSend = {
        .command = CREATE,
    };
    DataStruct dataRecv;

    try {
        this->_ioContext.reset();
        std::mutex mutex;

        mutex.lock();
        std::cout << "Trying to create a room" << std::endl;
        size_t byteSend = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_serverEndpoint);
        std::cout << "[DEBUG] byteSend: " << byteSend << std::endl;

        std::cout << "Launching timer" << std::endl;
        boost::asio::deadline_timer timer(this->_ioContext);
        timer.expires_from_now(boost::posix_time::seconds(10));
        timer.async_wait([&](boost::system::error_code ec) {
            if (!ec)
                this->_socket.cancel();
        });


        std::cout << "Waiting for response..." << std::endl;
        std::cout << "[Attente async] dans askCreate" << std::endl;
        this->_socket.async_receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_serverEndpoint,
        
        [this, &dataRecv, &mutex, &timer](boost::system::error_code ec, std::size_t bytes_transferred) {
            std::cout << "[Réponse async] dans askCreate" << std::endl;
            if (!ec) {
                if (dataRecv.command == OK) {
                    timer.cancel();
                    this->setRoomPort(dataRecv.currentRoom.port);
                    this->setRoomNetwork();
                    this->_operationSuccedeed = true;
                    return true;
                } else {
                    // CAN NOT CREATE ROOM | ROOM'S COUNTER IS FULL
                    timer.cancel();
                    this->_operationSuccedeed = false;
                    std::cout << "Can not create room" << std::endl;
                    return false;
                }
            } else if (ec == boost::asio::error::operation_aborted) {
                this->_operationSuccedeed = false;
                std::cout << "Timeout" << std::endl;
                return false;
            } else {
                timer.cancel();
                this->_operationSuccedeed = false;
                std::cout << "Error: " << ec.message() << std::endl;
                return false;
            }
            mutex.unlock();
        });
        this->_ioContext.run();
        return false;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return false;
}

void UDPClient::askJoin(void)
{
    std::cout << "[CLIENT] Dans la fonction askJoin" << std::endl;
}

void UDPClient::askJoin(std::string pseudonyme, std::string roomID)
{
    /////////////////////////////////////////////////
    DataStruct dataSend = {
        .command = JOIN,
    };
    DataStruct dataRecv;
    
    dataSend.currentRoom.roomNameLength = roomID.length();
    for (int i = 0; i < roomID.length(); i++) {
        dataSend.currentRoom.roomName[i] = roomID[i];
        dataSend.currentRoom.password[i] = roomID[i];
    }

    // DEBUG
    for (int i = 0; i != dataSend.currentRoom.roomNameLength; i++)
        std::cout << dataSend.currentRoom.roomName[i];
    std::cout << std::endl;
    //

    /////////////////////////////////////////////////

    try {
        this->_ioContext.reset();
        std::mutex mutex;

        mutex.lock();
        std::cout << "Trying to join a room" << std::endl;
        size_t byteSend = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_serverEndpoint);
        std::cout << "[DEBUG] byteSend: " << byteSend << std::endl;

        std::cout << "Launching timer" << std::endl;
        boost::asio::deadline_timer timer(this->_ioContext);
        timer.expires_from_now(boost::posix_time::seconds(10));
        timer.async_wait([&](boost::system::error_code ec) {
            if (!ec)
                this->_socket.cancel();
        });

        std::cout << "Waiting for response..." << std::endl;
        std::cout << "[Attente async] dans askJoin" << std::endl;
        this->_socket.async_receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_serverEndpoint,
        
        [this, &dataRecv, pseudonyme, &mutex, &timer](boost::system::error_code ec, std::size_t bytes_transferred) {
            
            std::cout << "[Réponse async] dans askJoin" << std::endl;
            if (!ec) {
                if (dataRecv.command == OK) {
                    timer.cancel();
                    this->setRoomPort(dataRecv.port);
                    this->setRoomNetwork();
                    /////////////////////////////////////////////////
                    // DataStruct pseudo = {
                    //     .command = NAME,
                    //     .pseudoLength = static_cast<short>(pseudonyme.size()),
                    // };
                    // for (int i = 0; i < pseudonyme.size(); i++) {
                    //     pseudo.pseudo[i] = pseudonyme[i];
                    // }
                    // /////////////////////////////////////////////////
                    // size_t byteSend2 = this->_socket.send_to(boost::asio::buffer(&pseudo, sizeof(pseudo)), this->_roomEndpoint);
                    // std::cout << "[DEBUG] byteSend: " << byteSend2 << std::endl;
                    this->_operationSuccedeed = true;
                } else if (dataRecv.command == KO) {
                    // CAN NOT JOIN ROOM | ROOM DOESN'T EXIST
                    timer.cancel();
                    this->_operationSuccedeed = false;
                    std::cout << "Can not join room" << std::endl;
                    return;
                }
            } else if (ec == boost::asio::error::operation_aborted) {
                this->_operationSuccedeed = false;
                std::cout << "Timeout" << std::endl;
            } else {
                timer.cancel();
                this->_operationSuccedeed = false;
                std::cout << "Error: " << ec.message() << std::endl;
            }
            mutex.unlock();
        });
        this->_ioContext.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void UDPClient::askLeave(void)
{
    std::cout << "[CLIENT] Dans la fonction askLeave" << std::endl;

    DataStruct dataSend = {
        .command = LEAVE,
    };
    DataStruct dataRecv;

    try {
        this->_ioContext.reset();
        std::mutex mutex;

        mutex.lock();

        std::cout << "Trying to leave a room" << std::endl;
        size_t byteSend = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[DEBUG] byteSend: " << byteSend << std::endl;

        std::cout << "Launching timer" << std::endl;
        boost::asio::deadline_timer timer(this->_ioContext);
        timer.expires_from_now(boost::posix_time::seconds(10));
        timer.async_wait([&](boost::system::error_code ec) {
            if (!ec)
                this->_socket.cancel();
        });

        std::cout << "Waiting for response..." << std::endl;
        std::cout << "[Attente async] dans askLeave" << std::endl;
        this->_socket.async_receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint,
        [this, &dataRecv, &mutex, &timer](boost::system::error_code ec, std::size_t bytes_transferred) {
            std::cout << "[Réponse async] dans askLeave" << std::endl;
            std::cout << "Received: " << bytes_transferred << " bytes" << std::endl;
            if (!ec) {
                if (dataRecv.command == OK) {
                    timer.cancel();
                    this->setRoomPort(0);
                    this->setRoomNetwork();
                    this->_operationSuccedeed = true;
                } else {
                    timer.cancel();
                    this->_operationSuccedeed = false;
                    std::cout << "Can not leave room" << std::endl;
                }
            } else if (ec == boost::asio::error::operation_aborted) {
                std::cout << "Timeout" << std::endl;
                this->_operationSuccedeed = false;
            } else {
                this->_operationSuccedeed = false;
                std::cout << "Error: " << ec.message() << std::endl;
            }
            mutex.unlock();
        });
        this->_ioContext.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void UDPClient::askList(void)
{
    DataStruct dataSend = {
        .command = LIST,
    };
    DataStruct dataRecv;

    try {
        this->_ioContext.reset();
        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_serverEndpoint);
        boost::asio::deadline_timer timer(this->_ioContext);
        timer.expires_from_now(boost::posix_time::seconds(10));
        timer.async_wait([&](boost::system::error_code ec) {
            if (!ec) {
                this->_socket.cancel();
                std::cout << "Timeout" << std::endl;
            }
            });

        std::mutex mutex;
        mutex.lock();
        std::cout << "[Attente async] dans askList" << std::endl;
        this->_socket.async_receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_serverEndpoint,
            [this, &dataRecv, &mutex, &timer](boost::system::error_code ec, std::size_t bytes_transferred) {
            std::cout << "[Réponse async] dans askList" << std::endl;
                if (!ec) {
                    if (dataRecv.command == OK) {
                        timer.cancel();
                        this->_roomList.clear();
                        for (int i = 0; i != dataRecv.roomCount; i++) {
                            //if (dataRecv.rooms[i].port != 0) {
                                this->_roomList.push_back(dataRecv.rooms[i]);
                            //}
                        }
                        std::cout << "List of rooms: " << std::endl;
                        for (int i = 0; i != this->_roomList.size(); i++) {
                            std::cout << "Room " << i + 1 << ": " << this->_roomList[i].roomName << std::endl;
                        }
                        if (this->_roomList.size() > 1) {
                            std::cout << "There are " << this->_roomList.size() << " rooms" << std::endl;
                        }
                        else {
                            std::cout << "There is " << this->_roomList.size() << " room" << std::endl;
                        }
                        this->_operationSuccedeed = true;
                    }
                    else if (dataRecv.command == KO) {
                        std::cout << "Can not get list of rooms" << std::endl;
                        timer.cancel();
                        this->_operationSuccedeed = false;
                    }
                }
                else {
                    this->_operationSuccedeed = false;
                    std::cout << "Error: " << ec.message() << std::endl;
                }
            mutex.unlock();
            });
        this->_ioContext.run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void UDPClient::askName(void)
{
    std::cout << "[CLIENT] Dans la fonction askName" << std::endl;

    try {
        std::mutex mutex;

        mutex.lock();
        this->_ioContext.reset();
        /////////////////////////////////////////////////
        DataStruct dataSend = {
            .command = NAME,
            .pseudoLength = static_cast<short>(this->_pseudo.length()),
        };
        for (int i = 0; i != 32; i++)
            dataSend.pseudo[i] = this->_pseudo[i];
        /////////////////////////////////////////////////

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[CLIENT] " << byteSent << " bytes sent" << std::endl;
        
        // boost::asio::deadline_timer timer(this->_ioContext);
        // timer.expires_from_now(boost::posix_time::seconds(10));
        // timer.async_wait([&](boost::system::error_code ec) {
        //     if (!ec) {
        //         this->_socket.cancel();
        //         std::cout << "Timeout" << std::endl;
        //     }
        // });

        // DataStruct dataRecv;

        // this->_socket.async_receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint,
        // [this, &dataRecv, &mutex, &timer](boost::system::error_code ec, std::size_t bytes_transferred) {
        //     if (!ec) {
        //         if (dataRecv.command == OK) {
        //             timer.cancel();
        //         } else if (dataRecv.command == KO) {
        //             std::cout << "Can not set name" << std::endl;
        //             timer.cancel();
        //         }
        //     } else {
        //         std::cout << "Error: " << ec.message() << std::endl;
        //     }
        //     mutex.unlock();
        // });
        // this->_ioContext.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return;
}
void UDPClient::askStart(void)
{
    std::cout << "[CLIENT] Dans la fonction askStart" << std::endl;

    try {

        this->_operationSuccedeed = false;
        std::mutex mutex;

        mutex.lock();

        this->_ioContext.reset();

        /////////////////////////////////////////////////
        DataStruct dataSend = {
            .command = START,
        };
        /////////////////////////////////////////////////

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[CLIENT] " << byteSent << " bytes sent" << std::endl;

        /*boost::asio::deadline_timer timer(this->_ioContext);
        timer.expires_from_now(boost::posix_time::seconds(10));
        timer.async_wait([&](boost::system::error_code ec) {
            if (!ec) {
                this->_socket.cancel();
                std::cout << "Timeout" << std::endl;
            }
        });

        DataStruct dataRecv;

        this->_socket.async_receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint, 0,
        [this, &dataRecv, &mutex, &timer](boost::system::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                if (dataRecv.command == START) {
                    timer.cancel();
                    this->_operationSuccedeed = true;
                } else if (dataRecv.command == KO) {
                    std::cout << "Can not start game" << std::endl;
                    timer.cancel();
                }
            } else {
                std::cout << "Error: " << ec.message() << std::endl;
            }
            mutex.unlock();
        });*/

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return;
}
void UDPClient::askStop(void)
{
    std::cout << "[CLIENT] Dans la fonction askStop" << std::endl;
    return;
}
void UDPClient::askPause(void)
{
    std::cout << "[CLIENT] Dans la fonction askPause" << std::endl;
    return;
}
void UDPClient::askResume(void)
{
    std::cout << "[CLIENT] Dans la fonction askResume" << std::endl;
    return;
}

std::string UDPClient::askPlayerName(int playerID, int askTry)
{
    std::cout << "[CLIENT] Dans la fonction askPlayerName" << std::endl;

    try {
        if (askTry <= 0)
            return "??????";

        std::mutex m;

        m.lock();
        this->_socket.non_blocking(false);

        /////////////////////////////////////////
        DataStruct dataSend{
            .command = PLAYERNAME,
            .currentPlayer = {
                .playerID = playerID,
            }
        };
        /////////////////////////////////////////

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[CLIENT] " << byteSent << " bytes sent" << std::endl;

        DataStruct dataRecv;
        std::cout << "[Attente] dans askPlayerName" << std::endl;
        this->_socket.receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint);
        std::cout << "[Réponse] dans askPlayerName" << std::endl;

        if (dataRecv.command == OK) {
            this->_socket.non_blocking(true);
            m.unlock();
            return std::string(dataRecv.currentPlayer.pseudo);
        }
        else if (dataRecv.command == KO) {
            this->_socket.non_blocking(true);
            m.unlock();
            std::cout << "[CLIENT] KO" << std::endl;
            return this->askPlayerName(playerID, askTry - 1);
        }
        else {
            this->_socket.non_blocking(true);
            m.unlock();
            std::cout << "[CLIENT] Error UNKNOW Command" << std::endl;
            return this->askPlayerName(playerID, askTry - 1);
        }
    }
    catch (std::exception& e) {
        this->_socket.non_blocking(true);
        std::cerr << e.what() << std::endl;
    }
    return "??????";
}

void UDPClient::askShoot(float x, float y)
{
    try {
        std::mutex mutex;

        mutex.lock();

        std::pair<float, float> pos(x, y);

        DataStruct dataSend{
            .command = SHOOT,
            .currentPlayer {
                .positions = pos
            }
        };

        boost::system::error_code error;

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint, 0, error);

        if (error) {
            std::cout << "Error: " << error.message() << std::endl;
        }

        mutex.unlock();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
void UDPClient::askMove(float x, float y, MoveDirection moveDirection)
{
    try {
        std::mutex mutex;

        mutex.lock();

        std::pair<float, float> pos(x, y);

        DataStruct dataSend {
            .command = MOVE,
            .currentPlayer {
                .positions = pos,
                .moveDirection = moveDirection,
            }
        };

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        mutex.unlock();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void UDPClient::askDie(void)
{
    std::cout << "[CLIENT] Dans la fonction askDie" << std::endl;
    return;
}

int UDPClient::askPlayerCount(int askTry)
{
    std::cout << "[CLIENT] Dans la fonction askPlayerCount" << std::endl;

    try {

        if (askTry <= 0)
            return -1;

        std::mutex m;

        m.lock();
        this->_socket.non_blocking(false);

        /////////////////////////////////////////
        DataStruct dataSend{
            .command = PCOUNT,
        };
        /////////////////////////////////////////

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[CLIENT] " << byteSent << " bytes sent" << std::endl;

        DataStruct dataRecv;
        std::cout << "[Attente] dans askPlayerCount" << std::endl;
        this->_socket.receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint);
        std::cout << "[Réponse] dans askPlayerCount" << std::endl;

        if (dataRecv.command == OK) {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] " << (int)dataRecv.currentRoom.playerCount << " players in the room" << std::endl;
            m.unlock();
            return (int)dataRecv.currentRoom.playerCount;
        } else if (dataRecv.command == KO) {
            this->_socket.non_blocking(true);
            m.unlock();
            std::cout << "[CLIENT] KO" << std::endl;
            return this->askPlayerCount(askTry - 1);
        } else {
            this->_socket.non_blocking(true);
            m.unlock();
            std::cout << "[CLIENT] Error UNKNOW Command" << std::endl;
            std::cout << "COMMAND RECEIVE: " << dataRecv.command << std::endl;
            return this->askPlayerCount(askTry - 1);
        }
    } catch (std::exception& e) {
        this->_socket.non_blocking(true);
        std::cerr << e.what() << std::endl;
    }
    return -1;
}

int UDPClient::askPlayerIsLoading(int askTry)
{
    try {

        if (askTry <= 0)
            return -1;

        std::mutex m;

        m.lock();
        this->_socket.non_blocking(false);

        /////////////////////////////////////////
        DataStruct dataSend{
            .command = ISLOADING,
        };
        /////////////////////////////////////////

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[CLIENT] " << byteSent << " bytes sent" << std::endl;

        DataStruct dataRecv;
        std::cout << "[Attente] dans askPlayerIsLoading" << std::endl;
        this->_socket.receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint);
        std::cout << "[Réponse] dans askPlayerIsLoading" << std::endl;

        if (dataRecv.command == OK) {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] Server understand I'm loading" << std::endl;
            m.unlock();
            return 0;
        }
        else if (dataRecv.command == KO) {
            this->_socket.non_blocking(true);
            m.unlock();
            std::cout << "[CLIENT] KO" << std::endl;
            return askPlayerIsPlaying(askTry - 1);
        }
        else {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] Error UNKNOW Command" << std::endl;
            m.unlock();
            return askPlayerIsPlaying(askTry - 1);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return -1;
}

int UDPClient::askPlayerIsPlaying(int askTry)
{
    try {

        if (askTry <= 0)
            return -1;

        std::mutex m;

        m.lock();
        this->_socket.non_blocking(false);

        /////////////////////////////////////////
        DataStruct dataSend {
            .command = ISPLAYING,
        };
        /////////////////////////////////////////

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[CLIENT] " << byteSent << " bytes sent" << std::endl;

        DataStruct dataRecv;
        std::cout << "[Attente] dans askPlayerIsPlaying" << std::endl;
        this->_socket.receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint);
        std::cout << "[Réponse] dans askPlayerIsPlaying" << std::endl;

        if (dataRecv.command == OK) {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] Server understand I'm ready to play" << std::endl;
            m.unlock();
            return 0;
        }
        else if (dataRecv.command == KO) {
            this->_socket.non_blocking(true);
            m.unlock();
            std::cout << "[CLIENT] KO" << std::endl;
            return askPlayerIsPlaying(askTry - 1);
        } else {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] Error UNKNOW Command" << std::endl;
            m.unlock();
            return askPlayerIsPlaying(askTry - 1);
        }
    }
    catch (std::exception& e) {
        this->_socket.non_blocking(true);
        std::cerr << e.what() << std::endl;
    }
    return -1;
}

int UDPClient::askPlayerId(int askTry)
{
    std::cout << "[CLIENT] Dans la fonction askPlayerID" << std::endl;

    try {

        if (askTry <= 0)
            return -1;

        std::mutex m;

        m.lock();
        this->_socket.non_blocking(false);

        /////////////////////////////////////////
        DataStruct dataSend{
            .command = ID,
        };
        /////////////////////////////////////////

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[CLIENT] " << byteSent << " bytes sent" << std::endl;

        DataStruct dataRecv;
        std::cout << "[Attente] dans askPlayerId" << std::endl;
        this->_socket.receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint);
        std::cout << "[Réponse] dans askPlayerId" << std::endl;

        if (dataRecv.command == OK) {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] Your ID is:" << dataRecv.currentPlayer.playerID << std::endl;
            m.unlock();
            return dataRecv.currentPlayer.playerID;
        }
        else if (dataRecv.command == KO) {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] KO" << std::endl;
            m.unlock();
            return this->askPlayerId(askTry - 1);
        }
        else {
            this->_socket.non_blocking(true);
            std::cout << "COMMAND RECEIVE: " << dataRecv.command << std::endl;
            std::cout << "[CLIENT] Error UNKNOW Command" << std::endl;
            m.unlock();
            return this->askPlayerId(askTry - 1);
        }
    }
    catch (std::exception& e) {
        this->_socket.non_blocking(true);
        std::cerr << e.what() << std::endl;
    }
    return -1;
}

int UDPClient::askEnemyNumbers(int askTry)
{
    try {

        if (askTry <= 0)
            return -1;

        std::mutex m;

        m.lock();
        this->_socket.non_blocking(false);

        /////////////////////////////////////////
        DataStruct dataSend{
            .command = ENEMYCOUNT,
        };
        /////////////////////////////////////////

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[CLIENT] " << byteSent << " bytes sent" << std::endl;

        DataStruct dataRecv;
        std::cout << "[Attente] dans askEnemyNumbers" << std::endl;
        this->_socket.receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint);
        std::cout << "[Réponse] dans askEnemyNumbers" << std::endl;

        if (dataRecv.command == OK) {
            this->_socket.non_blocking(true);
            m.unlock();
            return dataRecv.currentRoom.enemyCount;
        }
        else if (dataRecv.command == KO) {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] KO" << std::endl;
            m.unlock();
            return this->askEnemyNumbers(askTry - 1);
        }
        else {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] Error UNKNOW Command" << std::endl;
            m.unlock();
            return this->askEnemyNumbers(askTry - 1);
        }
    }
    catch (std::exception& e) {
        this->_socket.non_blocking(true);
        std::cerr << e.what() << std::endl;
    }

    return -1;
}

BreedData UDPClient::askBreeds(int breedIndex, int askTry)
{
    std::cout << "[CLIENT] Dans la fonction askEnemies" << std::endl;

    try {

        if (askTry <= 0)
            return { .occurences = -1 };

        std::mutex m;

        m.lock();
        this->_socket.non_blocking(false);

        /////////////////////////////////////////
        DataStruct dataSend{
            .command = BREED,
            .currentRoom {
            .askedBreed = static_cast<char>(breedIndex),
            }
        };
        /////////////////////////////////////////

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[CLIENT] " << byteSent << " bytes sent" << std::endl;

        DataStruct dataRecv;
        std::cout << "[Attente] dans askBreeds" << std::endl;
        this->_socket.receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint);
        std::cout << "[Réponse] dans askBreeds" << std::endl;

        if (dataRecv.command == OK) {
            this->_socket.non_blocking(true);
            m.unlock();
            return dataRecv.currentRoom.breed;
        }
        else if (dataRecv.command == KO) {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] KO" << std::endl;
            m.unlock();
            return this->askBreeds(breedIndex, askTry - 1);
        }
        else {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] Error UNKNOW Command" << std::endl;
            m.unlock();
            return this->askBreeds(breedIndex, askTry - 1);
        }
    }
    catch (std::exception& e) {
        this->_socket.non_blocking(true);
        std::cerr << e.what() << std::endl;
    }
    return { .occurences = -1 };
}

int UDPClient::askBreedsNumbers(int askTry)
{

    std::cout << "askBreed" << std::endl;

    try {

        if (askTry <= 0)
            return -1;

        std::mutex m;

        m.lock();
        this->_socket.non_blocking(false);

        /////////////////////////////////////////
        DataStruct dataSend{
            .command = BREEDNUMBER,
        };
        /////////////////////////////////////////

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[CLIENT] " << byteSent << " bytes sent" << std::endl;

        DataStruct dataRecv;
        std::cout << "[Attente] dans askBreedsNumbers" << std::endl;
        this->_socket.receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint);
        std::cout << "[Réponse] dans askBreedsNumbers" << std::endl;

        if (dataRecv.command == OK) {
            this->_socket.non_blocking(true);
            m.unlock();
            return dataRecv.currentRoom.breedCount;
        }
        else if (dataRecv.command == KO) {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] KO" << std::endl;
            m.unlock();
            return this->askBreedsNumbers(askTry - 1);
        }
        else {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] Error UNKNOW Command" << std::endl;
            m.unlock();
            return this->askBreedsNumbers(askTry - 1);
        }
    }
    catch (std::exception& e) {
        this->_socket.non_blocking(true);
        std::cerr << e.what() << std::endl;
    }
    return -1;
}

void UDPClient::resetServerEndpoint(void)
{
    boost::system::error_code error;
    boost::asio::ip::address endpointAddress = boost::asio::ip::address::from_string(this->_serverHost, error);

    if (error.value() != 0) {
        std::cerr << error.message() << std::endl;
        return;
    }
    this->_serverEndpoint = boost::asio::ip::udp::endpoint(endpointAddress, this->_serverPort);
}

void UDPClient::resetOperationSuccedeed(void)
{
    this->_operationSuccedeed = false;
}

void UDPClient::awnserPong(void)
{
    DataStruct dataSend = {
        .command = CommandCode::PONG,
    };

    this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
}

std::array<int, 3> UDPClient::askGetPlayerColor(int playerID, int askTry)
{
    std::cout << "[CLIENT] Dans la fonction askPlayerColor" << std::endl;

    try {

        if (askTry <= 0)
            return { -1, -1, -1 };

        std::mutex m;

        m.lock();
        this->_socket.non_blocking(false);

        /////////////////////////////////////////
        DataStruct dataSend {
            .command = GETPLAYERCOLOR,
            .currentPlayer {
                .playerID = playerID,
            }
        };
        /////////////////////////////////////////

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[CLIENT] " << byteSent << " bytes sent" << std::endl;

        DataStruct dataRecv;
        std::cout << "[Attente] dans askGetPlayerColor" << std::endl;
        this->_socket.receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint);
        std::cout << "[Réponse] dans askGetPlayerColor" << std::endl;

        if (dataRecv.command == OK) {
            this->_socket.non_blocking(true);
            m.unlock();

            return dataRecv.currentPlayer.playerColors;
        }
        else if (dataRecv.command == KO) {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] KO" << std::endl;
            m.unlock();
            return this->askGetPlayerColor(playerID, askTry - 1);
        }
        else {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] Error UNKNOW Command" << std::endl;
            m.unlock();
            return this->askGetPlayerColor(playerID, askTry - 1);
        }
    }
    catch (std::exception& e) {
        this->_socket.non_blocking(true);
        std::cerr << e.what() << std::endl;
    }
    return { -1, -1, -1 };
}

int UDPClient::askSetPlayerColor(std::array<int, 3> colors, int askTry)
{
    std::cout << "[CLIENT] Dans la fonction askPlayerColor" << std::endl;

    try {

        if (askTry <= 0)
            return -1;

        std::mutex m;

        m.lock();
        this->_socket.non_blocking(false);

        /////////////////////////////////////////
        DataStruct dataSend{
            .command = SETPLAYERCOLOR,
            .currentPlayer {
                .playerColors = colors,
            }
        };
        /////////////////////////////////////////

        size_t byteSent = this->_socket.send_to(boost::asio::buffer(&dataSend, sizeof(dataSend)), this->_roomEndpoint);
        std::cout << "[CLIENT] " << byteSent << " bytes sent" << std::endl;

        DataStruct dataRecv;
        std::cout << "[Attente] dans askSetPlayerColor" << std::endl;
        this->_socket.receive_from(boost::asio::buffer(&dataRecv, sizeof(dataRecv)), this->_roomEndpoint);
        std::cout << "[Réponse] dans askSetPlayerColor" << std::endl;
        if (dataRecv.command == OK) {
            this->_socket.non_blocking(true);
            m.unlock();
            return 0;
        }
        else if (dataRecv.command == KO) {
            this->_socket.non_blocking(true);
            std::cout << "[CLIENT] KO" << std::endl;
            m.unlock();
            return this->askSetPlayerColor(colors, askTry - 1);
        }
        else {
            this->_socket.non_blocking(true);
            std::cout << dataRecv.command << std::endl;
            std::cout << "[CLIENT] Error UNKNOW Command" << std::endl;
            m.unlock();
            return this->askSetPlayerColor(colors, askTry - 1);
        }
    }
    catch (std::exception& e) {
        this->_socket.non_blocking(true);
        std::cerr << e.what() << std::endl;
    }
    return -1;
}