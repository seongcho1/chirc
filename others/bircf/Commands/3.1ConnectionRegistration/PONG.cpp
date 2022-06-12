#include "../../MessageManager.hpp"

void MessageManager::PONG(int cs, std::vector<std::string>, std::string trailing) {
  if (trailing == PONG_RESULT) {
    users()[cs].dead = time(NULL) + TIMEOUT + WAIT_TIME;
  }
}
