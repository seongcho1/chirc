#include "../../MessageManager.hpp"

void MessageManager::PING(int cs, std::vector<std::string> paramsVec) {

  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "PING", paramsVec); //461
    announceOneUser(cs, std::string().append("** Usage: PING <cookie> **"));
    return;
  }

  announceOneUser(cs, paramsVec[0]);
}