cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(
    r-type_client
    VERSION 0.1
    DESCRIPTION "R-Type Client"
    LANGUAGES CXX
)

set(
    SOURCES
    "${PROJECT_SOURCE_DIR}/engine/src/engine/Engine.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/engine/Time.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/settings/Errors.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/settings/Options.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/settings/Music.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/utilities/CustomSound.cpp"
    "${PROJECT_SOURCE_DIR}/engine/src/utilities/GameDatas.cpp"

    "${PROJECT_SOURCE_DIR}/client/src/utilities/AnimatedSprite.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/StaticSprite.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/CustomText.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/CustomSlider.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/CustomButton.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/CustomCheckBox.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/CustomRect.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/RandomNumbers.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/InputBox.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/backgrounds/PlanetsGenerator.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/backgrounds/AsteroidsGenerator.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/backgrounds/BackgroundSpaceGenerator.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/keyboard/VirtualKeyboard.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/utilities/keyboard/VirtualKeys.cpp"

    "${PROJECT_SOURCE_DIR}/client/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/UDPClient.cpp"

    "${PROJECT_SOURCE_DIR}/client/src/menus/MainMenu/MainMenu.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/menus/MainMenu/ColorPlayerPopup.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/menus/MainMenu/MainMenuOptions.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/menus/MainMenu/MainMenuEasterEgg.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/menus/MainMenu/Credits.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/menus/MultiplayerMenu/MultiplayerRoomMenu.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/menus/MultiplayerMenu/RoomInfoComponent.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/menus/MultiplayerMenu/MultiplayerConfigMenu.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/menus/MultiplayerMenu/ConnectionPopup.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/menus/MultiplayerWaitingRoom/MultiplayerWaitingRoom.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/menus/MultiplayerWaitingRoom/SlotInfoComponent.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/menus/LoadingScreen.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/menus/BeginingScreen.cpp"

    "${PROJECT_SOURCE_DIR}/client/src/game/UI/GameUI.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/game/UI/ShootBar.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/game/menus/PauseMenu.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/game/menus/PauseMenuOptions.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/game/menus/GameOverScreen.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/game/menus/EndingScreen.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/game/menus/TransitionScreen.cpp"
    "${PROJECT_SOURCE_DIR}/client/src/game/levels/Level_1.cpp"

    "${PROJECT_SOURCE_DIR}/components/src/Actor/Monster.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Actor/Breed.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Actor/Player.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Actor/Missile.cpp"

    "${PROJECT_SOURCE_DIR}/components/src/Actor/MissileBoss.cpp"

    "${PROJECT_SOURCE_DIR}/components/src/Actor/Asteroid.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Actor/BossDobkeratops.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/InputHandler.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/TestCommand.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/MoveUpCommand.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/MoveDownCommand.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/MoveRightCommand.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/MoveLeftCommand.cpp"
    "${PROJECT_SOURCE_DIR}/components/src/Action/ShootCommand.cpp"

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
    "${PROJECT_SOURCE_DIR}/components/include/Actor/Monster.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/Breed.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/Player.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/Missile.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/MissileBoss.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/BossDobkeratops.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/ACommand.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/InputHandler.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/TestCommand.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/MoveUpCommand.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/ShootCommand.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/MoveDownCommand.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/MoveRightCommand.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Action/MoveLeftCommand.hpp"

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

    "${PROJECT_SOURCE_DIR}/client/include/menus/MainMenu/MainMenu.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MainMenu/ColorPlayerPopup.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MainMenu/MainMenuOptions.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MainMenu/MainMenuEasterEgg.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MainMenu/Credits.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MultiplayerMenu/MultiplayerRoomMenu.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MultiplayerMenu/RoomInfoComponent.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MultiplayerMenu/ConnectionPopup.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MultiplayerMenu/MultiplayerConfigMenu.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MultiplayerWaitingRoom/MultiplayerWaitingRoom.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MultiplayerWaitingRoom/SlotInfoComponent.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/menus/LoadingScreen.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/menus/BeginingScreen.hpp"

    "${PROJECT_SOURCE_DIR}/client/include/game/UI/GameUI.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/game/UI/ShootBar.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/game/menus/PauseMenu.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/game/menus/PauseMenuOptions.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/game/menus/GameOverScreen.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/game/menus/EndingScreen.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/game/menus/TransitionScreen.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/game/levels/Level_1.hpp"

    "${PROJECT_SOURCE_DIR}/client/include/utilities/GlobalUtils.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/ACustomSprite.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/StaticSprite.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/AnimatedSprite.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/CustomText.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/CustomSlider.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/CustomButton.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/CustomCheckBox.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/CustomRect.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/RandomNumbers.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/backgrounds/PlanetsGenerator.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/backgrounds/AsteroidsGenerator.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/InputBox.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/backgrounds/BackgroundSpaceGenerator.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/keyboard/VirtualKeyboard.hpp"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/keyboard/VirtualKeys.hpp"

    "${PROJECT_SOURCE_DIR}/client/include/UDPClient.hpp"

    "${PROJECT_SOURCE_DIR}/components/include/Actor/Actor.hpp"
    "${PROJECT_SOURCE_DIR}/components/include/Actor/Asteroid.hpp"
)

add_executable(r-type_client ${SOURCES} ${HEADERS})

target_compile_definitions(r-type_client PRIVATE CLIENT)

target_link_libraries(r-type_client ${CONAN_LIBS})

target_include_directories(
    r-type_client PRIVATE
    "${PROJECT_SOURCE_DIR}/engine/include"
    "${PROJECT_SOURCE_DIR}/engine/include/engine"
    "${PROJECT_SOURCE_DIR}/engine/include/settings"
    "${PROJECT_SOURCE_DIR}/engine/include/utilities"
    "${PROJECT_SOURCE_DIR}/server/include"
    "${PROJECT_SOURCE_DIR}/components/include"
    "${PROJECT_SOURCE_DIR}/components/include/Action"
    "${PROJECT_SOURCE_DIR}/components/include/Action/Pattern"
    "${PROJECT_SOURCE_DIR}/components/include/Scene"
    "${PROJECT_SOURCE_DIR}/components/include/Actor"
    "${PROJECT_SOURCE_DIR}/components/include/PowerUp"
    "${PROJECT_SOURCE_DIR}/components/include/PowerUp/Items"
    "${PROJECT_SOURCE_DIR}/components/include/PowerUp/Actions"
    "${PROJECT_SOURCE_DIR}/client/include"
    "${PROJECT_SOURCE_DIR}/client/include/menus"
    "${PROJECT_SOURCE_DIR}/client/include/game"
    "${PROJECT_SOURCE_DIR}/client/include/game/menus"
    "${PROJECT_SOURCE_DIR}/client/include/game/levels"
    "${PROJECT_SOURCE_DIR}/client/include/game/UI"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MainMenu"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MultiplayerMenu"
    "${PROJECT_SOURCE_DIR}/client/include/menus/MultiplayerWaitingRoom"
    "${PROJECT_SOURCE_DIR}/client/include/utilities"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/backgrounds"
    "${PROJECT_SOURCE_DIR}/client/include/utilities/keyboard"
)
