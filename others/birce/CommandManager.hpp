#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <list>
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

		std::vector<std::string> commandLines = splitCommands(cs);
		std::string command;

		while(commandLines.size()) {
			command = commandLines.front();
			executeCommand(cs, command);
			commandLines.erase(commandLines.begin()); //in case of vector
			//commandLines.pop(); 					  //in case of queue
		}
	}

	std::vector<std::string>		splitCommands(int cs, bool bClearCommands = true) {
		return splitString(commands_[cs], NEWLINE, bClearCommands);
	}

	std::vector<std::string>		splitString(std::string &s, std::string delimiter, bool bClearCommands = false) {
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();
		std::string token;
		std::vector<std::string> result;

		while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
			token = s.substr(pos_start, pos_end - pos_start); //+ delimiter.length());
			pos_start = pos_end + delim_len;
			result.push_back(token);  //in case of vector
		}
		if (pos_start > 0 && pos_start < s.length())
			result.push_back(s.substr(pos_start));
		if (result.size() > 0 && bClearCommands == true)
			s.clear();
		return result;
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
