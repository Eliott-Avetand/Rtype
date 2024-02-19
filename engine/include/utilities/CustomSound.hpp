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
//  File:            CustomSound.hpp                                                 //
//  Description:     CustomSound class header file                                   //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <memory>

#include <SFML/Audio.hpp>

#include "Errors.hpp"

namespace rtype {
	namespace engine {
		namespace utilities {
			class CustomSound {
				private:
					sf::Sound _sound;
					std::unique_ptr<sf::SoundBuffer> _buffer;

				public:
					CustomSound(std::string soundFile = "");

					void playSound(bool isSoundActive);
					sf::Sound getSound(void);
			};
		};
	};
}