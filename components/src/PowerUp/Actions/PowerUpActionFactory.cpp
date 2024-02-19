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
//  File:            PowerUpActionFactory.hpp                                        //
//  Description:     Factory that create PowerUp actions                             //
//                                                                                   //
// ********************************************************************************* //

#include "PowerUpActionFactory.hpp"

std::unique_ptr<rtype::powerup::APowerUpAction> rtype::powerup::PowerUpActionFactory::getPowerUpAction(std::string powerUpName)
{
    if (powerUpName == "TripleShoot") {
        auto ptr = std::make_unique<rtype::powerup::TripleShootAction>();
        if (!ptr)
            throw rtype::settings::Errors("Not enough memory", "MemoryError");
        return ptr;
    }
    else
        return nullptr;
}