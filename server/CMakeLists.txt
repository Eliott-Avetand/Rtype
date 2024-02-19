cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(
    r-type_server
    VERSION 0.1
    DESCRIPTION "R-Type Server"
    LANGUAGES CXX
)

set(
    SOURCES
    "${PROJECT_SOURCE_DIR}/client/src/utilities/RandomNumbers.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/engine/Engine.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/engine/Time.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/settings/Errors.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/settings/Options.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/settings/Music.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/utilities/CustomSound.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/utilities/GameDatas.cpp"

    "${PROJECT_SOURCE_DIR}/server/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/server/src/UDPServer.cpp"
    "${PROJECT_SOURCE_DIR}/server/src/Room.cpp"
    "${PROJECT_SOURCE_DIR}/server/src/Game.cpp"
    "${PROJECT_SOURCE_DIR}/server/src/Client.cpp"


    "${PROJECT_SOURCE_DIR}/client/src/utilities/AnimatedSprite.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/StaticSprite.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/CustomText.cpp"

    "${PROJECT_SOURCE_DIR}/components/src/Actor/Monster.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Actor/Breed.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Actor/Player.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Actor/Missile.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Actor/Asteroid.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Actor/BossDobkeratops.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Actor/MissileBoss.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/InputHandler.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/TestCommand.cpp"

    "${PROJECT_SOURCE_DIR}/components/src/PowerUp/Items/TripleShootItem.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/PowerUp/Actions/PowerUpActionFactory.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/PowerUp/Actions/TripleShootAction.cpp"

    "${PROJECT_SOURCE_DIR}/components/src/Action/Pattern/CircularPattern.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/Pattern/SinusoidalPattern.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/Pattern/ZigZagPattern.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/Pattern/RandomPattern.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/Pattern/QuickLeftPattern.cpp"
)

set(
    HEADERS
    "${PROJECT_SOURCE_DIR}/engine/include/engine/Engine.hpp"
    "${PROJECT_SOURCE_DIR}/engine/include/engine/Time.hpp"
    "${PROJECT_SOURCE_DIR}/engine/include/settings/Errors.hpp"
    "${PROJECT_SOURCE_DIR}/engine/include/settings/Music.hpp"
    "${PROJECT_SOURCE_DIR}/engine/include/settings/Options.hpp"
    "${PROJECT_SOURCE_DIR}/engine/include/utilities/CustomSound.hpp"
    "${PROJECT_SOURCE_DIR}/engine/include/utilities/GameDatas.hpp"

    "${PROJECT_SOURCE_DIR}/components/include/Scene/AGameState.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Scene/APlayerMovementComponent.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/Monster.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/Actor.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/Breed.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/Player.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/BossDobkeratops.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/MissileBoss.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/Missile.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/Asteroid.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/ACommand.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/InputHandler.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/TestCommand.hpp"

    "${PROJECT_SOURCE_DIR}/components/include/Action/Pattern/ActorPattern.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/Pattern/CircularPattern.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/Pattern/SinusoidalPattern.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/Pattern/ZigZagPattern.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/Pattern/RandomPattern.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/Pattern/QuickLeftPattern.hpp"

    "${PROJECT_SOURCE_DIR}/components/include/PowerUp/Items/APowerUpItem.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/PowerUp/Items/TripleShootItem.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/PowerUp/Actions/APowerUpAction.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/PowerUp/Actions/PowerUpActionFactory.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/PowerUp/Actions/TripleShootAction.hpp"

    "${PROJECT_SOURCE_DIR}/client/include/utilities/ACustomSprite.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/AnimatedSprite.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/StaticSprite.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/RandomNumbers.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/CustomText.hpp"

    "${PROJECT_SOURCE_DIR}/server/include/UDPServer.hpp"
    "${PROJECT_SOURCE_DIR}/server/include/Room.hpp"
    "${PROJECT_SOURCE_DIR}/server/include/Game.hpp"
    "${PROJECT_SOURCE_DIR}/server/include/Client.hpp"
)

add_executable(r-type_server ${SOURCES} ${HEADERS})

target_compile_definitions(r-type_server PRIVATE SERVER)

target_link_libraries(r-type_server ${CONAN_LIBS})

target_include_directories(
    r-type_server PRIVATE
    "${PROJECT_SOURCE_DIR}/engine/include"
    "${PROJECT_SOURCE_DIR}/engine/include/engine"
    "${PROJECT_SOURCE_DIR}/engine/include/settings"
    "${PROJECT_SOURCE_DIR}/engine/include/utilities"
    "${PROJECT_SOURCE_DIR}/server/include/"

    "${PROJECT_SOURCE_DIR}/components/include"
    "${PROJECT_SOURCE_DIR}/components/include/Scene"
    "${PROJECT_SOURCE_DIR}/components/include/Actor"
    "${PROJECT_SOURCE_DIR}/components/include/Action"
    "${PROJECT_SOURCE_DIR}/components/include/Action/Pattern"

    "${PROJECT_SOURCE_DIR}/components/include/PowerUp"
    "${PROJECT_SOURCE_DIR}/components/include/PowerUp/Items"
    "${PROJECT_SOURCE_DIR}/components/include/PowerUp/Actions"

    "${PROJECT_SOURCE_DIR}/client/include/utilities"
)
