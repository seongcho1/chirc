#include "MessageManager.hpp"
#include <sys/stat.h>

void MessageManager::registerFunctions() {
  functionCallMap_["PASS"] =        &MessageManager::PASS;
  functionCallMap_["NICK"] =        &MessageManager::NICK;
  functionCallMap_["USER"] =        &MessageManager::USER;

  functionCallMap_["PING"] =        &MessageManager::PING;
  functionCallMap_["PONG"] =        &MessageManager::PONG;

  functionCallMap_["AWAY"] =        &MessageManager::AWAY;
  functionCallMap_["QUIT"] =        &MessageManager::QUIT;
  functionCallMap_["JOIN"] =        &MessageManager::JOIN;
  functionCallMap_["PART"] =        &MessageManager::PART;
  functionCallMap_["LIST"] =        &MessageManager::LIST;
  functionCallMap_["KICK"] =        &MessageManager::KICK;
  functionCallMap_["MODE"] =        &MessageManager::MODE;
  functionCallMap_["TOPIC"] =       &MessageManager::TOPIC;
  functionCallMap_["NAMES"] =       &MessageManager::NAMES;
  functionCallMap_["INVITE"] =      &MessageManager::INVITE;

  functionCallMap_["PRIVMSG"] =     &MessageManager::PRIVMSG;
  functionCallMap_["SELFMSG"] =     &MessageManager::SELFMSG;
  functionCallMap_["PUBLICMSG"] =   &MessageManager::PUBLICMSG;
  functionCallMap_["TESTMSG"] =     &MessageManager::TESTMSG;
}

MessageManager::~MessageManager() {
  users_.clear();
  // inMessages_.clear();
  // outMessages_.clear();
}

void MessageManager::fdClean(int cs) {
  if (users_.find(cs) != users_.end()) {
    PART(cs, std::vector<std::string>(users_[cs].engaged.begin(), users_[cs].engaged.end()));
    // std::set<std::string>::iterator it = users_[cs].engaged.begin();
    // while (it != users_[cs].engaged.end()) {
    //   PART(cs, std::vector<std::string>(users_[cs].engaged.begin(), users_[cs].engaged.end()));
    //   channels_[*it++].leave(cs);
    // }
    nickFdPair_.erase(users_[cs].nick);
    users_.erase(cs);
  }

  // if (inMessages_.find(cs) != inMessages_.end())
  //   inMessages_.erase(cs);

  // if (outMessages_.find(cs) != outMessages_.end())
  //   outMessages_.erase(cs);
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
  // return SS::splitString(inMessages_[cs], NEWLINE, bSkipLast, bClearMessages);
  return SS::splitString(users_[cs].rbuff, NEWLINE, bSkipLast, bClearMessages);
}

void MessageManager::executeMessage(int cs, std::string message) {

  //remove front and back white spaces
  SS::trim(message);

  //For a client, the only valid prefix is the registered nickname associated with the client.
  //If the case, delete the prefix to make it easy to split
  if ( users_[cs].authenticated == AUTH_MASK )
    SS::eraseFirstWord(message, std::string(":" + users_[cs].hostmask()), std::string(":" + users_[cs].nick));


  //trailing can be a empty string, so if a string have a colon, it will have two elements, second one is empty one
  //if a string does not contain a colon it will have only one element

  std::string command_params, trailing;
  std::vector<std::string> command_params_trailing = SS::splitString(message, SPACE_COLON, false, false, true);



  // it will do stop (return;) for the following cases
  // " :bbb"    returns (), (bbb)       <- won't happen because of SS::trim(message) -> ":bbb"
  // " :"       returns (),()           <- won't happen because of SS::trim(message) -> ":"
  // NULL       returns NULL            <- won't happen because splitMessages() would not pass a null message
  // guard up anyway
  if ( command_params_trailing.empty()  ||
       (command_params_trailing.size() == 2 && command_params_trailing[0].empty()) )
    return;

  command_params = command_params_trailing[0];

  std::string command;
  std::vector<std::string> paramsVec = SS::splitString(command_params, SPACE);

  // get the commandName which is the first element in the argsVec
  command = SS::toUpper(paramsVec.front());
  // then remove the first element and the rest should be args
  paramsVec.erase(paramsVec.begin());

  //if (empty) trailing exists, it will be the last element of paramsVec
  if (command_params_trailing.size() == 2)
    paramsVec.push_back(command_params_trailing[1]);

  //if not registered yet, only three following commands are usable
  if (users_[cs].authenticated < AUTH_MASK) {
    if (command.compare("PASS") != 0 &&
        command.compare("NICK") != 0 &&
        command.compare("USER") != 0 &&
        command.compare("QUIT") != 0) {

      reply(cs, ERR_NOTREGISTERED,"executeMessage", paramsVec); //451
      return;
    }
  }

  //the command parameters (maximum of fifteen including trailing)
  if (paramsVec.size() > 15) {
    reply(cs, ERR_NEEDMOREPARAMS, command.append(" :Too Many Parameters"), paramsVec); //461 too many parameters
    return;
  }

  // paramsVec elements need to follow nospcrlfcl rule
  // LF, SPACE, COLON is already filtered.
  // it can be done by checking CR and NUL
  // CR ctrl+V, ctrl+M can be checked
  // NUL ?? how can we check NUL ??

  if (SS::containExceptChar(paramsVec, CR)) {
    reply(cs, ERR_NEEDMOREPARAMS, command.append(" :Use nospcrlfcl Parameters"), paramsVec); //461 Use nospcrlfcl parameters
    return;
  }

  std::map<std::string, FuncPtr>::iterator it = functionCallMap_.find(command);
  if (it != functionCallMap_.end())
    (this->*functionCallMap_[command])(cs, paramsVec);
  else {
    paramsVec.insert(paramsVec.begin(), command);
    reply(cs, ERR_UNKNOWNCOMMAND, "executeMessage", paramsVec); //421
  }

}

