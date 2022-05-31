#ifndef User_HPP
#define User_HPP

#include <iostream>

class User {

  private:
	static int	last_id_;
	int		id_;
	int		fd_;
	std::string nick_;
	std::string user_;
	//nick!user@domain.com
	User();

  public:

	int	const	&fd(void) const { return fd_; }
	int	const	&id(void) const { return id_; }
	//getters and setters

	int			getNewId();

	bool hasNick();

	User(int const &fd);
	User(User const &other);
	User &operator=(User const &other);
	~User();

};

#endif
