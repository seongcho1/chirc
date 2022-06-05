#ifndef FD_HPP
#define FD_HPP

#include <iostream>
#include <queue>

#define FD_FREE		0
#define FD_SERV		1
#define FD_CLIENT	2

#define BUF_SIZE	4096

typedef struct		s_fd
{
	int							type;
	void						(*fct_read)(struct s_env *, int);
	void						(*fct_write)(struct s_env *, int);
	char						buf_read[BUF_SIZE + 1];
	std::queue<std::string>		write_queue;

}					t_fd;

#endif
