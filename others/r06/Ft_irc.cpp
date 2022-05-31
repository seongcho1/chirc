#include "Ft_irc.hpp"


Ft_irc::Ft_irc() {
	std::cout << "Ft_irc: This looks like another boring day." << std::endl;
}

Ft_irc::Ft_irc(unsigned long addr, unsigned short  port) {
	std::cout << "Ft_irc: This looks like another boring day." << std::endl;
	init(addr, port);
}



//Ft_irc(Ft_irc const &other) {}
//Ft_irc &operator=(Ft_irc const &other) {}

Ft_irc::~Ft_irc() {
	std::cout << "FT_irc	: My job here is done!" << std::endl;

	for (iterator it = userMap.begin(); it != userMap.end(); ++it)
		delete it->second;
	userMap.clear();
}

void	Ft_irc::init(unsigned long addr, unsigned short port) {

	//socket
    bzero(&servaddr_, sizeof(servaddr_));
    servaddr_.sin_family = AF_INET;
	servaddr_.sin_addr.s_addr = htonl(addr);
	servaddr_.sin_port = htons(port);

	//bind
    if ((ircfd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        fatal();

    if (bind(ircfd_, (const struct sockaddr *)&servaddr_, sizeof(servaddr_)) < 0)
        fatal();
}

void	Ft_irc::fatal() {
	std::cerr << "Fatal error" << std::endl;
	//shutdown(ircfd_, 2); //Further sends and receives are disallowed
	close(ircfd_);
	this->~Ft_irc();
	exit(1);
}

void	Ft_irc::loop() {

	if (listen(ircfd_, 0) < 0)
		fatal();

	FD_ZERO(&curr_sock_);
	FD_SET(ircfd_, &curr_sock_);
	bzero(&tmp_, sizeof(tmp_));
	bzero(&buf_, sizeof(buf_));
	bzero(&str_, sizeof(str_));


	while(42)
	{
		cpy_write_ = cpy_read_ = curr_sock_;
		if (select(getMaxUserFd() + 1, &cpy_read_, &cpy_write_, NULL, NULL) < 0)
			continue;
		for (int fd = 0; fd <= getMaxUserFd(); fd++) {

			if (FD_ISSET(fd, &cpy_read_)) {
				if (fd == ircfd_) {
					bzero(&msg_, sizeof(msg_));
					connectUser();
					break;

				} else {

					int ret_recv = 1000;
					while (ret_recv == 1000 || str_[strlen(str_) - 1] != '\n') {
						ret_recv = recv(fd, str_ + strlen(str_), 1000, 0);
						if (ret_recv <= 0)
							break ;
					}

					if (ret_recv <= 0) {
						bzero(&msg_, sizeof(msg_));
						sprintf(msg_, "server: client %d just left\n", delUser(fd));
						sendToAll(fd, msg_);
						FD_CLR(fd, &curr_sock_);
						close(fd);
						break;

					} else {
						exchangeMsg(fd);
					}
				}
			}

		} //for loop

	} //while loop
}


void	Ft_irc::sendToAll(int fd, char *str_req) {

	std::cout << "[ircid=" << ircfd_ << "]";
	for (iterator it = userMap.begin(); it != userMap.end(); ++it) {
		User *user = it->second;

		std::cout << "[id=" << user->id() << ", fd=" << user->fd() << "]";

		//except myself, send msg to the otehrs
		if (user->fd() != fd && FD_ISSET(user->fd(), &cpy_write_)) {
			if (send(user->fd(), str_req, strlen(str_req), 0) < 0)
				fatal();
		}
	}
	std::cout << str_req;
}

void	Ft_irc::exchangeMsg(int fd)
{

    int i = 0;
    int j = 0;

    while (str_[i])
    {
        tmp_[j] = str_[i];
        j++;
        if (str_[i] == '\n')
        {
            sprintf(buf_, "client %d: %s", getUserId(fd), tmp_);
            sendToAll(fd, buf_);
            j = 0;
            bzero(&tmp_, strlen(tmp_));
            bzero(&buf_, strlen(buf_));
        }
        i++;
    }
    bzero(&str_, strlen(str_));

}


int		Ft_irc::getMaxUserFd()
{
	int	maxfd = ircfd_;

	for (iterator it = userMap.begin(); it != userMap.end(); ++it) {
		User *user = it->second;

		if (user->fd() > maxfd)
			maxfd = user->fd();
	}
	return maxfd;
}

int		Ft_irc::getUserId(int const &userfd) {

	for (iterator it = userMap.begin(); it != userMap.end(); ++it) {
		User *user = it->second;
		if (user->fd() == userfd)
			return user->id();
	}
	return -1;
}

void Ft_irc::connectUser()
{
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	int client_fd;

	if ((client_fd = accept(ircfd_, (struct sockaddr *)&clientaddr, &len)) < 0)
		fatal();
	sprintf(msg_, "server: client %d just arrived\n", addUser(client_fd));
	sendToAll(client_fd, msg_);
	FD_SET(client_fd, &curr_sock_);

}

int		Ft_irc::addUser(int const &userfd) {

	User *user = new User(userfd);
	userMap.insert(pair(
				user->id(), user));
	return user->id();
}

int		Ft_irc::delUser(int const &userfd) {

	int userid = getUserId(userfd);
	if (userid < 0) return -1;

	iterator it = userMap.find(userid);
	if (it != userMap.end()) {
		delete it->second;
		userMap.erase(userid);
		return userid;
	}
	return -1;
}

/*
void Ft_irc::launchSpell(std::string str, ATarget const &target) {
	ATarget *nullTarget = NULL;
	if (nullTarget == &target)
		return;
	ASpell *spell = inventory[str];
	if (spell)
		spell->launch(target);
}
*/
