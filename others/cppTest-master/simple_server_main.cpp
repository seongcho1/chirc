
//http://coffeenix.net/doc/lg/issue74/tougher.html
//https://github.com/ParkSC/cppTest

#include "ServerSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>

int main ( int argc, char **argv )
{
  std::cout << "running....\n";

  try {
	// Create the socket
	ServerSocket server ( 6667 );

	while ( true ) {

		ServerSocket new_sock;
		server.accept ( new_sock );

		try {
			while ( true ) {
				std::string data;
				new_sock >> data;
				data.append(" from server");
				new_sock << data;
			}
		} catch ( SocketException& ) {}
		}
	} catch ( SocketException& e ) {
	std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
	}

	return 0;
}
