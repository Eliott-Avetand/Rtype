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
//  File:            Music.cpp                                                       //
//  Description:     Music class file                                                //
//                                                                                   //
// ********************************************************************************* //
#include "Options.hpp"

rtype::settings::Music::Music():
    _hoverSound(HOVER_SOUND), _clickSound(CLICK_SOUND)
{
    _isSoundActive = true;
    _actualMusicPath = "";
    _isMusicActive = true;
    _musicVolume = 100;
    _soundVolume = 100;
    _oldMusicTime = 0;
}

sf::Music &rtype::settings::Music::getMusicStream()
{
    return this->_music;
}

void rtype::settings::Music::playHoverSound(void)
{
    _hoverSound.playSound(_isSoundActive);
}

void rtype::settings::Music::playClickSound(void)
{
    _clickSound.playSound(_isSoundActive);
}

sf::Sound rtype::settings::Music::getClickSound()
{
    return _clickSound.getSound();
}

sf::Sound rtype::settings::Music::getHoverSound()
{
    return _hoverSound.getSound();
}

bool rtype::settings::Music::isSoundActive()
{
    return _isSoundActive;
}

bool rtype::settings::Music::isMusicActive()
{
    return _isMusicActive;
}

void rtype::settings::Music::changeMusic(std::string musicPath)
{
    _music.stop();
    if (!_music.openFromFile(musicPath))
        throw rtype::settings::Errors("Can't open file", "File Error:");
    _actualMusicPath = musicPath;
    _music.setLoop(true);
    _music.play();
}

void rtype::settings::Music::rechangeMusic(std::string musicPath)
{
}

void rtype::settings::Music::changePitch(float pitch)
{
    _music.setPitch(pitch);
}


void rtype::settings::Music::toggleSound(bool isActive)
{
    _isSoundActive = isActive;
}

void rtype::settings::Music::toggleMusic(bool isActive)
{
    _isMusicActive = isActive;
    if (!isActive)
        _music.pause();
    else
        _music.play();
}

int rtype::settings::Music::getMusicVolume()
{
    return _musicVolume;
}

int rtype::settings::Music::getSoundVolume()
{
    return _soundVolume;
}

std::string rtype::settings::Music::getMusicPath(void)
{
    return _actualMusicPath;
}

void rtype::settings::Music::increaseSound(int addSound)
{
    if (_soundVolume + addSound >= 100)
        _soundVolume = 100;
    else
        _soundVolume += addSound;
    _hoverSound.getSound().setVolume(_soundVolume);
    _clickSound.getSound().setVolume(_soundVolume);
}

void rtype::settings::Music::decreaseSound(int subSound)
{
    if (_soundVolume - subSound <= 0)
        _soundVolume = 0;
    else
        _soundVolume -= subSound;
    _hoverSound.getSound().setVolume(_soundVolume);
    _clickSound.getSound().setVolume(_soundVolume);
}

void rtype::settings::Music::increaseMusic(int addVolume)
{
    if (_musicVolume + addVolume >= 100)
        _musicVolume = 100;
    else
        _musicVolume += addVolume;
    _music.setVolume(_musicVolume);
}

void rtype::settings::Music::decreaseMusic(int subMusic)
{
    if (_musicVolume - subMusic <= 0)
        _musicVolume = 0;
    else
        _musicVolume -= subMusic;
    _music.setVolume(_musicVolume);
}

void rtype::settings::Music::setMusicVolume(int musicVolume)
{
    if (musicVolume < 0)
        musicVolume = 0;
    if (musicVolume > 100)
        musicVolume = 100;

    this->_musicVolume = musicVolume;
    this->_music.setVolume(musicVolume);
}

void rtype::settings::Music::setSoundVolume(int soundVolume)
{
    if (soundVolume < 0)
        soundVolume = 0;
    if (soundVolume > 100)
        soundVolume = 100;

    this->_soundVolume = soundVolume;
    _hoverSound.getSound().setVolume(soundVolume);
    _clickSound.getSound().setVolume(soundVolume);
}

void rtype::settings::Music::update()
{
}
