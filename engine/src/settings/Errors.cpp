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
//  File:            Errors.cpp                                                      //
//  Description:     Errors class file                                               //
//                                                                                   //
// ********************************************************************************* //
#include "Errors.hpp"

rtype::settings::Errors::Errors(std::string const &message, std::string const &errorType)
{
    this->_name = errorType;
    this->_message = message;
}

const char *rtype::settings::Errors::what() const noexcept
{
    return this->_message.c_str();
}

std::string const &rtype::settings::Errors::getErrorType() const
{
    return this->_name;
}