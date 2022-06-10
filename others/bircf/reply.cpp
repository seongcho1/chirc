#include "MessageManager.hpp"
#include <fstream>
#include <string>

int		MessageManager::initReplies(std::string configFile) {
	(void)configFile;

	std::ifstream fileStream(configFile);

	if ( fileStream.fail() )
		return -1;

	std::string line;
	while (std::getline(fileStream, line))	{
		std::istringstream is_line(line);

		std::string valueLine;
		if (!std::getline(is_line, valueLine, '#'))
			continue;

		std::string key;
		std::string value;

		std::vector<std::string> valueVec = SS::splitString(valueLine, "=", false, false, true);
		if (valueVec.size() != 2)
			continue;

		key = SS::trim(valueVec[0]);
		value = SS::trim(valueVec[1]);

		replies_[std::stoi(key)] = value;
	}
	return 0;
}

void	MessageManager::reply(int cs, int code, std::string command, std::vector<std::string> paramsVec, std::string trailing) {

	//(void)cs;
	//(void)code;
	//(void)command;
	(void)paramsVec;
	(void)trailing;

	std::map<int, std::string>::iterator it = replies_.find(code);
	if (it == replies_.end()) {
		return;
	}

	std::string msg = it->second;
	std::vector<std::string>			sVec;
	std::vector<std::string>			rVec;

	switch (code) {

		case	ERR_NOSUCHNICK			:	sVec.push_back("<nickname>");		rVec.push_back(paramsVec[0]);
											break;	//401
		case	ERR_CANNOTSENDTOCHAN	:	sVec.push_back("<channel name>");	rVec.push_back(paramsVec[0]);
											break;	//404

		case	ERR_NORECIPIENT			:	sVec.push_back("<command>");		rVec.push_back(command);
											break;	//411
		case	ERR_NOTEXTTOSEND			:
											break;	//412

		case	ERR_NEEDMOREPARAMS		:	sVec.push_back("<command>");		rVec.push_back(command);
											break;	//461

		case	ERR_ALREADYREGISTRED	:
											break;	//462

		default	:						break;
	}

	SS::replaceString(msg, sVec, rVec);
	msg = std::string("").append(msg).append(NEWLINE);
	outMessages_[cs].append(msg);
}


