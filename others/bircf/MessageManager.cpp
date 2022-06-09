#include "MessageManager.hpp"

void MessageManager::registerFunctions() {
  functionCallMap_["PRIVMSG"] =	&MessageManager::PRIVMSG;
  functionCallMap_["SELFMSG"] =	&MessageManager::SELFMSG;
  functionCallMap_["PUBLICMSG"] =	&MessageManager::PUBLICMSG;
}

void MessageManager::PRIVMSG(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() != 1) {
    // do something with errcode errcode:errstr map
    return;
  }
  if (trailing.length() == 0) {
    // do something with errcode errcode:errstr map
    return;
  }

  int toFd;
  try {
    toFd = std::stoi(paramsVec[0]);
  }
  catch (const std::exception &e) {
    // do something with errcode errcode:errstr map
    return;
  }

  std::map<int, User>::iterator uit = users_.find(toFd);
  if (uit == users_.end()) {
    // do something with errcode errcode:errstr map
    return;
  }

  trailing = std::string("[from " + SS::toString(cs) + ", to " + paramsVec[0] + "]").append(trailing).append(NEWLINE);
  outMessages_[toFd].append(trailing);
}

void MessageManager::SELFMSG(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() != 0) {
    // do something with errcode errcode:errstr map
    return;
  }
  if (trailing.length() == 0) {
    // do something with errcode errcode:errstr map
    return;
  }
  std::map<int, User>::iterator uit = users_.find(cs);
  if (uit == users_.end()) {
    // do something with errcode errcode:errstr map
    return;
  }

  trailing = std::string("[from myself, " + SS::toString(cs) + "]").append(trailing).append(NEWLINE);
  outMessages_[cs].append(trailing);
}

void MessageManager::PUBLICMSG(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() != 0) {
    // do something with errcode errcode:errstr map
    return;
  }
  if (trailing.length() == 0) {
    // do something with errcode errcode:errstr map
    return;
  }
  std::map<int, User>::iterator uit = users_.find(cs);
  if (uit == users_.end()) {
    // do something with errcode errcode:errstr map
    return;
  }

  trailing = std::string("[from" + SS::toString(cs) + "]").append(trailing).append(NEWLINE);
  for (uit = users_.begin(); uit != users_.end(); ++uit) {
    if (uit->first != cs)
      outMessages_[uit->first].append(trailing);
  }
}

MessageManager::~MessageManager() {
  users_.clear();
  inMessages_.clear();
  outMessages_.clear();
}

void MessageManager::fdClean(int cs) {
  std::map<int, User>::iterator uit = users_.find(cs);
  if (uit != users_.end())
    users_.erase(cs);

  // delete from inMessages_
  std::map<int, std::string>::iterator cit = inMessages_.find(cs);
  if (cit != inMessages_.end())
    inMessages_.erase(cs);

  // delete from outMessages_
  cit = outMessages_.find(cs);
  if (cit != outMessages_.end())
    outMessages_.erase(cs);
}

void MessageManager::executeMessages(int cs) {
  std::vector<std::string> messageVec = splitMessages(cs);
  std::string message;

  while (messageVec.size()) {
    message = messageVec.front();
    executeMessage(cs, message);
    messageVec.erase(messageVec.begin()); // in case of vector
    // messageVec.pop(); 					//in case of queue
  }
  // inMessages_[cs].clear();
}

std::vector<std::string> MessageManager::splitMessages(int cs, bool bSkipLast, bool bClearMessages) {
  return SS::splitString(inMessages_[cs], NEWLINE, bSkipLast, bClearMessages);
}

void MessageManager::executeMessage(int cs, std::string message) {
  std::string command_params, trailing;
  std::vector<std::string> command_params_trailing = SS::splitString(message, SPACE_COLON, false, false, true);
  if (command_params_trailing.empty())
    return;
  switch (command_params_trailing.size())
  {
  case 1:
    command_params = command_params_trailing[0];
    trailing = "";
    break;
  case 2:
    command_params = command_params_trailing[0];
    trailing = command_params_trailing[1];
    break;
  default:
    return;
    break;
  }

  std::string command;
  std::vector<std::string> paramsVec = SS::splitString(command_params, SPACE);
  if (paramsVec.empty())
    return;
  // get the commandName which is the first element in the argsVec
  command = SS::toUpper(paramsVec.front());
  // then remove the first element and the rest should be args
  paramsVec.erase(paramsVec.begin());

  // change this pattern tcsin_leno using a map<commandName, commandFunc>
  // execute commandMap[command](cs, command)
  /*
  if (commandName.compare("PRIVMSG") == 0)
    PRIVMSG(cs, command);
  else if (commandName.compare("SELFMSG") == 0)
    SELFMSG(cs, command);
  else if (commandName.compare("PUBLICMSG") == 0)
    PUBLICMSG(cs, command);
  */
  std::map<std::string, FuncPtr>::iterator it = functionCallMap_.find(command);
  if (it != functionCallMap_.end())
    (this->*functionCallMap_[command])(cs, paramsVec, trailing);
}

void MessageManager::srvAccept(int s) {
  int cs;
  struct sockaddr_in csin;
  socklen_t csinLen;

  csinLen = sizeof(csin);
  cs = accept(s, (struct sockaddr *)&csin, &csinLen);

  fcntl(cs, F_SETFL, O_NONBLOCK);

  std::cout << "New client #" << cs << " from "
            << inet_ntoa(csin.sin_addr) << ":"
            << ntohs(csin.sin_port) << std::endl;

  // clean_fd(cs);
  // fds_[cs].type = FD_CLIENT;
  // //fds_[cs].fct_read = client_read;
  // //fds_[cs].fct_write = client_write;
  reqAuthenticates_.insert(std::pair<int, User>(cs, User(cs, inet_ntoa(csin.sin_addr))));
  // users_.insert(std::pair<int, User>(cs, User(cs)));
}

void MessageManager::clientRead(int cs) {
std::cout << "Debug begin\n* " << inMessages_[cs] << "before len = " << inMessages_[cs].length() << std::endl;
std::cout << "Debug end\n";
  if (users_[cs].clientRead(inMessages_[cs])) {
std::cout << "Debug begin\n* " << inMessages_[cs] << "after len = " << inMessages_[cs].length() << std::endl;
std::cout << "Debug end\n";
    executeMessages(cs);
  }
  else {
    fdClean(cs); // del User *, inMessages_, out_commands
    close(cs);    // cleaning the table first, and then the table will be ready for another client
    std::cout << "client #" << cs << " gone away" << std::endl;
  }
}
