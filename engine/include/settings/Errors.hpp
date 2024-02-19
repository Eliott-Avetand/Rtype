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
//  File:            GameEngine.hpp                                                  //
//  Description:     Errors class' header                                            //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <exception>
#include <string>

namespace rtype {
    namespace settings {
        class Errors : public std::exception {
            protected:
                std::string _message;
                std::string _name;

            public:
                Errors(std::string const &message, std::string const &errorType = "Unknown");

                virtual const char *what() const noexcept;
                std::string const &getErrorType() const;
        };
    };
};
