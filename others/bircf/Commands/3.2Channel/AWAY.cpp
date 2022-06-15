#include "../../MessageManager.hpp"

void MessageManager::AWAY(int cs, std::vector<std::string> paramsVec) {
// invite gello #1irc
// :*.freenode.net 341 gello2 gello :#1irc
// :*.freenode.net 301 gello2 gello :"hahaha this is gello"

// AWAY "have a good bye"
// :*.freenode.net 306 gello2 :You have been marked as being away

  if (paramsVec.size() == 0) {
    reply(cs, RPL_UNAWAY, "AWAY", paramsVec);
    users_[cs].away = "";
    return;
  }

  std::string away = SS::makeOneString(paramsVec.begin(), paramsVec.end());
  paramsVec[0] = away;
  reply(cs, RPL_NOWAWAY, "AWAY", paramsVec);
  users_[cs].away = away;
}
