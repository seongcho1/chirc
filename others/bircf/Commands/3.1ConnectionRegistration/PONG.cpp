#include "../../MessageManager.hpp"

void MessageManager::PONG(int cs, std::vector<std::string> paramsVec) {

  if (paramsVec.size() != 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "PONG", paramsVec); //461
    return;
  }

  if (paramsVec[0] == PONG_RESULT) {
    users()[cs].dead = time(NULL) + TIMEOUT + WAIT_TIME;
  }
}
