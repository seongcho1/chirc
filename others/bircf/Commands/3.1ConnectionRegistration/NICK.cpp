#include "../../MessageManager.hpp"

void MessageManager::NICK(int cs, std::vector<std::string> paramsVec, std::string trailing) {

  if (paramsVec.size() != 1 || !trailing.empty()) {
    reply(cs, ERR_NONICKNAMEGIVEN, "NICK", paramsVec, trailing); //431
    return;
  }

	// get user's mode string???
  // if(users_.find(cs) !== users_.end() && users_[cs].getMode().find('r') != std::string::npos)
  //  reply(cs, ERR_RESTRICTED, "NICK", paramsVec, trailing); //484
  //  return;
  //}

  //See section 2.3.1 for details on valid nicknames.
  // if (nick[0] is numeric or special or error in nick) {
  std::string nick = paramsVec[0];
  if ( (nick.length() > NICK_MAX_LENGTH) ||
       (SS::toUpper(nick).compare("ANONYMOUS") == 0) ||
       SS::containExceptChar(nick, std::string(" ,*?!@.")) ||
       nick[0] == '$' || //nick[0] == ':' || //: already filtered up there in 431
       nick[0] == '#' ) { //if there are more channel type prefixes then put them here
    reply(cs, ERR_ERRONEUSNICKNAME, "NICK", paramsVec, trailing); //432
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
      reply(cs, RPL_WELCOME, "NICK", paramsVec, trailing); //001
      ping(cs);
    }
    /*
    //broadcast that currentPrefix changed his nick
    if ( (users_[cs].authenticated == AUTH_MASK) && !legacyPrefix.empty()) {
      for (std::map<int, User>::iterator uit = users_.begin(); uit != users_.end(); ++uit)
        outMessages_[uit->first].append(":" + legacyPrefix + " NICK " + nick).append(NEWLINE);
    }
    */
  }
  else {
    reply(cs, ERR_NICKNAMEINUSE, "NICK", paramsVec, trailing); //433
  }

  //to avoid duplicated  announce, get unique users from all channels the user is engaged
  //then  announce to the users list ???
  if ( (users_[cs].authenticated == AUTH_MASK) && !legacyPrefix.empty()) {
    std::string message;
    std::set<std::string>::iterator eit = users_[cs].engaged.begin();
    message.append(":" + legacyPrefix + " NICK " + nick).append(NEWLINE);
    while (eit != users_[cs].engaged.end()) {
      announceToChannel(*eit++, message);
    }
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
