// #include "birce.hpp"
// #include <sys/socket.h>
// #include <string.h>
// #include <iostream>

// void	client_write(t_env *e, int cs)
// {
// 	e->commander.client_write(cs);

// 	/*
// 	char	buf_write[BUF_SIZE + 1];
// 	std::string *out_commands; //a reference causes a compile error, so go with a pointer
// 	out_commands = &(e->commander.out_commands()[cs]);

// 	if (out_commands->empty())
// 		return;

// 	int buf_size;
// 	while (out_commands->length()) {

// 		if (out_commands->length() > BUF_SIZE)
// 			buf_size = BUF_SIZE;
// 		else
// 			buf_size = out_commands->length();

// 		sprintf(buf_write, "%s", out_commands->substr(0, buf_size).c_str());
// 		buf_write[buf_size] = 0;
// 		send(cs, buf_write, buf_size, 0);
// 		out_commands->erase(0, buf_size);
// 	}
// 	*/
// }
