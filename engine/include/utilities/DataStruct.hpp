
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
//  File:            DataStruct.hpp                                                  //
//  Description:     File containing all the structure to be send by client          //
//                   and server with all new data                                    //
//                                                                                   //
//                                                                                   //
// ********************************************************************************* //


#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <boost/asio.hpp>

/**
 * @brief CommandCode
 * 
 * @details
 * 
 * CREATE: Create a room                            | Do nothing if the player is already in a game / room
 * JOIN: Join a room                                | Can join the game only if the player was disconnected from this specific room
 * NAME: Change the player name                     | Do nothing in game
 * LIST: List all the rooms                         | Do nothing in game
 * LEAVE: Leave a room
 * INFO: Get the info of a room                     | Do nothing in game
 * START: Start a room                              | Do nothing in game
 * STOP: Stop the starting of a room                | Do nothing in game
 * PAUSE: Pause a game                              | Do nothing in a room (waiting for players)
 * RESUME: Resume a game                            | Do nothing in a room (waiting for players)
 * PCOUNT: Ask the player count of a room           | Do nothing in game
 * SHOOT: Player shoot                              | Do nothing in a room (waiting for players)
 * MOVE: Player move                                | Do nothing in a room (waiting for players)
 * DIE: Player die                                  | Do nothing in a room (waiting for players)
 * ID: Get the player ID                            | Do nothing in game
 * ISPLAYING: Ask if the player is playing
 * 
 * OK: SERVER ONLY
 * KO: SERVER ONLY
 * CLOSING: SERVER ONLY
 * PING: Ask the other party if it's still alive / Permit to calculate the latency
 * PONG: Answer to a PING
 * LATENCY: 
 *
*/
typedef enum c {

    CREATE,
    JOIN,
    NAME,
    PLAYERNAME,
    LIST,
    LEAVE,
    INFO,
    READY,
    START,
    STOP,
    PAUSE,
    RESUME,
    PCOUNT,
    SHOOT,
    MOVE,
    DIE,
    ID,
    ENEMY,
    ENEMYSHOOT,
    ENEMYDAMAGE,
    ENEMYCOUNT,
    PLAYERSCORE,
    PLAYERDAMAGE,
    SETPLAYERCOLOR,
    GETPLAYERCOLOR,
    BOSSSPAWN,
    BOSSTAILS,
    BOSSMISSILE,
    BOSSHEARTMISSILE,
    BOSSDAMAGE,
    BOSSHEARTINFO,
    BOSSMISSILEDESTROYED,
    BOSSHEARTMISSILEDESTROYED,
    PLAYERBOSSDAMAGE,
    BOSSHEARTDESTROY,
    ASTEROID,
    ASTEROIDDESTROY,
    PLAYERASTEROIDDAMAGE,
    BREED,
    BREEDNUMBER,
    ISPLAYING,
    ISLOADING,

    LEVELSTATUS,

    OK,
    KO,
    CLOSING,
    PING,
    PONG,
    LATENCY

} CommandCode;

typedef enum {
    INGAME,
    WIN,
    LOOSE,
    ENDING,
} LevelState;

/**
 * @brief This class is used to store room's informations
 * 
 * @details This class is used to store room's informations
 * 
 * WAITING: The room is waiting for players
 * PLAYING: The room is playing
 * ENDED: The room has ended
 * CLOSED: The room has been closed
*/
typedef enum {
    WAITING,
    PLAYING,
    ENDED,
    CLOSED
} RoomState;

/**
 * @brief MoveDirection
 * 
 * UP: Move up
 * DOWN: Move down
 * LEFT: Move left
 * RIGHT: Move right
 * IDLE: Do nothing
*/
typedef enum m {

    UP,
    DOWN,
    LEFT,
    RIGHT,
    IDLE

} MoveDirection;

/**
 * @brief Structure containing all the player data to be send by client and server
 * 
 * pseudo: Player's pseudo (32 char max)
 * pseudoLength: Length of the player's pseudo
 * isReady: Is the player ready to play
 * isAlive: Is the player still alive
 * positions: Player's positions
 * moveDirection: Player's move direction
*/
typedef struct pdata {

    char pseudo[32];
    char pseudoLength;
    int playerID;
    int score;
    int health;

    std::array<int, 3> playerColors;

    bool isReady;
    bool isAlive;
    bool isPlaying;

    int latency;

    std::pair<float, float> positions;
    MoveDirection moveDirection;

} PlayerData;

typedef struct dmData
{
    char missileID[11];

    bool isDestroyed;

    std::pair<float, float> positions;
} dobkeratopsMissileDatas;

typedef struct dData
{
    std::pair<float, float> tailPositions;
    int tailIndex;

    char heartId[11];
    int heartIndex;

    dobkeratopsMissileDatas missileDatas;
} dobkeratopsDatas;

typedef struct aData {
    char asteroidID[11];

    bool isDestroyed;
    float size;
    float speed;

    std::pair<float, float> positions;
} AsteroidsDatas;

/**
 * @brief Structure containing all the breed data to be send by client and server
 *
 * name: name of the breed
 * maxHealth: max health of the monster contained in the breed
 * occurences: all monsters which can spawn in the breed
 * type: type of monster
*/
typedef struct ebreeds {
    char name[32];
    int maxHealth;
    int occurences;
    char sprite[100];
} BreedData;

/**
 * @brief Structure containing all the enemies data to be send by client and server
 *
 * enemyID: ID of the actual enemy
 * positions: Positions x and y of the enemy
*/
typedef struct edata {
    char enemyID[11];
    bool isAlive;
    bool isNew;

    int breedIndex;
    std::pair<float, float> positions;
} EnemyData;

/**
 * @brief 
 * 
*/
typedef struct rd {

    short roomNameLength;
    char roomName[32];
    char playerCount;
    RoomState state;
    unsigned short port;
    char password[5];

    char levelNum;
    LevelState levelState;

    std::array<PlayerData, 4> players;

    AsteroidsDatas asteroidInfos;

    BreedData breed;
    char breedCount;
    char askedBreed;
    
    dobkeratopsDatas bossInfos;

    EnemyData enemyInfos;
    int enemyCount;

} RoomData;

/**
 * @brief Structure containing all the data to be send by client and server
 * 
 * @details
 * 
 * command: CommandCode (see CommandCode enum)
 * pseudoLength: Length of the pseudo
 * pseudo: Pseudo of the player (32 char max)
 * port: Port of the server
 * 
 * currentRoom: RoomData of the current room
 * roomCount: Number of rooms
 * rooms: Array of RoomData
 * 
*/

typedef struct Ri {

    short roomNameLength;
    char roomName[32];

    char playerCount;
} RoomInfo;

typedef struct d {

    CommandCode command;

    //PLAYER
    short pseudoLength;
    char pseudo[32];

    // NETWORK
    unsigned short port;
    boost::asio::ip::udp::endpoint clientEndpoint;

    // ROOM
    RoomData currentRoom;
    PlayerData currentPlayer;

    short roomCount;
    RoomInfo rooms[10];

    // CLIENT
    // std::vector<Client> clients;
    // PLAYER

} DataStruct;
