#ifndef BIRCD_H_
# define BIRCD_H_

# include <sys/select.h>
# include <iostream>
#include <sstream>
# include <queue>
# include <map>

# define FD_FREE	0
# define FD_SERV	1
# define FD_CLIENT	2

# define BUF_SIZE	4096
# define DELIMITER "\n"

# define Xv(err,res,str)	(x_void(err,res,str,(char *)__FILE__,__LINE__))
# define X(err,res,str)		(x_int(err,res,str,(char *)__FILE__,__LINE__))
# define MAX(a,b)	((a > b) ? a : b)

# define USAGE		"Usage: %s port\n"




template <typename T>
std::string NumberToString ( T number ) {
	std::ostringstream ss;
	ss << number;
	return ss.str();
}


typedef struct	s_fd
{
  int	type;
  void	(*fct_read)(struct s_env *, int);
  void	(*fct_write)(struct s_env *, int);
  char	buf_read[BUF_SIZE + 1];
  std::queue<std::string> write_queue;
//  std::string write_string;

}		t_fd;

typedef struct	s_env
{
  t_fd		*fds;
  int		port;
  int		maxfd;
  int		max;
  int		r;
  fd_set	fd_read;
  fd_set	fd_write;

  std::map<int, std::string> command;


}		t_env;

void	init_env(t_env *e);
void	get_opt(t_env *e, int ac, char **av);
void	main_loop(t_env *e);
void	srv_create(t_env *e, int port);
void	srv_accept(t_env *e, int s);
void	client_read(t_env *e, int cs);
void	client_write(t_env *e, int cs);
void	clean_fd(t_fd *fd);
int		x_int(int err, int res, char *str, char *file, int line);
void	*x_void(void *err, void *res, char *str, char *file, int line);
void	init_fd(t_env *e);
void	do_select(t_env *e);
void	check_fd(t_env *e);

#endif /* !BIRCD_H_ */
