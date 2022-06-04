#include "bircd.hpp"

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>


void	client_read(t_env *e, int cs)
{
  int	r;
  int	i;
  char	buf_write[BUF_SIZE + 1];

  r = recv(cs, e->fds[cs].buf_read, BUF_SIZE, 0);
  if (r <= 0)
  {
    close(cs);
    clean_fd(&e->fds[cs]);
    printf("client #%d gone away\n", cs);
  }
  else
  {
 	e->fds[cs].buf_read[r] = 0;
	i = 0;
    while (i < e->maxfd)
    {
      if ((e->fds[i].type == FD_CLIENT) && (i != cs)) {
		//bzero(buf_write, sizeof(buf_write));
		//sprintf(buf_write, "[from %d] %s", cs, e->fds[cs].buf_read);
		sprintf(buf_write, "%s", e->fds[cs].buf_read);
        //send(i, e->fds[cs].buf_read, r, 0);
		//e->fds[i].write_queue.push(std::string(buf_write));
		//e->fds[i].write_string.append(std::string(buf_write)); //change to commandmap
		e->command[i].append(std::string(buf_write));

		std::string delimiter = DELIMITER;
		std::string token;
		size_t pos = 0;
		while ((pos = e->command[i].find(delimiter)) != std::string::npos) {
			token = e->command[i].substr(0, pos + delimiter.length());
			token = std::string("[from" + NumberToString(cs) + "]").append(token);
			e->fds[i].write_queue.push(token);
			e->command[i].erase(0, pos + delimiter.length());
		}

/*
		while ((pos = e->fds[i].write_string.find(delimiter)) != std::string::npos) {
			token = e->fds[i].write_string.substr(0, pos + delimiter.length());
			token = std::string("[from" + NumberToString(cs) + "]").append(token);
			e->fds[i].write_queue.push(token);
			e->fds[i].write_string.erase(0, pos + delimiter.length());
		}
*/






	  }
      i++;
    }
	//sprintf(e->fds[cs].buf_write, "%s", e->fds[cs].buf_read);
  }
}
