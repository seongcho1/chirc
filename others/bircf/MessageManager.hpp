#ifndef MessageManager_HPP
#define MessageManager_HPP

#include "StringUtils.hpp"
//#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <vector>
#include <map>

class MessageManager {

  private:
	std::map<int, User>			userMap_;
	std::map<int, std::string>	in_messages_;
	std::map<int, std::string>	out_messages_;
	//t_fd						*fds_;
	int							maxfd_;

	typedef void(MessageManager::*FuncPtr)(int cs, std::vector<std::string> paramsVec, std::string trailing);
	std::map<std::string, FuncPtr>	functionCallMap_;
	void							registerFunctions() {
		functionCallMap_["PRIVMSG"] =	&MessageManager::PRIVMSG;
		functionCallMap_["SELFMSG"] =	&MessageManager::SELFMSG;
		functionCallMap_["PUBLICMSG"] =	&MessageManager::PUBLICMSG;
	}
	void						PRIVMSG(int cs, std::vector<std::string> paramsVec, std::string trailing) {
		if (paramsVec.size() != 1) {
			//do something with errcode errcode:errstr map
			return;
		}
		if (trailing.length() == 0) {
			//do something with errcode errcode:errstr map
			return;
		}

		int toFd;
		try {
			toFd = std::stoi(paramsVec[0]);
		} catch (const std::exception &e) {
			//do something with errcode errcode:errstr map
			return;
		}

		std::map<int, User>::iterator uit = userMap_.find(toFd);
		if (uit == userMap_.end()) {
			//do something with errcode errcode:errstr map
			return;
		}

		trailing = std::string("[from " + SS::toString(cs) + ", to " + paramsVec[0] + "]").append(trailing).append(NEWLINE);
		out_messages_[toFd].append(trailing);
	}
	void						SELFMSG(int cs, std::vector<std::string> paramsVec, std::string trailing) {
		if (paramsVec.size() != 0) {
			//do something with errcode errcode:errstr map
			return;
		}
		if (trailing.length() == 0) {
			//do something with errcode errcode:errstr map
			return;
		}
		std::map<int, User>::iterator uit = userMap_.find(cs);
		if (uit == userMap_.end()) {
			//do something with errcode errcode:errstr map
			return;
		}

		trailing = std::string("[from myself, " + SS::toString(cs) + "]").append(trailing).append(NEWLINE);
		out_messages_[cs].append(trailing);
	}
	void						PUBLICMSG(int cs, std::vector<std::string> paramsVec, std::string trailing) {
		if (paramsVec.size() != 0) {
			//do something with errcode errcode:errstr map
			return;
		}
		if (trailing.length() == 0) {
			//do something with errcode errcode:errstr map
			return;
		}
		std::map<int, User>::iterator uit = userMap_.find(cs);
		if (uit == userMap_.end()) {
			//do something with errcode errcode:errstr map
			return;
		}

		trailing = std::string("[from" + SS::toString(cs) + "]").append(trailing).append(NEWLINE);
		for (uit = userMap_.begin(); uit != userMap_.end(); ++uit) {
			if (uit->first != cs)
				out_messages_[uit->first].append(trailing);
		}
	}

	MessageManager(MessageManager const &other);
	MessageManager &operator=(MessageManager const &other);

  public:

	MessageManager() {
		registerFunctions();
	}
/*
	MessageManager(t_fd *fds, int maxfd) {
		registerFunctions();
		link2fds(fds, maxfd);
	}
*/
	~MessageManager() {

		userMap_.clear();
		in_messages_.clear();
		out_messages_.clear();
	}
/*
	void						link2fds(t_fd *fds, int maxfd) {
		//fds_ = fds;
		maxfd_ = maxfd;
	}
*/
	void	clean_fd(int cs) {
		// fds_[cs].type = FD_FREE;
		// //fds_[cs].fct_read = NULL;
		// //fds_[cs].fct_write = NULL;

		//delete User * from userMap_
		std::map<int, User>::iterator uit = userMap_.find(cs);
		if (uit != userMap_.end())
			userMap_.erase(cs);

		//delete from in_messages_
		std::map<int, std::string>::iterator cit = in_messages_.find(cs);
		if (cit != in_messages_.end())
			in_messages_.erase(cs);

		//delete from out_messages_
		cit = out_messages_.find(cs);
		if (cit != out_messages_.end())
			out_messages_.erase(cs);
	}

	std::map<int, User>			&userMap() { return userMap_; }
	std::map<int, std::string>	&in_messages() { return in_messages_; }
	std::map<int, std::string>	&out_messages() { return out_messages_; }
	std::size_t					in_size() const { return in_messages_.size(); }
	std::size_t					out_size() const { return out_messages_.size(); }

