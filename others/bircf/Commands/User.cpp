#include "../MessageManager.hpp"
#include <sstream>

void MessageManager::USER(int cs, std::vector<std::string> paramsVec, std::string trailing) {
  if (paramsVec.size() != 4 || !trailing.empty()) {
    outMessages_[cs].append("** Usage: [USER <user_name> <host> <not use> <real_name>] **\n");
    return;
  }

  if (users_[cs].authenticated & AUTH_LEVEL3) {
  // if (users_.find(cs) == users_.end()) {
    outMessages_[cs].append("** You may not reregister **\n");
    return;
  }

  User &user = users_[cs];
  std::vector<std::string>::iterator it = paramsVec.begin();
  if (AUTH_LEVEL1 <= user.authenticated) {
    user.authenticated |= AUTH_LEVEL3;
    user.user = *it++;
    it++;
    it++;
    user.real = *it;
    outMessages_[cs].append("-- welcome Sir-[").append(user.real).append("]. --\n");
  }
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

           ERR_NEEDMOREPARAMS
           ERR_ALREADYREGISTRED

   Example:

   USER guest 0 * :Ronnie Reagan   ; User registering themselves with a
                                   username of "guest" and real name
                                   "Ronnie Reagan".

   USER guest 8 * :Ronnie Reagan   ; User registering themselves with a
                                   username of "guest" and real name
                                   "Ronnie Reagan", and asking to be set
                                   invisible.
*/
