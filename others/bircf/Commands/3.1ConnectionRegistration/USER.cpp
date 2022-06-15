#include "../../MessageManager.hpp"

void MessageManager::USER(int cs, std::vector<std::string> paramsVec) {

  if (!(users_[cs].authenticated & AUTH_LEVEL1)) {
    reply(cs, ERR_NOTREGISTERED, "executeMessage", paramsVec); //451
    return;
  }

  if ((users_[cs].authenticated & AUTH_LEVEL3)) {
    reply(cs, ERR_ALREADYREGISTRED, "USER", paramsVec); //462
    return;
  }

  if (paramsVec.size() != 4) {
    reply(cs, ERR_NEEDMOREPARAMS, "USER", paramsVec); //461
    outMessages_[cs].append("** Usage: USER <user> <mode> <unused> <realname> **\n");
    return;
  }

  //<mode> should be a numeric
  if (!SS::isNumber(paramsVec[1])) {
    reply(cs, ERR_NEEDMOREPARAMS, "USER :The <mode> parameter should be a numeric", paramsVec); //461
    outMessages_[cs].append("** Usage: USER <user> <mode> <unused> <realname> **\n");
    return;
  }

  User &user = users_[cs];
  std::vector<std::string>::iterator it = paramsVec.begin();
  //there are no orders for pass, nick user
  //if (AUTH_LEVEL1 <= user.authenticated) {
    user.authenticated |= AUTH_LEVEL3;
    user.user = *it++;
    it++;// user.setMode(true, atoi((*it++).c_str()));
    it++;
    user.real = *it;

    if (users_[cs].authenticated == AUTH_MASK) {
      reply(cs, RPL_WELCOME, "USER", paramsVec); //001
      ping(cs);
    }
  //}
}

/*
https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.3
3.1.3 User message

      Command: USER
   Parameters: <user> <mode> <unused> <realname>

   The USER command is used at the beginning of connection to specify
   the username, hostname and realname of a new user.

   The <mode> parameter should be a numeric, and can be used to
   automatically set user modes when registering with the server.  This
   parameter is a bitmask, with only 2 bits having any signification: if
   the bit 2 is set, the user mode 'w' will be set and if the bit 3 is
   set, the user mode 'i' will be set.  (See Section 3.1.5 "User
   Modes").

   The <realname> may contain space characters.

   Numeric Replies:

           ERR_NEEDMOREPARAMS                 :461 done
           ERR_ALREADYREGISTRED               :462 done



   Example:

   USER guest 0 * :Ronnie Reagan   ; User registering themselves with a
                                   username of "guest" and real name
                                   "Ronnie Reagan".

   USER guest 8 * :Ronnie Reagan   ; User registering themselves with a
                                   username of "guest" and real name
                                   "Ronnie Reagan", and asking to be set
                                   invisible.
*/
