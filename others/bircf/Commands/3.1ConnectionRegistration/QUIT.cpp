#include "../../MessageManager.hpp"

void MessageManager::QUIT(int cs, std::vector<std::string>, std::string) {
  users_[cs].toQuit();
}
