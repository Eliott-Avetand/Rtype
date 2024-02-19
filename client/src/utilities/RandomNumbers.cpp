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
//  File:            RandomNumbers.cpp                                               //
//  Description:     File for the RandomNumbers Class                                //
//                                                                                   //
// ********************************************************************************* //
#include "RandomNumbers.hpp"

int rtype::client::utilities::RandomNumbers::randomInteger(int min, int max)
{
	return ((rand() % max) + min);
}

float rtype::client::utilities::RandomNumbers::randomFloat(float min, float max)
{
	return (min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min))));
}