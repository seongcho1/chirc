#include "MessageManager.hpp"

void	MessageManager::reply(int cs, int code, std::vector<std::string> paramsVec, std::string trailing) {


	std::map<int, std::string>			replies_;

	(void)cs;
	(void)code;
	(void)paramsVec;
	(void)trailing;







	trailing = std::string("[reply from server]").append("welcome, but no params").append(NEWLINE);
	outMessages_[cs].append(trailing);
}


