#include "../../MessageManager.hpp"

void MessageManager::NICK(int cs, std::vector<std::string> paramsVec) {

  if (paramsVec.size() != 1) {
    reply(cs, ERR_NONICKNAMEGIVEN, "NICK", paramsVec); //431
    return;
  }

	// get user's mode string???
  // if(users_.find(cs) !== users_.end() && users_[cs].getMode().find('r') != std::string::npos)
  //  reply(cs, ERR_RESTRICTED, "NICK", paramsVec); //484
  //  return;
  //}

  //See section 2.3.1 for details on valid nicknames.
  // if (nick[0] is numeric or special or error in nick) {
  std::string nick = paramsVec[0];
  if ( (nick.length() > NICK_MAX_LENGTH) ||
       (SS::toUpper(nick).compare("ANONYMOUS") == 0) ||
       SS::containExceptChar(nick, std::string(" ,*?!@.")) ||
       !isalpha(nick[0]) || //https://datatracker.ietf.org/doc/html/rfc1459#section-2.3.1
       nick[0] == '$' || nick[0] == ':' ||
       nick[0] == '#' || nick[0] == '&' || nick[0] == '+' || nick[0] == '!') {
    reply(cs, ERR_ERRONEUSNICKNAME, "NICK", paramsVec); //432
    return;
  }

  std::string legacyPrefix = users_[cs].prefix();

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

  } else {
    reply(cs, ERR_NICKNAMEINUSE, "NICK", paramsVec); //433
  }

  // seongcho: to avoid duplicated  announce,
  // get unique users from all channels the user is engaged
  // then  announce to the users list ???
  if ( (users_[cs].authenticated == AUTH_MASK) && !legacyPrefix.empty()) {
    std::string message;
    // std::set<std::string>::iterator eit = users_[cs].engaged.begin();
    // :WiZ!jto@tolsun.oulu.fi NICK Kilroy  ; Server telling that WiZ changed his nickname to Kilroy.
    message.append(":" + legacyPrefix + " NICK " + nick);
    announceToUser(ircfd, message);
    // while (eit != users_[cs].engaged.end()) {
    //   announceToChannel(*eit++, message);
    // }
  }
  // announce to engaged channels [from nick to nick] ----------------------------------
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
