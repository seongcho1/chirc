#ifndef FD_HPP
#define FD_HPP

#include <iostream>
#include <queue>

#define FD_FREE		0
#define FD_SERV		1
#define FD_CLIENT	2

#define BUF_SIZE	512 //4096

// typedef struct		s_fd {
// 	int							type;
// //	void						(*fct_read)(struct s_env *, int);
// //	void						(*fct_write)(struct s_env *, int);
// }					t_fd;

class User {
 	//int		type_;

  public:
	User() {}
	//User(int type) : type_(type) {}
	//int	const &type() const { return type_; }
};

#endif
