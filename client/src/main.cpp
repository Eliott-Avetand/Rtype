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
//  File:            main.cpp                                                        //
//  Description:     This is the main file of the client.                            //
//                                                                                   //
// ********************************************************************************* //

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "Engine.hpp"
#include "Errors.hpp"
#include "MainMenu.hpp"
#include "BeginingScreen.hpp"
#include "Credits.hpp"
#include "../include/main.hpp"

#include "Credits.hpp"

int main(int ac, char **av)
{
    rtype::engine::Engine launcher;

    try {
        std::string host = "";
        std::string port = "";
        UDPClient client(host, port);

        launcher.init();
        launcher.setClient(&client);
        launcher.changeState(rtype::client::BeginingScreen::getInstance());
        launcher.gameLoop();
        launcher.cleanup();
    } catch (rtype::settings::Errors const &error) {
        std::cout << error.getErrorType() << ": " << error.what() << std::endl;
        return 84;
    }
	return 0;
}
