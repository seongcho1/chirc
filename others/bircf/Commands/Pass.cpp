#include "../MessageManager.hpp"

void MessageManager::PASS(int cs, std::vector<std::string> paramsVec, std::string trailing) {

  if (users_[cs].authenticated & AUTH_MASK) { //??which level of auth do i need to use here??
    reply(cs, ERR_ALREADYREGISTRED, "PASS", paramsVec, trailing);
    return;
  }

  if (users_[cs].authenticated & AUTH_LEVEL1)
    return;

  if (paramsVec.size() != 1 || !trailing.empty()) {
    reply(cs, ERR_NEEDMOREPARAMS, "PASS", paramsVec, trailing);
    return;
  }

  if (*paramsVec.begin() == pass) {
    users_[cs].authenticated |= AUTH_LEVEL1;
    outMessages_[cs].append("-- welcome [SOMEBODY], setup nickname please --\n");
  }
  else
    outMessages_[cs].append("** WRONG PASSWORD! **\n");
}