void MessageManager::srvAccept(int s) {
  int cs;
  struct sockaddr_in csin;
  socklen_t csinLen;

  csinLen = sizeof(csin);
  cs = accept(s, (sockaddr *)&csin, &csinLen);

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

// void MessageManager::clientRead(int cs) {
//   users_[cs].clientRead(inMessages_[cs]) ?
//     executeMessages(cs) : users_[cs].toQuit(); /*kickUser(cs);*/
// }
void MessageManager::clientRead(int cs) {
  users_[cs].clientRead() ?
    executeMessages(cs) : users_[cs].toQuit(); /*kickUser(cs);*/
}

void MessageManager::kickUser(int cs) {
  fdClean(cs); // del User *, inMessages_, out_commands
  close(cs);    // cleaning the table first, and then the table will be ready for another client
  std::cout << "client #" << cs << " gone away" << std::endl;
}

// void MessageManager::ping(int cs) {
//   outMessages_[cs].append(PING_REQUEST).append(PONG_RESULT).append(NEWLINE);
//   users_[cs].alive = time(NULL) + TIMEOUT;
// }
void MessageManager::ping(int cs) {
  users_[cs].wbuff.append(PING_REQUEST).append(PONG_RESULT).append(NEWLINE);
  users_[cs].alive = time(NULL) + TIMEOUT;
}

// void MessageManager::announceToChannel(int cs, std::string title, std::string message, bool withMe) {
//   std::set<int>::iterator it = channels_[title].member.begin();
//   while (it != channels_[title].member.end()) {
//     if (withMe || cs != *it)
//       outMessages_[*it].append(message).append(NEWLINE);
//     ++it;
//   }
// }
void MessageManager::announceToChannel(int cs, std::string title, std::string message, bool withMe) {
  std::set<int>::iterator it = channels_[title].member.begin();
  while (it != channels_[title].member.end()) {
    if (withMe || cs != *it)
      users_[*it].wbuff.append(message).append(NEWLINE);
    ++it;
  }
}

// void MessageManager::announceOneUser(int cs, std::string message) {
//   outMessages_[cs].append(message).append(NEWLINE);
// }
void MessageManager::announceOneUser(int cs, std::string message) {
  users_[cs].wbuff.append(message).append(NEWLINE);
}

// void MessageManager::announceToNeighbors(int cs, std::string message, bool withMe) {
//   std::set<std::string>::iterator engagedit = users_[cs].engaged.begin();
//   std::set<int> friendly;

//   while (engagedit != users_[cs].engaged.end()) {
//     friendly.insert(channels_[*engagedit].member.begin(), channels_[*engagedit].member.end());
//     ++engagedit;
//   }

//   if (withMe)
//     friendly.insert(cs);

//   for (std::set<int>::iterator it = friendly.begin(); it != friendly.end(); ++it) {
//     outMessages_[*it].append(message).append(NEWLINE);
//   }
// }
void MessageManager::announceToNeighbors(int cs, std::string message, bool withMe) {
  std::set<std::string>::iterator engagedit = users_[cs].engaged.begin();
  std::set<int> friendly;

  while (engagedit != users_[cs].engaged.end()) {
    friendly.insert(channels_[*engagedit].member.begin(), channels_[*engagedit].member.end());
    ++engagedit;
  }

  if (withMe)
    friendly.insert(cs);

  for (std::set<int>::iterator it = friendly.begin(); it != friendly.end(); ++it) {
    users_[*it].wbuff.append(message).append(NEWLINE);
  }
}

std::string MessageManager::channelMemberToString(std::string channel) {
  std::string memberString;

  std::set<int>::iterator it = channels_[channel].member.begin();
  while (it != channels_[channel].member.end()) {
    if (channels_[channel].isOper(*it))
      memberString.append("@");
    memberString.append(users_[*it++].nick).append(" ");
  }

  return memberString;
}
