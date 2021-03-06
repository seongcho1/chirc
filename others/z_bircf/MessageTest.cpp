#include "StringUtils.hpp"
#include <map>

#define NEWLINE		"\n"
#define BUF_SIZE	10

class MessageManager {
  private:
	typedef void(MessageManager::*FuncPtr)(std::string command);
	std::map<std::string, FuncPtr> functionCallMap;
	void PRIVMSG(std::string command) { std::cout << "[PRIVMSG]" << command << std::endl; }
	void SELFMSG(std::string command) { std::cout << "[SELFMSG]" << command << std::endl; }
	void PUBLICMSG(std::string command) { std::cout << "[PUBLICMSG]" << command << std::endl; }

  public:
	MessageManager() {
		functionCallMap["PRIVMSG"] = &MessageManager::PRIVMSG;
		functionCallMap["SELFMSG"] = &MessageManager::SELFMSG;
		functionCallMap["PUBLICMSG"] = &MessageManager::PUBLICMSG;
	}

	void executeCommand (std::string command) {
		(this->*functionCallMap[command])(command);
	}
};

int clientWriteTest() {

	std::string out_commands = "aaa\n";;
	std::string token;
	char	buf_write[BUF_SIZE + 1];

	int buf_size;
	while (out_commands.length()) {
		if (out_commands.length() > BUF_SIZE)
			buf_size = BUF_SIZE;
		else
			buf_size = out_commands.length();
		//token = out_commands.substr(0, BUF_SIZE);
		//std::cout << token;
		sprintf(buf_write, "%s", out_commands.substr(0, buf_size).c_str());
		buf_write[buf_size] = 0;
		std::cout << buf_write;
		//send(cs, token.c_str(), token.length(), 0);
		out_commands.erase(0, buf_size);
	}
	return 0;
}

int splitSstringTest() {

	int i;
	std::string a1 = "\n\n\naaa\nbbb\nccc\nddd";
	std::string a2 = "aaaaa   bbbb :5 z     2 :1    a b c   e";

	std::vector<std::string> avec1 = SS::splitString(a1, NEWLINE, true, true);

	std::cout << "vec size=" << avec1.size() << std::endl;
	i = 0;
	while (i < (int) avec1.size()) {
		std::cout << "(" << avec1[i] << ")->";
		i++;
	}
	std::cout << std::endl << "a1=(" << a1 << ")" << std::endl;


	std::vector<std::string> avec2 = SS::splitString(a2, SPACE_COLON, false, true, false);

	std::cout << "vec size=" << avec2.size() << std::endl;
	i = 0;
	while (i < (int) avec2.size()) {
		std::cout << "(" << avec2[i] << ")->";
		i++;
	}
	std::cout << std::endl << "a2=(" << a2 << ")" << std::endl;

	return 0;
}


int toUpperTest() {
	std::string a = "a{|}^z";
	a = SS::toUpper(a);
	std::cout << a << std::endl;

	return 0;
}

int main() {

	//MessageManager c;
	//c.executeCommand("PRIVMSG");

	//clientWriteTest();
	splitSstringTest();
	//toUpperTest();


}
