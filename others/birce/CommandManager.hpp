#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include "StringUtils.hpp"
//#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <map>
#include <queue>

class CommandManager {

  private:

	std::map<int, std::string>	in_commands_;
	std::map<int, std::string>	out_commands_;
	t_fd						*fds_;
	int							maxfd_;

	typedef void(CommandManager::*FuncPtr)(int cs, std::string command);
	std::map<std::string, FuncPtr>	functionCallMap;
	void							registerFunctions() {
		functionCallMap["PRIVMSG"] =	&CommandManager::PRIVMSG;
		functionCallMap["SELFMSG"] =	&CommandManager::SELFMSG;
		functionCallMap["PUBLICMSG"] =	&CommandManager::PUBLICMSG;
	}
	void							PRIVMSG(int cs, std::string command) {
		std::vector<std::string> commandVec = SS::splitString(command, SPACE);
		if (commandVec.size() != 3) {
			//do something with errcode errcode:errstr map
			return;
		}

		int toFd;
		try {
			toFd = std::stoi(commandVec[1]);
		} catch (const std::exception &e) {
			//do something with errcode errcode:errstr map
			return;
		}

		if (fds_[toFd].type != FD_CLIENT) {
			//do something with errcode errcode:errstr map
			return;
		}
		std::string msg;
		msg = std::string("[from " + SS::toString(cs) + ", to " + commandVec[1] + "]").append(commandVec[2]).append(NEWLINE);
		if (fds_[toFd].type == FD_CLIENT)
			out_commands_[toFd].append(msg);
	}
	void							SELFMSG(int cs, std::string command) {
		std::string msg;
		msg = std::string("[from myself, " + SS::toString(cs) + "]").append(command).append(NEWLINE);
		if (fds_[cs].type == FD_CLIENT)
			out_commands_[cs].append(msg);
	}
	void							PUBLICMSG(int cs, std::string command) {
		std::string msg;
		msg = std::string("[from" + SS::toString(cs) + "]").append(command).append(NEWLINE);
		int i = 0;
		while (i < maxfd_) {
			if ((fds_[i].type == FD_CLIENT) && (i != cs)) {
				out_commands_[i].append(msg);
			}
			i++;
		}
	}

	CommandManager(CommandManager const &other);
	CommandManager &operator=(CommandManager const &other);

  public:

	CommandManager() { registerFunctions(); }

	CommandManager(t_fd *fds, int maxfd) { link2fds(fds, maxfd); }
	~CommandManager() {
		in_commands_.clear();
		out_commands_.clear();
	}

	void						link2fds(t_fd *fds, int maxfd) {
		fds_ = fds;
		maxfd_ = maxfd;
	}

	void	clean_fd(int cs) {
		fds_[cs].type = FD_FREE;
		//fds_[cs].fct_read = NULL;
		//fds_[cs].fct_write = NULL;
	}

	std::map<int, std::string>	&in_commands() { return in_commands_; }
	std::map<int, std::string>	&out_commands() { return out_commands_; }
	std::size_t					in_size() const { return in_commands_.size(); }
	std::size_t					out_size() const { return out_commands_.size(); }

	int							append_and_execute(int cs, std::string command) {
		try {
			append(cs, command);
			executeCommands(cs);
			return 0;
		} catch (const std::exception &e) {
			std::cerr << "append_and_execute() failed on " << cs << "'s command:" << command << " because " << e.what() << std::endl;
			return -1;
		}
	}

	void						append(int cs, std::string command) {
		if (command.empty())
			return;
		in_commands_[cs].append(command);
	}

	void						executeCommands(int cs) {

		std::vector<std::string> commandLines = splitCommands(cs);
		std::string command;

		while(commandLines.size()) {
			command = commandLines.front();
			executeCommand(cs, command);
			commandLines.erase(commandLines.begin()); //in case of vector
			//commandLines.pop(); 					  //in case of queue
		}
	}

	std::vector<std::string>		splitCommands(int cs, bool bSkipLast = true, bool bClearCommands = true) {
		return SS::splitString(in_commands_[cs], NEWLINE, bSkipLast, bClearCommands);
	}

	void						executeCommand(int cs, std::string command) {

		std::vector<std::string> commandVec = SS::splitString(command, SPACE);

		if (commandVec.empty())
			return;
		//get the first word from the command string: commandName
		std::string commandName = SS::toUpper(commandVec.front());

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
		std::map<std::string, FuncPtr>::iterator it = functionCallMap.find(commandName);
		if (it != functionCallMap.end())
			(this->*functionCallMap[commandName])(cs, command);
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
		clean_fd(cs);
		fds_[cs].type = FD_CLIENT;
		//fds_[cs].fct_read = client_read;
		//fds_[cs].fct_write = client_write;
	}

	void	client_read(int cs) {
		int		r;
		char	buf_read[BUF_SIZE + 1];

		r = recv(cs, buf_read, BUF_SIZE, 0);
		if (r <= 0) {
			close(cs);
			clean_fd(cs);
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

		if (out_commands_[cs].empty())
			return;

		int buf_size;
		while (out_commands_[cs].length()) {

			if (out_commands_[cs].length() > BUF_SIZE)
				buf_size = BUF_SIZE;
			else
				buf_size = out_commands_[cs].length();

			sprintf(buf_write, "%s", out_commands_[cs].substr(0, buf_size).c_str());
			buf_write[buf_size] = 0;
			send(cs, buf_write, buf_size, 0);
			out_commands_[cs].erase(0, buf_size);
		}
	}

};

/*
std::ostream& operator<<(std::ostream& out, const CommandManager& c) {
	return out << "CommandManager (map size=" << c.size() << ")";
}
*/

#endif
