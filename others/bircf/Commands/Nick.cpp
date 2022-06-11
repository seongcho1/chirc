#include "../MessageManager.hpp"
#include <sstream>

void MessageManager::NICK(int cs, std::vector<std::string> paramsVec, std::string trailing) {

  if (paramsVec.size() != 1 || !trailing.empty()) {
    reply(cs, ERR_NONICKNAMEGIVEN, "NICK", paramsVec, trailing); //431
    return;
  }

  if (AUTH_LEVEL1 <= users_[cs].authenticated) {
    if (NICK_MAX_LENGTH < (*paramsVec.begin()).length())
      outMessages_[cs].append("** nickname having a maximum length of nine(9) characters **\n");
    else if (isUniqueNick(cs, *paramsVec.begin())) {
      users_[cs].authenticated |= AUTH_LEVEL2;
      outMessages_[cs].append("-- welcome [").append(*paramsVec.begin()).append("] --\n");
    }
    else
      outMessages_[cs].append(std::string("** [" + *paramsVec.begin() + "] already occupied **\n"));
  }
}

bool MessageManager::isUniqueNick(int cs, std::string &nick) {
  if (nickFdPair_.find(nick) == nickFdPair_.end()) {
    User &user = users_[cs];
    std::string current = user.nick;
    nickFdPair_[nick] = cs;
    user.nick = nick;
    nickFdPair_.erase(current);
    return true;
  }
  return false;
}

/*
https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.2
3.1.2 Nick message


      Command: NICK
   Parameters: <nickname>

   NICK command is used to give user a nickname or change the existing
   one.

   Numeric Replies:

           ERR_NONICKNAMEGIVEN                :431 done
           ERR_ERRONEUSNICKNAME               :432
           ERR_NICKNAMEINUSE                  :433
           ERR_NICKCOLLISION                  :436 not in the scope <-server to server
           ERR_UNAVAILRESOURCE                :437
           ERR_RESTRICTED                     :484 when user mode "+r"

   Examples:

   NICK Wiz                ; Introducing new nick "Wiz" if session is
                           still unregistered, or user changing his
                           nickname to "Wiz"

   :WiZ!jto@tolsun.oulu.fi NICK Kilroy
                           ; Server telling that WiZ changed his
                           nickname to Kilroy.

*/
