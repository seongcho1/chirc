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
  if (paramsVec.size() < 2) {
    reply(cs, ERR_NEEDMOREPARAMS, "MODE", paramsVec);
    return;
  }

  if (users_[cs].nick != paramsVec[0]) {
    // O (server oper.[privilion] only)
    return;
  }

  if (paramsVec[1][0] == '-' || paramsVec[1][0] == '+') {
    // not match param
    return;
  }

  if (paramsVec[1][0] == '-' && paramsVec[1][1] == 'i') {
    users_[cs].setMode(false, 'i');
    // RPL_UMODEIS
    announceOneUser(cs, "i am visible");
  }

  if (paramsVec[1][0] == '+' && paramsVec[1][1] == 'i') {
    users_[cs].setMode(true, 'i');
    // RPL_UMODEIS
    announceOneUser(cs, "i am invisible");
  }
}

void MessageManager::modeChannel(int cs, std::vector<std::string> paramsVec) {
  // std::string channel = paramsVec[0];

  if (channels_.find(paramsVec[0]) == channels_.end()) {
    reply(cs, ERR_NOSUCHCHANNEL, "MODE", paramsVec);
    return;
  }

  if (paramsVec.size() < 2) {
    outMessages_[cs].append("Mode: ").append(channels_[paramsVec[0]].currentMode(CHN_M_A_FLAGS)).append("\n");
    return;
  }

  if (channels_[paramsVec[0]].channelOperators.find(cs) == channels_[paramsVec[0]].channelOperators.end()) {
    reply(cs, ERR_CHANOPRIVSNEEDED, "MODE", paramsVec);
    return;
  }

  if (paramsVec[1][0] != '+' && paramsVec[1][0] != '-') {
    // mode flag error
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
    msg.append(users_[cs].nick).append(" has changed mode: ");
    msg += "-+"[add];
    msg += *mit;
    if (SS::compare(*mit, CHN_M_FLAGS)) {
      channels_[paramsVec[0]].setMode(add, *mit);
      // RPL_CHANNELMODEIS
      announceToChannel(cs, paramsVec[0], msg, true);
    }
    else if (!paramOnce && *mit == 'k' && 2 < (int)paramsVec.size()) {
      channels_[paramsVec[0]].setMode(add, *mit);
      channels_[paramsVec[0]].key = paramsVec[2];
      // RPL_CHANNELMODEIS
      announceToChannel(cs, paramsVec[0], msg.append(" ").append(paramsVec[2]), true);
      paramOnce = true;
    }
    else if (!paramOnce && *mit == 'l' && 2 < (int)paramsVec.size()) {
      channels_[paramsVec[0]].setMode(add, *mit);
      channels_[paramsVec[0]].limit = MIN(atoi(paramsVec[2].c_str()), CHANNEL_MEMBER_LIMIT);
      // RPL_CHANNELMODEIS
      announceToChannel(cs, paramsVec[0], msg.append(" ").append(paramsVec[2]), true);
      paramOnce = true;
    }
    else if (!paramOnce && *mit == 'v' && 2 < (int)paramsVec.size()) {
      channels_[paramsVec[0]].setMode(add, *mit);
      if (add)
        channels_[paramsVec[0]].channelSpeaker.insert(nickFdPair_.find(paramsVec[2])->second);
      else
        channels_[paramsVec[0]].channelSpeaker.erase(nickFdPair_.find(paramsVec[2])->second);
      // RPL_CHANNELMODEIS
      announceToChannel(cs, paramsVec[0], msg.append(" ").append(paramsVec[2]), true);
      paramOnce = true;
    }
    else if (!paramOnce && *mit == 'o' && 2 < (int)paramsVec.size()) {
      channels_[paramsVec[0]].setMode(add, *mit);
      if (add)
        channels_[paramsVec[0]].channelOperators.insert(nickFdPair_.find(paramsVec[2])->second);
      else
        channels_[paramsVec[0]].channelOperators.erase(nickFdPair_.find(paramsVec[2])->second);
      // RPL_CHANNELMODEIS
      announceToChannel(cs, paramsVec[0], msg.append(" ").append(paramsVec[2]), true);
      paramOnce = true;
    }

    if (MODE_LENGTH <= ++loop)
      break;
  }
}



// std::string MessageManager::prettyModeFlags(std::string mode) {
//   std::string result;

//   for (std::string::iterator it = mode.begin(); it != mode.end(); ++it) {
//     if (0 < (int)result.size() && (*it == '+' || *it == '-') && (result.back() == '+' || result.back() == '-'))
//       result.pop_back();
//     if (*it == '+' || *it == '-')
//       result.push_back(*it);
//     // if (SS::first())
//   }

//   return result;
// }
