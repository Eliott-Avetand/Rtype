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
//  File:            Server.hpp                                                      //
//  Description:     This is the main of the project.                                //
//                   It contains the main() function which creates and inits the     //
//                   UDP server thanks to the Boost.asio lib                         //
//                                                                                   //
// ********************************************************************************* //

#include "../include/UDPServer.hpp"
#include "Engine.hpp"

int main()
{
    try {
        rtype::engine::Engine engine;
        engine.setServer(new UDPServer());
        std::cout << "[SERVER] Server is running..." << std::endl;

        // This is used to handle the SIGINT signal
        static UDPServer* handler_ptr = engine.getServer();
        signal(SIGINT, [](int signum) {handler_ptr->signalHandler(signum);});
        //

        engine.getServer()->run();
        std::cout << "[SERVER] Server is stopped" << std::endl;

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}