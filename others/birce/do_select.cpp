
//#include <stdlib.h>
#include "birce.hpp"
#include <iostream>
void	do_select(t_env *e) {
	/*
	struct timeval timeout;
	timeout.tv_sec = 3;			//3 second timeout
	timeout.tv_usec = 0;		//0 micro second timeout
	*/

	e->r = select(e->max + 1, &e->fd_read, &e->fd_write, NULL, NULL); //&timeout); //NULL, 0
	//std::cout << "[select=" << e->r << "]" << std::endl;
}
