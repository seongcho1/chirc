#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <iostream>
#include <sstream>
#include <map>
#include <queue>

#define NEWLINE		"\n"
#define SPACE		" "


template <typename T>
std::string NumberToString ( T number ) {
	std::ostringstream ss;
	ss << number;
	return ss.str();
}

class CommandManager {

  private:

	std::map<int, std::string>	commands_;
	t_fd						*fds_;
	int							maxfd_;
  public:

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
		commands_[cs].append(command);
	}

	void						executeCommands(int cs) {

		std::queue<std::string> commandLines = splitCommands(cs);
		std::string command;

		while(commandLines.size()) {
			command = commandLines.front();
			executeCommand(cs, command);
			commandLines.pop();
		}
	}

	std::queue<std::string>		splitCommands(int cs) {
		std::queue<std::string> commandLines;
		std::string delimiter = NEWLINE;
		std::string token;
		size_t pos = 0;

		while ((pos = commands_[cs].find(delimiter)) != std::string::npos) {
			std::string &command = commands_[cs];
			token = command.substr(0, pos);// + delimiter.length());
			commandLines.push(token);
			command.erase(0, pos + delimiter.length());
		}
		return commandLines;
	}

	void						executeCommand(int cs, std::string command) {
		//get the first word from the command string <- command
		//execute commandMap[command]

		std::string msg;
		msg = std::string("[from" + NumberToString(cs) + "]").append(command).append("\n");
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
