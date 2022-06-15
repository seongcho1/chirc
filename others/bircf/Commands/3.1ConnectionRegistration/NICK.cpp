#include "../../MessageManager.hpp"

void MessageManager::NICK(int cs, std::vector<std::string> paramsVec) {

  if (!(users_[cs].authenticated & AUTH_LEVEL1)) {
    reply(cs, ERR_NOTREGISTERED, "executeMessage", paramsVec); //451
    return;
  }

  if (paramsVec.size() != 1) {
    reply(cs, ERR_NONICKNAMEGIVEN, "NICK", paramsVec); //431
    return;
  }

  //See section 2.3.1 for details on valid nicknames.
  // if (nick[0] is numeric or special or error in nick) {
  std::string nick = paramsVec[0];
  if ( (nick.length() > NICK_MAX_LENGTH) ||
       (SS::toUpper(nick).compare("ANONYMOUS") == 0) ||
       !SS::isNickChar(nick)) {
    reply(cs, ERR_ERRONEUSNICKNAME, "NICK", paramsVec); //432
    return;
  }

  char authStatus = users_[cs].authenticated;

  if (isUniqueNick(cs, nick)) {
    bool auth = false;
    if (users_[cs].authenticated != AUTH_MASK)
      auth = true;

    users_[cs].authenticated |= AUTH_LEVEL2;
    // welcome msg after pass + nick + user
    if (users_[cs].authenticated == AUTH_MASK && auth) {
      reply(cs, RPL_WELCOME, "NICK", paramsVec); //001
      ping(cs);
    }

    if (authStatus == AUTH_MASK)
      announceToNeighbors(cs, std::string().append (":").append(users_[cs].legacyPrefix()).append(" NICK ").append(nick), true);
  }
  else {
    reply(cs, ERR_NICKNAMEINUSE, "NICK", paramsVec); //433
    return;
  }

  // seongcho: to avoid duplicated  announce,
  // get unique users from all channels the user is engaged
  // then  announce to the users list ???
  // if (authStatus == AUTH_MASK) {
  // if (AUTH_MASK == users_[cs].authenticated && users_[cs].authenticated == authStatus) {
  //   // :WiZ!jto@tolsun.oulu.fi NICK Kilroy  ; Server telling that WiZ changed his nickname to Kilroy.
  //   announceToNeighbors(cs, std::string().append (":").append(users_[cs].legacyPrefix()).append(" NICK ").append(nick), true);
  // }
}

bool MessageManager::isUniqueNick(int cs, std::string &nick) {
  if (nickFdPair_.find(nick) == nickFdPair_.end()) {
    nickFdPair_[nick] = cs;

    User &user = users_[cs];
    user.pnik = user.nick;
    user.nick = nick;
    nickFdPair_.erase(user.pnik);
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
           ERR_ERRONEUSNICKNAME               :432 done
           ERR_NICKNAMEINUSE                  :433 half done
           ERR_NICKCOLLISION                  :436 not in the scope <-server to server
           ERR_UNAVAILRESOURCE                :437 ??
           ERR_RESTRICTED                     :484 ?? //when user mode "+r"

   Examples:

   NICK Wiz                ; Introducing new nick "Wiz" if session is
                           still unregistered, or user changing his
                           nickname to "Wiz"

   :WiZ!jto@tolsun.oulu.fi NICK Kilroy
                           ; Server telling that WiZ changed his
                           nickname to Kilroy.

*/
