#include "MessageManager.hpp"
#include <sys/stat.h>

void MessageManager::registerFunctions() {
  functionCallMap_["PASS"] = &MessageManager::PASS;
  functionCallMap_["NICK"] = &MessageManager::NICK;
  functionCallMap_["USER"] = &MessageManager::USER;
  
  functionCallMap_["PONG"] = &MessageManager::PONG;

  functionCallMap_["QUIT"] = &MessageManager::QUIT;
  functionCallMap_["JOIN"] = &MessageManager::JOIN;
  functionCallMap_["PART"] = &MessageManager::PART;

  functionCallMap_["PRIVMSG"] =	&MessageManager::PRIVMSG;
  functionCallMap_["SELFMSG"] =	&MessageManager::SELFMSG;
  functionCallMap_["PUBLICMSG"] =	&MessageManager::PUBLICMSG;
  functionCallMap_["TESTMSG"] =	&MessageManager::TESTMSG;
}

MessageManager::~MessageManager() {
  users_.clear();
  inMessages_.clear();
  outMessages_.clear();
}

void MessageManager::fdClean(int cs) {
  if (users_.find(cs) != users_.end()) {
    PART(cs, std::vector<std::string>(users_[cs].engaged.begin(), users_[cs].engaged.end()), "");
    // std::set<std::string>::iterator it = users_[cs].engaged.begin();
    // while (it != users_[cs].engaged.end()) {
    //   PART(cs, std::vector<std::string>(users_[cs].engaged.begin(), users_[cs].engaged.end()), "");
    //   channels_[*it++].leave(cs);
    // }
    nickFdPair_.erase(users_[cs].nick);
    users_.erase(cs);
  }

  if (inMessages_.find(cs) != inMessages_.end())
    inMessages_.erase(cs);

  if (outMessages_.find(cs) != outMessages_.end())
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
}

std::vector<std::string> MessageManager::splitMessages(int cs, bool bSkipLast, bool bClearMessages) {
  return SS::splitString(inMessages_[cs], NEWLINE, bSkipLast, bClearMessages);
}

void MessageManager::executeMessage(int cs, std::string message) {

  //For a client, the only valid prefix is the registered nickname associated with the client.
  //If the case, delete the prefix to make it easy to split
  if (users_[cs].authenticated == AUTH_MASK &&
      !message.empty() && message[0] == ':') {
    SS::ltrim(message, std::string(":" + users_[cs].nick + " ").c_str());
    SS::trim(message);
  }

  std::string command_params, trailing;
  std::vector<std::string> command_params_trailing = SS::splitString(message, SPACE_COLON, false, false, true);

  //now SS::splitString(message, SPACE_COLON, false, false, true) always returns two (empty) elements
  if ( (command_params_trailing.size() != 2)  ||
       (command_params_trailing[0].empty() && command_params_trailing[1].empty()) )
    return;

  command_params = command_params_trailing[0];
  trailing = command_params_trailing[1];

  std::string command;
  std::vector<std::string> paramsVec = SS::splitString(command_params, SPACE);

  if (paramsVec.empty())
    return;
  // get the commandName which is the first element in the argsVec
  command = SS::toUpper(paramsVec.front());
  // then remove the first element and the rest should be args
  paramsVec.erase(paramsVec.begin());

  //if not registered yet, only three following commands are usable
  if (users_[cs].authenticated < AUTH_MASK) {
    if (command.compare("PASS") != 0 &&
        command.compare("NICK") != 0 &&
        command.compare("USER") != 0 &&
        command.compare("QUIT") != 0) {

      reply(cs, ERR_NOTREGISTERED, "executeMessage", paramsVec, trailing); //451
      return;
    }
  }

  //the command parameters (maximum of fifteen including trailing)
  if ( (paramsVec.size() > 15 && trailing.empty()) ||
       (paramsVec.size() > 14 && !trailing.empty()) ) {
    reply(cs, ERR_NEEDMOREPARAMS, "executeMessage", paramsVec, trailing); //461 too many parameters
    return;
  }

  std::map<std::string, FuncPtr>::iterator it = functionCallMap_.find(command);
  if (it != functionCallMap_.end())
    (this->*functionCallMap_[command])(cs, paramsVec, trailing);
  else {
    paramsVec.insert(paramsVec.begin(), command);
    reply(cs, ERR_UNKNOWNCOMMAND, "executeMessage", paramsVec, trailing); //421
  }

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
  users_.insert(std::pair<int, User>(cs, User(cs, inet_ntoa(csin.sin_addr), pass.empty())));
}

void MessageManager::clientRead(int cs) {
  users_[cs].clientRead(inMessages_[cs]) ?
    executeMessages(cs) : users_[cs].toQuit(); /*kickUser(cs);*/
}

void MessageManager::kickUser(int cs) {
  fdClean(cs); // del User *, inMessages_, out_commands
  close(cs);    // cleaning the table first, and then the table will be ready for another client
  std::cout << "client #" << cs << " gone away" << std::endl;
}

void MessageManager::ping(int cs) {
  outMessages_[cs].append(PING_REQUEST).append(PONG_RESULT).append(NEWLINE);
  users_[cs].alive = time(NULL) + TIMEOUT;
}
