#include "../../MessageManager.hpp"

void MessageManager::PING(int cs, std::vector<std::string> paramsVec) {

  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "PING", paramsVec); //461
    announceToSelf(cs, std::string().append("** Usage: PING <cookie> **"));
    return;
  }

  announceToSelf(cs, paramsVec[0]);
}