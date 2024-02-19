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
//  File:            ShootCommand.cpp                                                //
//  Description:     ShootCommand class                                              //
//                                                                                   //
// ********************************************************************************* //
#include "ShootCommand.hpp"

/**
 * @brief Shoot Command: Call the shoot methods of selected actor
 * @see This command is called when an input is pressed
 */
void rtype::action::ShootCommand::perform(std::unique_ptr<rtype::actor::Actor>& actor)
{
	actor->shoot();
}