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
//  File:            MoveDownCommand.cpp                                             //
//  Description:     MoveDownCommand class                                           //
//                                                                                   //
// ********************************************************************************* //
#include "MoveDownCommand.hpp"

/**
 * @brief Create component which have all sprite'informations
 * @see Constructor is called when this class is create
 */
void rtype::action::MoveDownCommand::perform(std::unique_ptr<rtype::actor::Actor>& actor)
{
	actor->moveDown();
}