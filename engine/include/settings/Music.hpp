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
//  File:            Music.hpp                                                       //
//  Description:     Music class' header                                             //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <map>
#include <string>
#include <SFML/Audio.hpp>

#include "CustomSound.hpp"
#include "Errors.hpp"

#define HOVER_SOUND "./client/assets/musics/hover_sound.ogg"
#define CLICK_SOUND "./client/assets/musics/click_sound.ogg"

namespace rtype {
    namespace settings {
        class Music {
            private:
                sf::Music _music;
                sf::Time _offset;
                rtype::engine::utilities::CustomSound _hoverSound;
                rtype::engine::utilities::CustomSound _clickSound;
                std::string _actualMusicPath;
                int _soundVolume;
                int _musicVolume;
                bool _isSoundActive;
                bool _isMusicActive;
                float _oldMusicTime;

            public:
                Music();

                void update();

                void toggleMusic(bool isActive);
                void toggleSound(bool isActive);

                bool isSoundActive();
                bool isMusicActive();

                void changeMusic(std::string musicPath);
                void rechangeMusic(std::string musicPath);

                void changePitch(float pitch);

                sf::Music &getMusicStream();
                sf::Sound getClickSound();
                sf::Sound getHoverSound();

                std::string getMusicPath(void);

                void playClickSound(void);
                void playHoverSound(void);

                int getMusicVolume();
                int getSoundVolume();

                void increaseMusic(int addMusic);
                void increaseSound(int addSound);
                void decreaseMusic(int subMusic);
                void decreaseSound(int subSound);

                void setMusicVolume(int musicVolume);
                void setSoundVolume(int soundVolume);
        };
    }
}