	int							append_and_execute(int cs, std::string message) {
		try {
			append(cs, message);
			executeMessages(cs);
			return 0;
		} catch (const std::exception &e) {
			std::cerr << "append_and_execute() failed on " << cs << "'s message:" << message << " because " << e.what() << std::endl;
			return -1;
		}
	}

	void						append(int cs, std::string message) {
		if (message.empty())
			return;
		in_messages_[cs].append(message);
	}

	void						executeMessages(int cs) {

		std::vector<std::string> messageVec = splitMessages(cs);
		std::string message;

		while(messageVec.size()) {
			message = messageVec.front();
			executeMessage(cs, message);
			messageVec.erase(messageVec.begin()); //in case of vector
			//messageVec.pop(); 					//in case of queue
		}
	}

	std::vector<std::string>		splitMessages(int cs, bool bSkipLast = true, bool bClearMessages = true) {
		return SS::splitString(in_messages_[cs], NEWLINE, bSkipLast, bClearMessages);
	}

	//tried to use terms in https://datatracker.ietf.org/doc/html/rfc2812#section-2.3.1
	void						executeMessage(int cs, std::string message) {

		std::string command_params, trailing;
		std::vector<std::string> command_params_trailing = SS::splitString(message, SPACE_COLON, false, false, true);
		if (command_params_trailing.empty())
			return;
		switch (command_params_trailing.size()) {
			case 1: command_params = command_params_trailing[0];
					trailing = "";
					break;
			case 2: command_params = command_params_trailing[0];
					trailing = command_params_trailing[1];
					break;
			default:
					return;
					break;
		}

		std::string command;
		std::vector<std::string> paramsVec = SS::splitString(command_params, SPACE);
		if (paramsVec.empty())
			return;
		//get the commandName which is the first element in the argsVec
		command = SS::toUpper(paramsVec.front());
		//then remove the first element and the rest should be args
		paramsVec.erase(paramsVec.begin());

		//change this pattern to using a map<commandName, commandFunc>
		//execute commandMap[command](cs, command)
		/*
		if (commandName.compare("PRIVMSG") == 0)
			PRIVMSG(cs, command);
		else if (commandName.compare("SELFMSG") == 0)
			SELFMSG(cs, command);
		else if (commandName.compare("PUBLICMSG") == 0)
			PUBLICMSG(cs, command);
		*/
		std::map<std::string, FuncPtr>::iterator it = functionCallMap_.find(command);
		if (it != functionCallMap_.end())
			(this->*functionCallMap_[command])(cs, paramsVec, trailing);
	}

	void			srv_accept(int s) {
		int			cs;
		struct sockaddr_in	csin;
		socklen_t		csin_len;

		csin_len = sizeof(csin);
		cs = accept(s, (struct sockaddr*)&csin, &csin_len);

		fcntl(cs, F_SETFL, O_NONBLOCK);

		std::cout << "New client #" << cs << " from "
					<< inet_ntoa(csin.sin_addr) << ":"
					<< ntohs(csin.sin_port) << std::endl;

		// clean_fd(cs);
		// fds_[cs].type = FD_CLIENT;
		// //fds_[cs].fct_read = client_read;
		// //fds_[cs].fct_write = client_write;
		userMap_.insert(std::pair<int, User>(cs, User()));
	}

	void	client_read(int cs) {
		int		r;
		char	buf_read[BUF_SIZE + 1];

		r = recv(cs, buf_read, BUF_SIZE, 0);
		if (r <= 0) {
			clean_fd(cs); //del User *, in_messages_, out_commands
			close(cs); //cleaning the table first, and then the table will be ready for another client

			std::cout << "client #" << cs << " gone away" << std::endl;
		} else {
			if (r == BUF_SIZE)
				buf_read[r-1] = NEWLINE[0];
			buf_read[r] = 0;
			append_and_execute(cs, std::string(buf_read));
		}
	}

	void	client_write(int cs) {
		//send(cs, e->fds[cs].buf_write, strlen(e->fds[cs].buf_write), 0);
		//e->fds[cs].buf_write[0] = 0;

		//if the size of out_commands is larger than buffer size
		//divide and conquer

		char	buf_write[BUF_SIZE + 1];

		if (out_messages_[cs].empty())
			return;

		int buf_size;
		while (out_messages_[cs].length()) {

			if (out_messages_[cs].length() > BUF_SIZE)
				buf_size = BUF_SIZE;
			else
				buf_size = out_messages_[cs].length();

			sprintf(buf_write, "%s", out_messages_[cs].substr(0, buf_size).c_str());
			buf_write[buf_size] = 0;
			send(cs, buf_write, buf_size, 0);
			out_messages_[cs].erase(0, buf_size);
		}
	}

};

/*
std::ostream& operator<<(std::ostream& out, const MessageManager& c) {
	return out << "MessageManager (map size=" << c.size() << ")";
}
*/

#endif
