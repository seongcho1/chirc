#include "../../MessageManager.hpp"

void MessageManager::NAMES(int cs, std::vector<std::string> paramsVec, std::string trailing) {

std::cout << "names list\n";

  (void)trailing;
  // somethings;
  // reply(cs, RPL_NAMREPLY, "NAMES", paramsVec, trailing);
  outMessages_[cs].append("[NAMES BEGIN]\n");
  if (paramsVec.size()) {
    std::vector<std::string>::iterator it = paramsVec.begin();
    while (it != paramsVec.end()) {
      std::map<std::string, Channel>::iterator cit = channels_.find(*it);
      if (cit != channels_.end())
        // reply(cs, RPL_NAMREPLY, "NAMES", namesVec(cit->second.member), trailing);
        outMessages_[cs].append("[").append(namesVec_TEST(cit->second.member)).append("]\n");
      ++it;
    }
  }
  else {
    std::map<std::string, Channel>::iterator it = channels_.begin();
    while (it != channels_.end()) {
      // reply(cs, RPL_LIST, "NAMES", namesVec(it->second.member), trailing);
      outMessages_[cs].append("[").append(namesVec_TEST(it->second.member)).append("]\n");
      ++it;
    }
  }
  
  // reply(cs, RPL_ENDOFNAMES, "NAMES", paramsVec, trailing);
  outMessages_[cs].append("[NAMES END]\n");
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

std::string MessageManager::namesVec_TEST(std::set<int> fds) {
  std::set<int>::iterator si = fds.begin();
  std::string result;

  while (si != fds.end()) {
    result.append(users_[*si].nick).append("/");
    ++si;
  }
  return result;
}
