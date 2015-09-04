/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/
#include <stdlib.h>
#include <Wt/WServer>
#include "StudentSession.h"
#include "StudentApp.h"

int main(int argc, char **argv)
{
	
	
	try{
		//starts wserver instance, adds entry points for application
		WServer server(argv[0]);
		server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
		
		std::cout << "Adding student portal entry point ";
		server.addEntryPoint(Application,
			boost::bind(&createStudentApplication, _1),
			"/students");

		StudentSession::configureAuth();

		std::cout << "Starting Server";
		if (server.start()) {
			int sig = WServer::waitForShutdown(argv[0]);
			std::cerr << "Shutdown (signal = " << sig << ")" << std::endl;
			server.stop();
		}
		
	}
	catch (Wt::WServer::Exception& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (std::exception &e) {
		std::cerr << "exception: " << e.what() << std::endl;
	}
} 