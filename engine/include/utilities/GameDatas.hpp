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
//  File:            GameDatas.hpp                                                   //
//  Description:     GameDatas class header file                                     //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <SFML/Graphics.hpp>

namespace rtype {
	namespace engine {

		class GameDatas {
			public:
				GameDatas();

				int getScore(void);
				void setScore(int score);

				int getHighScore(void);
				void setHighScore(int highScore);

				int getActualLevel(void);
				void setActualLevel(int level);

				void setPlayerColor(sf::Color color);
				sf::Color getPlayerColor(void);
			private:
				int _score;
				int _highScore;

				int _actualLevel;
				sf::Color _playerColor;
		};
	}
}
