#include "../../MessageManager.hpp"

void MessageManager::MODE(int cs, std::vector<std::string> paramsVec) {
  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "MODE", paramsVec);
    return;
  }

  std::string channel = paramsVec[0];

  if (channels_.find(channel) == channels_.end()) {
    reply(cs, ERR_NOSUCHCHANNEL, "MODE", paramsVec);
    return;
  }

  if (paramsVec.size() < 2) {
    outMessages_[cs].append("Mode: ").append(channels_[channel].currentMode(CHN_M_A_FLAGS)).append("\n");
    return;
  }

  if (channels_[channel].channelOperators.find(cs) == channels_[channel].channelOperators.end()) {
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
      channels_[channel].setMode(add, *mit);
      announceToChannel(cs, channel, msg, true);
    }
    else if (!paramOnce && *mit == 'k' && 2 < (int)paramsVec.size()) {
      channels_[channel].setMode(add, *mit);
      channels_[channel].key = paramsVec[2];
      announceToChannel(cs, channel, msg.append(" ").append(paramsVec[2]), true);
      paramOnce = true;
    }
    else if (!paramOnce && *mit == 'l' && 2 < (int)paramsVec.size()) {
      channels_[channel].setMode(add, *mit);
      channels_[channel].limit = atoi(paramsVec[2].c_str());
      announceToChannel(cs, channel, msg.append(" ").append(paramsVec[2]), true);
      paramOnce = true;
    }
    else if (!paramOnce && *mit == 'v' && 2 < (int)paramsVec.size()) {
      channels_[channel].setMode(add, *mit);
      if (add)
        channels_[channel].channelSpeaker.insert(nickFdPair_.find(paramsVec[2])->second);
      else
        channels_[channel].channelSpeaker.erase(nickFdPair_.find(paramsVec[2])->second);
      announceToChannel(cs, channel, msg.append(" ").append(paramsVec[2]), true);
      paramOnce = true;
    }
    else if (!paramOnce && *mit == 'o' && 2 < (int)paramsVec.size()) {
      channels_[channel].setMode(add, *mit);
      if (add)
        channels_[channel].channelOperators.insert(nickFdPair_.find(paramsVec[2])->second);
      else
        channels_[channel].channelOperators.erase(nickFdPair_.find(paramsVec[2])->second);
      announceToChannel(cs, channel, msg.append(" ").append(paramsVec[2]), true);
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
