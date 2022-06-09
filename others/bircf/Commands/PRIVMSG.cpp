#include "../MessageManager.hpp"

void MessageManager::PRIVMSG(int cs, std::vector<std::string> paramsVec, std::string trailing) {
	if (paramsVec.size() != 1) {
		reply(cs, ERR_NORECIPIENT, "PRIVMSG", paramsVec, trailing);
		return;
	}
	if (trailing.length() == 0) {
		reply(cs, ERR_NOTEXTTOSEND, "PRIVMSG", paramsVec, trailing);
		return;
	}
	std::string msgtarget = paramsVec[0];
	if (msgtarget[0] != '#' &&
		nickFdPair_.find(msgtarget) == nickFdPair_.end()) {
		reply(cs, ERR_NOSUCHNICK, "PRIVMSG", paramsVec, trailing);
		return;
	}
	if (msgtarget[0]  == '#' &&
		channels_.find(msgtarget) == channels_.end()) {
		reply(cs, ERR_CANNOTSENDTOCHAN, "PRIVMSG", paramsVec, trailing);
		return;
	}


/*
ERR_NOTOPLEVEL
ERR_WILDTOPLEVEL
ERR_TOOMANYTARGETS
RPL_AWAY
*/



  int toFd;
  try {
    toFd = std::stoi(paramsVec[0]);
  }
  catch (const std::exception &e) {
    // do something with errcode errcode:errstr map
    return;
  }

  std::map<int, User>::iterator uit = users_.find(toFd);
  if (uit == users_.end()) {
    // do something with errcode errcode:errstr map
    return;
  }

  trailing = std::string("[from " + SS::toString(cs) + "(" + users_[cs].nick + ")" + ", to " + paramsVec[0] + "]").append(trailing).append(NEWLINE);
  outMessages_[toFd].append(trailing);
}
