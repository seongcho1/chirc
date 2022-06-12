#include "../../MessageManager.hpp"

void MessageManager::NAMES(int cs, std::vector<std::string> paramsVec, std::string trailing) {

  // somethings;
  // reply(cs, RPL_NAMREPLY, "NAMES", paramsVec, trailing);
  if (paramsVec.size()) {
    std::vector<std::string>::iterator it = paramsVec.begin();
    while (it != paramsVec.end()) {
      std::map<std::string, Channel>::iterator cit = channels_.find(*it);
      if (cit != channels_.end())
        reply(cs, RPL_NAMREPLY, "NAMES", namesVec(cit->second.member), trailing);
        // reply(cs, RPL_WELCOME, "NAMES", namesVec(cit->second.member), trailing);
      ++it;
    }
  }
  else {
    std::map<std::string, Channel>::iterator it = channels_.begin();
    while (it != channels_.end()) {
      reply(cs, RPL_LIST, "NAMES", namesVec(it->second.member), trailing);
      // reply(cs, RPL_WELCOME, "NAMES", namesVec(it->second.member), trailing);
      ++it;
    }
  }
  
  reply(cs, RPL_ENDOFNAMES, "NAMES", paramsVec, trailing);
  // reply(cs, RPL_WELCOME, "NAMES", paramsVec, trailing);
}

std::vector<std::string> MessageManager::namesVec(std::set<int> fds) {
  std::set<int>::iterator si = fds.begin();
  std::vector<std::string> result;
  
  result.reserve(fds.size());

  while (si != fds.end()) {
    result.push_back(users_[*si].nick);
    ++si;
  }
  return result;
}

