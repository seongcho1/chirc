#ifndef BIRCD_HPP
# define BIRCD_HPP

# include "fd.hpp"
# include "CommandManager.hpp"
# include <sys/select.h>

# define Xv(err,res,str)	(x_void(err,res,str,(char *)__FILE__,__LINE__))
# define X(err,res,str)		(x_int(err,res,str,(char *)__FILE__,__LINE__))
# define MAX(a,b)	((a > b) ? a : b)

# define USAGE		"Usage: %s port\n"

typedef struct	s_env {
	t_fd			*fds;
	int				port;
	int				maxfd;
	int				max;
	int				r;
	fd_set			fd_read;
	fd_set			fd_write;

	CommandManager	commander;

}				t_env;

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
