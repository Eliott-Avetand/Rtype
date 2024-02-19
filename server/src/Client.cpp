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
//  File:            Client.cpp                                                      //
//  Description:     File containing the Client class                                //
//                                                                                   //
//                                                                                   //
// ********************************************************************************* //

#include "../include/Client.hpp"

Client::Client(boost::asio::ip::udp::endpoint endpoint, size_t playerID)
{
    this->_endpoint = endpoint;
    this->_state = CONNECTED;
    this->_username = "_____";

    if (playerID > 3 || playerID < 1)
        this->_playerID = 3;
    this->_colors = {255,255,255};
    this->_playerID = playerID; 
    this->_isReady = false;
    this->_isPlaying = false;
}

Client::~Client()
{
}

// Getters

boost::asio::ip::udp::endpoint Client::getEndpoint() const
{
    return this->_endpoint;
}

std::string Client::getUsername() const
{
    return this->_username;
}

ClientState Client::getState() const
{
    return this->_state;
}

size_t Client::getPlayerID() const
{
    return this->_playerID;
}

bool Client::getIsReady() const
{
    return this->_isReady;
}

bool Client::getIsAlive() const
{
    return this->_isAlive;
}

std::pair<float, float> Client::getPositions() const
{
    return this->_positions;
}

MoveDirection Client::getMoveDirection() const
{
    return this->_moveDirection;
}

bool Client::getIsPlaying() const
{
    return this->_isPlaying;
}

long long Client::getTimestampPing(void) const
{
    return this->_timestampPing;
}

int Client::getLatency(void) const
{
    return this->_latency;
}

int Client::getAveragelatency(void) const
{
    return this->_latencyAverage;
}

int Client::getMaxLatency(void) const
{
    return this->_latencyMax;
}

int Client::getSpeed(void) const
{
    return this->_speed;
}

int Client::getLatencystdDeviation(void) const
{
    return this->_latencystdDeviation;
}

// Setters

void Client::setEndpoint(boost::asio::ip::udp::endpoint endpoint)
{
    this->_endpoint = endpoint;
}

std::string Client::getUsername(void)
{
    return this->_username;
}

void Client::setUsername(std::string username)
{
    this->_username = username;
}

void Client::setState(ClientState state)
{
    this->_state = state;
}

void Client::setPlayerID(size_t playerID)
{
    this->_playerID = playerID;
}

void Client::setIsReady(bool isReady)
{
    this->_isReady = isReady;
}


void Client::setIsAlive(bool isAlive)
{
    this->_isAlive = isAlive;
}

void Client::setPositions(std::pair<float, float> positions)
{
    this->_positions = positions;
}

void Client::setMoveDirection(MoveDirection moveDirection)
{
    this->_moveDirection = moveDirection;
}

void Client::setIsPlaying(bool isPlaying)
{
    this->_isPlaying = isPlaying;
}

std::array<int, 3> Client::getColors(void)
{
    return this->_colors;
}

void Client::setColors(std::array<int, 3> colors)
{
    this->_colors = colors;
}

PlayerData Client::getPlayerData(void) const
{
    PlayerData playerData = {
        .pseudoLength = static_cast<char>(this->_username.length()),
        .playerColors = this->_colors,
        .isReady = this->_isReady,
        .isAlive = this->_isAlive,
        .positions = this->_positions,
        .moveDirection = this->_moveDirection,
    };
    for (int i = 0; i < this->_username.length(); i++)
        playerData.pseudo[i] = this->_username[i];

    return playerData;
}

void Client::setTimestampPing(void)
{
    auto now = std::chrono::high_resolution_clock::now();
    this->_timestampPing = static_cast<long long>(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
}

void Client::updateLatency(void)
{
    auto now = std::chrono::high_resolution_clock::now();
    auto begin = std::chrono::high_resolution_clock::time_point(std::chrono::milliseconds(this->_timestampPing));
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - begin);
    auto tmp = static_cast<int>(duration.count());

    if (tmp > this->_latency)
        this->_latencyMax = tmp;

    if (this->_latencyList.size() >= 10)
        this->_latencyList.erase(this->_latencyList.begin());

    this->_latencyList.push_back(tmp);

    ///// Calculate Average latency of the client
    this->_latencyAverage = std::accumulate(this->_latencyList.begin(), this->_latencyList.end(), 0) / this->_latencyList.size();
    auto avrg = this->_latencyAverage;
    this->_latencyVariance = std::accumulate(this->_latencyList.begin(), this->_latencyList.end(), 0.0,
    [avrg](double acc, double x) { return acc + (x - avrg) * (x - avrg); }) / this->_latencyList.size();
    this->_latencystdDeviation = std::sqrt(this->_latencyVariance);
    //////////////////////////////////////////////

    this->_latency = tmp;
}

void Client::setSpeed(int newSpeed)
{
    this->_speed = newSpeed;
}