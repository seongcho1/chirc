#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include "StringUtils.hpp"
#include <map>
#include <queue>


class CommandManager {

  private:

	std::map<int, std::string>	commands_;
	t_fd						*fds_;
	int							maxfd_;
  public:

	int							hello() {
		return 42;
	}

	void						link2fds(t_fd *fds, int maxfd) {
		fds_ = fds;
		maxfd_ = maxfd;
	}
	std::map<int, std::string>	commands() { return commands_; }
	std::size_t					size() const { return commands_.size(); }

	void						push_and_execute(int cs, std::string command) {
		push(cs, command);
		executeCommands(cs);
	}

	void						push(int cs, std::string command) {
		if (command.empty())
			return;
		commands_[cs].append(command);
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
		return SS::splitString(commands_[cs], NEWLINE, bSkipLast, bClearCommands);
	}



	void						executeCommand(int cs, std::string command) {

		std::vector<std::string> commandVec = SS::splitString(command, SPACE);

		if (commandVec.empty())
			return;
		//get the first word from the command string: commandName
		std::string commandName = SS::toUpper(commandVec.front());

		//change this pattern to using a map<commandName, commandFunc>
		//execute commandMap[command](cs, command)
		if (commandName.compare("PRIVMSG") == 0)
			PRIVMSG(cs, command);
		else if (commandName.compare("SELFMSG") == 0)
			PUBLICMSG(cs, command);
		else if (commandName.compare("PUBLICMSG") == 0)
			PUBLICMSG(cs, command);
	}

	void						PRIVMSG(int cs, std::string command) {

		std::string msg;
		msg = std::string("[from myself, " + SS::toString(cs) + "]").append(command).append("\n");
		if (fds_[cs].type == FD_CLIENT)
			fds_[cs].write_queue.push(msg);
	}

	void						SELFMSG(int cs, std::string command) {

		std::string msg;
		msg = std::string("[from myself, " + SS::toString(cs) + "]").append(command).append("\n");
		if (fds_[cs].type == FD_CLIENT)
			fds_[cs].write_queue.push(msg);
	}

	void						PUBLICMSG(int cs, std::string command) {
		std::string msg;
		msg = std::string("[from" + SS::toString(cs) + "]").append(command).append("\n");
		int i = 0;
		while (i < maxfd_) {
			if ((fds_[i].type == FD_CLIENT) && (i != cs)) {
				fds_[i].write_queue.push(msg);
			}
			i++;
		}
	}

};


#endif
