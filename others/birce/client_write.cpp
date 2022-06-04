#include "bircd.hpp"

#include <sys/socket.h>
#include <string.h>
#include <iostream>

void	client_write(t_env *e, int cs)
{
	//send(cs, e->fds[cs].buf_write, strlen(e->fds[cs].buf_write), 0);
	//e->fds[cs].buf_write[0] = 0;




	std::string str;
	while (e->fds[cs].write_queue.size()) {
		str = e->fds[cs].write_queue.front();
		send(cs, str.c_str(), str.length(), 0);
		e->fds[cs].write_queue.pop();
	}



}
