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

		case	ERR_NOSUCHNICK				:	sVec.push_back("<nickname>");			rVec.push_back(paramsVec[0]);
																	break;	//401
		case	ERR_CANNOTSENDTOCHAN	:	sVec.push_back("<channel name>");	rVec.push_back(paramsVec[0]);
																	break;	//404

		case	ERR_NORECIPIENT				:	sVec.push_back("<command>");			rVec.push_back(command);
																	break;	//411
		case	ERR_NOTEXTTOSEND			:	break;	//412


		case	ERR_NONICKNAMEGIVEN		:	break;	//431
		case	ERR_ERRONEUSNICKNAME	:	sVec.push_back("<nick>");					rVec.push_back(paramsVec[0]);
																	break;	//432
		case	ERR_NICKNAMEINUSE			:	sVec.push_back("<nick>");					rVec.push_back(paramsVec[0]);
																	break;	//433

		//case	ERR_NICKCOLLISION		  :	break;	//436 not in the scope <-server to server

		case	ERR_UNAVAILRESOURCE		:	sVec.push_back("<nick/channel>");	rVec.push_back(paramsVec[0]);
																	break;	//437
		//work start

		//work end

		case	ERR_NEEDMOREPARAMS		:	sVec.push_back("<command>");			rVec.push_back(command);
																	break;	//461

		case	ERR_ALREADYREGISTRED	:	break;	//462
		case	ERR_NOPERMFORHOST			:	break;	//463
		case	ERR_PASSWDMISMATCH		:	break;	//464
		case	ERR_YOUREBANNEDCREEP	:	break;	//465
		case	ERR_YOUWILLBEBANNED		:	break;	//466
		//467		=	<channel> :Channel key already set
		case	ERR_KEYSET						:	sVec.push_back("<channel>");	rVec.push_back(paramsVec[0]);
																	break;	//467


		//471		=	<channel> :Cannot join channel (+l)
		case	ERR_CHANNELISFULL			:	sVec.push_back("<channel>");	rVec.push_back(paramsVec[0]);
																	break;	//471
		//472		=	<char> :is unknown mode char to me for <channel>
		case	ERR_UNKNOWNMODE				:	//sVec.push_back("<char>");	rVec.push_back(??);
																	//sVec.push_back("<channel>");	rVec.push_back(??);
																	break;	//472
		//473		=	<channel> :Cannot join channel (+i)
		case	ERR_INVITEONLYCHAN		:	sVec.push_back("<channel>");	rVec.push_back(paramsVec[0]);
																	break;	//473
		//474		=	<channel> :Cannot join channel (+b)
		case	ERR_BANNEDFROMCHAN		:	sVec.push_back("<channel>");	rVec.push_back(paramsVec[0]);
																	break;	//474
		//475		=	<channel> :Cannot join channel (+k)
		case	ERR_BADCHANNELKEY			:	sVec.push_back("<channel>");	rVec.push_back(paramsVec[0]);
																	break;	//475
		//476		=	<channel> :Bad Channel Mask
		case	ERR_BADCHANMASK				:	sVec.push_back("<channel>");	rVec.push_back(paramsVec[0]);
																	break;	//476
		//477		=	<channel> :Channel does not support modes
		case	ERR_NOCHANMODES				:	sVec.push_back("<channel>");	rVec.push_back(paramsVec[0]);
																	break;	//477
		//478		=	<channel> <char> :Channel list is full
		case	ERR_BANLISTFULL				:	sVec.push_back("<channel>");	rVec.push_back(paramsVec[0]);
																	break;	//478


		//481		=	:Permission Denied- You are not an IRC operator
		case	ERR_NOPRIVILEGES			:	break;	//481
		//482		=	<channel> :You are not channel operator
		case	ERR_CHANOPRIVSNEEDED	:	//sVec.push_back("<channel>");	rVec.push_back(??);
																	break;	//482
		//483		=	:You can not kill a server!
		case	ERR_CANTKILLSERVER		:	break;	//483
		//484		=	:Your connection is restricted!
		case	ERR_RESTRICTED				:	break;	//484
		//485		=	:You are not the original channel operator
		case	ERR_UNIQOPPRIVSNEEDED	:	break;	//485


		//491		=	:No O-lines for your host
		case	ERR_NOOPERHOST				:	break;	//491


		//501		=	:Unknown MODE flag
		case	ERR_UMODEUNKNOWNFLAG	:	break;	//501
		//502		=	:Cannot change mode for other users
		case	ERR_USERSDONTMATCH		:	break;	//502


		default	:											break;
	}

	SS::replaceString(msg, sVec, rVec);
	msg = std::string("").append(msg).append(NEWLINE);
	outMessages_[cs].append(msg);
}


