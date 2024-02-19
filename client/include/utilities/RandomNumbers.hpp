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
//  File:            RandomNumbers.hpp                                               //
//  Description:     File for the RandomNumbers Header                               //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <ctime>
#include <cstdlib>

namespace rtype {
	namespace client {
		namespace utilities {
			class RandomNumbers {
				public:

					/**
					 * @brief Return a random integer number between two numbers
					 * @see This methods is called wherever you want
					 *
					 * @param int		Minimum integer we can return
					 * @param int		Maximum integer we can return
					 *
					 * @return int		Random integer
					 */
					static int randomInteger(int min, int max);

					/**
					 * @brief Return a random float number between two numbers
					 * @see This methods is called wherever you want
					 *
					 * @param int		Minimum float we can return
					 * @param int		Maximum float we can return
					 *
					 * @return int		Random float
					 */
					static float randomFloat(float min, float max);
			};
		};
	};
};