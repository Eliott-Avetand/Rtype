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
//  File:            MoveRightCommand.cpp                                            //
//  Description:     MoveRightCommand class                                          //
//                                                                                   //
// ********************************************************************************* //
#include "MoveRightCommand.hpp"

/**
 * @brief Move Right Command: Call the move right methods of selected actor
 * @see This command is called when an input is pressed
 */
void rtype::action::MoveRightCommand::perform(std::unique_ptr<rtype::actor::Actor>& actor)
{
	actor->moveRight();
}