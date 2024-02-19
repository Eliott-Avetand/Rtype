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
//  File:            MoveLeftCommand.cpp                                             //
//  Description:     MoveLeftCommand class                                           //
//                                                                                   //
// ********************************************************************************* //
#include "MoveLeftCommand.hpp"

/**
 * @brief Move Left Command: Call the move left methods of selected actor
 * @see This command is called when an input is pressed
 */
void rtype::action::MoveLeftCommand::perform(std::unique_ptr<rtype::actor::Actor>& actor)
{
	actor->moveLeft();
}