#include "../../MessageManager.hpp"

void MessageManager::PASS(int cs, std::vector<std::string> paramsVec) {

  if (users_[cs].authenticated & AUTH_LEVEL1) {
    reply(cs, ERR_ALREADYREGISTRED, "PASS", paramsVec); //462
    return;
  }

  if (paramsVec.size() != 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "PASS", paramsVec); //461
    return;
  }

  if (*paramsVec.begin() == pass) {
    users_[cs].authenticated |= AUTH_LEVEL1;

  //  The PASS command is used to set a 'connection password'.  The
  //  optional password can and MUST be set before any attempt to register
  //  the connection is made.  Currently this requires that user send a
  //  PASS command before sending the NICK/USER combination.
    // if (users_[cs].authenticated == AUTH_MASK)
    //   reply(cs, RPL_WELCOME, "PASS", paramsVec); //001
  }
  else
    reply(cs, ERR_PASSWDMISMATCH, "PASS", paramsVec); //464

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
