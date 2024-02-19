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
//  File:            CustomSound.cpp                                                 //
//  Description:     CustomSound class file                                          //
//                                                                                   //
// ********************************************************************************* //
#include "CustomSound.hpp"

rtype::engine::utilities::CustomSound::CustomSound(std::string soundFile)
{
	_buffer = std::make_unique<sf::SoundBuffer>();

	if (!_buffer)
		throw rtype::settings::Errors("Not enough memories", "MemoryError:");

	if(!_buffer->loadFromFile(soundFile))
		throw rtype::settings::Errors("Cannot load file " + soundFile, "FileError");

	_sound.setBuffer(*_buffer);
}

void rtype::engine::utilities::CustomSound::playSound(bool isSoundActive)
{
	if (isSoundActive)
		_sound.play();
}

sf::Sound rtype::engine::utilities::CustomSound::getSound(void)
{
	return _sound;
}