#include "../../MessageManager.hpp"

void MessageManager::PASS(int cs, std::vector<std::string> paramsVec, std::string trailing) {

  if (paramsVec.size() != 1 || !trailing.empty()) {
    reply(cs, ERR_NEEDMOREPARAMS, "PASS", paramsVec, trailing); //461
    return;
  }

  if (users_[cs].authenticated & AUTH_LEVEL1) {
    reply(cs, ERR_ALREADYREGISTRED, "PASS", paramsVec, trailing); //462
    return;
  }
    return;

  if (*paramsVec.begin() == pass) {
    users_[cs].authenticated |= AUTH_LEVEL1;
    //welcome msg will be sent after nick (+ user)
    //outMessages_[cs].append("-- welcome [SOMEBODY], setup nickname please --\n");
  }
  //else
  //  outMessages_[cs].append("** WRONG PASSWORD! **\n");
}


/*
https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.1
3.1.1 Password message

      Command: PASS
   Parameters: <password>

   The PASS command is used to set a 'connection password'.  The
   optional password can and MUST be set before any attempt to register
   the connection is made.  Currently this requires that user send a
   PASS command before sending the NICK/USER combination.

   Numeric Replies:

           ERR_NEEDMOREPARAMS                 :461 done
           ERR_ALREADYREGISTRED               :462 done

   Example:

           PASS secretpasswordhere

*/
