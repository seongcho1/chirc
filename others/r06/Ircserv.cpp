#include "Ircserv.hpp"


Ircserv::Ircserv() {
	std::cout << "Ircserv: This looks like another boring day." << std::endl;
}

Ircserv::Ircserv(unsigned long addr, unsigned short  port) {
	std::cout << "Ircserv: This looks like another boring day." << std::endl;
	init(addr, port);
}



//Ircserv(Ircserv const &other) {}
//Ircserv &operator=(Ircserv const &other) {}

Ircserv::~Ircserv() {
	std::cout << "Ircserv	: My job here is done!" << std::endl;
	close(ircfd_);
	for (iterator it = userMap.begin(); it != userMap.end(); ++it)
		delete it->second;
	userMap.clear();
}

void	Ircserv::init(unsigned long addr, unsigned short port) {

	//socket
    bzero(&servaddr_, sizeof(servaddr_));
    servaddr_.sin_family = AF_INET;
	servaddr_.sin_addr.s_addr = htonl(addr);
	servaddr_.sin_port = htons(port);

	//bind
	int yes = 1;
    if ((ircfd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        fatal("init-socket");
	if (setsockopt(ircfd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)))
        fatal("init-setsocket");
	if (fcntl(ircfd_, F_SETFL, O_NONBLOCK) < 0)
		 fatal("init-fcntl");
    if (bind(ircfd_, (const struct sockaddr *)&servaddr_, sizeof(servaddr_)) < 0)
        fatal("init-bind");

	//listen
	if (listen(ircfd_, 0) < 0)
		fatal("init-listen");

	FD_ZERO(&curr_sock_);
	FD_SET(ircfd_, &curr_sock_);
	bzero(&tmp_, sizeof(tmp_));
	bzero(&buf_, sizeof(buf_));
	bzero(&str_, sizeof(str_));
}

void	Ircserv::fatal(std::string str) {
	std::cerr << "Fatal error" <<  " " << str << std::endl;
	//shutdown(ircfd_, 2); //Further sends and receives are disallowed
	this->~Ircserv();
	exit(1);
}

void	Ircserv::doInTheLoop() {


	cpy_write_ = cpy_read_ = curr_sock_;
	if (select(getMaxUserFd() + 1, &cpy_read_, &cpy_write_, NULL, NULL) < 0)
		return;
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

}


void	Ircserv::sendToAll(int fd, char *str_req) {

	std::cout << "[ircid=" << ircfd_ << "]";
	for (iterator it = userMap.begin(); it != userMap.end(); ++it) {
		User *user = it->second;

		std::cout << "[id=" << user->id() << ", fd=" << user->fd() << "]";

		//except myself, send msg to the otehrs
		if (user->fd() != fd && FD_ISSET(user->fd(), &cpy_write_)) {
			if (send(user->fd(), str_req, strlen(str_req), 0) < 0)
				fatal("sendToAll");
		}
	}
	std::cout << str_req;
}

void	Ircserv::exchangeMsg(int fd)
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


int		Ircserv::getMaxUserFd()
{
	reverse_iterator it = userMap.rbegin();

	if (it != userMap.rend())
		return it->first;
	return ircfd_;
}

int		Ircserv::getUserId(int const &userfd) {

	iterator it = userMap.find(userfd);

	if (it != userMap.end())
		return it->second->id();
	return -1;
}

void Ircserv::connectUser()
{
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	int userfd;

	if ((userfd = accept(ircfd_, (struct sockaddr *)&clientaddr, &len)) < 0)
		fatal("connectUser-accept");
	sprintf(msg_, "server: client %d just arrived\n", addUser(userfd));
	sendToAll(userfd, msg_);
	FD_SET(userfd, &curr_sock_);

}

int		Ircserv::addUser(int const &userfd) {

	User *user = new User(userfd);
	userMap.insert(pair(userfd, user));
	return user->id();
}

int		Ircserv::delUser(int const &userfd) {

	int userid;

	iterator it = userMap.find(userfd);
	if (it == userMap.end())
		return -1;

 	userid = it->second->id();
	delete it->second;
	userMap.erase(userfd);
	return userid;
}

/*
void Ircserv::launchSpell(std::string str, ATarget const &target) {
	ATarget *nullTarget = NULL;
	if (nullTarget == &target)
		return;
	ASpell *spell = inventory[str];
	if (spell)
		spell->launch(target);
}
*/
