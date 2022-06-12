#include "../../MessageManager.hpp"

void MessageManager::LIST(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  
  // somethings;

  reply(cs, RPL_LISTSTART, "LIST", paramsVec, trailing);
  // reply(cs, RPL_WELCOME, "LIST", paramsVec, trailing);

  if (paramsVec.size()) {
    std::vector<std::string>::iterator it = paramsVec.begin();
    while (it != paramsVec.end()) {
      if (channels_.find(*it) != channels_.end())
        reply(cs, RPL_LIST, "LIST", paramsVec, trailing);
        // reply(cs, RPL_WELCOME, "LIST", paramsVec, trailing);
      ++it;
    }
  }
  else {
    std::map<std::string, Channel>::iterator it = channels_.begin();
    while (it != channels_.end()) {
      reply(cs, RPL_LIST, "LIST", paramsVec, trailing);
      // reply(cs, RPL_WELCOME, "LIST", paramsVec, trailing);
      ++it;
    }
  }

  reply(cs, RPL_LISTEND, "LIST", paramsVec, trailing);
  // reply(cs, RPL_WELCOME, "LIST", paramsVec, trailing);
}
