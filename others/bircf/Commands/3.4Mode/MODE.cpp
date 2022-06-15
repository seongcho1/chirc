#include "../../MessageManager.hpp"

void MessageManager::MODE(int cs, std::vector<std::string> paramsVec) {
  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "MODE", paramsVec);
    return;
  }

  std::string whatis = *paramsVec.begin();
  if (whatis[0] == '#')
    modeChannel(cs, paramsVec);
  else
    modeUser(cs, paramsVec);
}

void MessageManager::modeUser(int cs, std::vector<std::string> paramsVec) {

  if (users_[cs].nick != paramsVec[0]) {
    reply(cs, ERR_USERSDONTMATCH, "MODE", paramsVec);
    return;
  }

  if (paramsVec.size() < 2 || (paramsVec[1][0] != '-' && paramsVec[1][0] != '+')) {
    paramsVec[0] = ":" + users_[cs].currentMode();
    reply(cs, RPL_UMODEIS, "MODE", paramsVec);
    return;
  }


  if (paramsVec[1][0] == '-' && paramsVec[1][1] == 'i') {
// :gello2!~1@freenode-ca7.4sl.2765s3.IP MODE gello2 :-i
    users_[cs].setMode(false, 'i');
    announceOneUser(cs, std::string(users_[cs].cmdPrefix("MODE") + users_[cs].nick + " :-i"));
    return;
  }

  if (paramsVec[1][0] == '+' && paramsVec[1][1] == 'i') {
// :gello2!~1@freenode-ca7.4sl.2765s3.IP MODE gello2 :+i
    users_[cs].setMode(true, 'i');
    announceOneUser(cs, std::string(users_[cs].cmdPrefix("MODE") + users_[cs].nick + " :+i"));
    return;
  }
}

void MessageManager::modeChannel(int cs, std::vector<std::string> paramsVec) {

  if (channels_.find(paramsVec[0]) == channels_.end()) {
    reply(cs, ERR_NOSUCHCHANNEL, "MODE", paramsVec);
    return;
  }

  if (paramsVec.size() < 2 || (paramsVec[1][0] != '+' && paramsVec[1][0] != '-')) {
    paramsVec.push_back(":" + channels_[paramsVec[0]].currentMode());
    paramsVec.push_back("");
    if (channels_[paramsVec[0]].isMode('l')) paramsVec[2] += " limit=" + SS::toString(channels_[paramsVec[0]].limit);
    if (channels_[paramsVec[0]].isMode('k')) paramsVec[2] += " key=<" + channels_[paramsVec[0]].key + "> exclude outter <>";

    reply(cs, RPL_CHANNELMODEIS, "MODE", paramsVec);
    return;
  }

  if (channels_[paramsVec[0]].channelOperators.find(cs) == channels_[paramsVec[0]].channelOperators.end()) {
    reply(cs, ERR_CHANOPRIVSNEEDED, "MODE", paramsVec);
    return;
  }

  bool paramOnce = false;
  bool add = paramsVec[1][0] == '+';
  char loop = 0;
  for (std::string::iterator mit = paramsVec[1].begin(); mit != paramsVec[1].end(); ++mit) {
    
    if (*mit == '+' || *mit == '-')
      add = *mit++ == '+';
    
    if (mit == paramsVec[1].end())
      break;

    std::string msg;
    
// :gello2!~1@freenode-ca7.4sl.2765s3.IP MODE #2irc :+i
    msg.append(users_[cs].cmdPrefix("MODE")).append(paramsVec[0]).append(" ");
    if (SS::compare(*mit, CHN_M_FLAGS)) {
      channels_[paramsVec[0]].setMode(add, *mit);
      msg.append(":");
      msg += "-+"[add];
      msg += *mit;
      announceToChannel(cs, paramsVec[0], msg, true);
    }
    
    else if (!paramOnce && *mit == 'k' && 2 < (int)paramsVec.size()) {
      channels_[paramsVec[0]].setMode(add, *mit);
      channels_[paramsVec[0]].key = paramsVec[2];
      msg += "-+"[add];
      msg += *mit;
      announceToChannel(cs, paramsVec[0], msg.append(" :").append(paramsVec[2]), true);
      paramOnce = true;
    }
    
    else if (!paramOnce && *mit == 'l' && 2 < (int)paramsVec.size()) {
      channels_[paramsVec[0]].setMode(add, *mit);
      channels_[paramsVec[0]].limit = MIN(atoi(paramsVec[2].c_str()), CHANNEL_MEMBER_LIMIT);
      msg += "-+"[add];
      msg += *mit;
      announceToChannel(cs, paramsVec[0], msg.append(" :").append(paramsVec[2]), true);
      paramOnce = true;
    }

// get
// :gello!~a@freenode-ca7.4sl.2765s3.IP MODE #1irc +v :gello2
// :noname!~lna@127.0.0.1 MODE #1 +o :gello
    else if (!paramOnce && *mit == 'v' && 2 < (int)paramsVec.size()) {
      channels_[paramsVec[0]].setMode(add, *mit);
      if (add)
        channels_[paramsVec[0]].channelSpeaker.insert(nickFdPair_.find(paramsVec[2])->second);
      else
        channels_[paramsVec[0]].channelSpeaker.erase(nickFdPair_.find(paramsVec[2])->second);
      msg += "-+"[add];
      msg += *mit;
      announceToChannel(cs, paramsVec[0], msg.append(" :").append(paramsVec[2]), true);
      paramOnce = true;
    }

//give
// :gello2!~1@freenode-ca7.4sl.2765s3.IP MODE #2irc +o :gello
// get
// :gello!~a@freenode-ca7.4sl.2765s3.IP MODE #1irc +o :gello2
    else if (!paramOnce && *mit == 'o' && 2 < (int)paramsVec.size()) {
      channels_[paramsVec[0]].setMode(add, *mit);
      if (add)
        channels_[paramsVec[0]].channelOperators.insert(nickFdPair_.find(paramsVec[2])->second);
      else
        channels_[paramsVec[0]].channelOperators.erase(nickFdPair_.find(paramsVec[2])->second);
      msg += "-+"[add];
      msg += *mit;
      announceToChannel(cs, paramsVec[0], msg.append(" :").append(paramsVec[2]), true);
      paramOnce = true;
    }

    if (MODE_LENGTH <= ++loop)
      break;
  }
}
