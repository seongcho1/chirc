#include "../../MessageManager.hpp"

void MessageManager::PART(int cs, std::vector<std::string> paramsVec) {
  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "PART", paramsVec); //461
    return;
  }

  // if (!users_[cs].isAuthenticated()) {
  //   reply(cs, ERR_NOTREGISTERED, "PART", paramsVec);
  //   return;
  // }

  std::vector<std::string>::iterator it = paramsVec.begin();
  while (it != paramsVec.end()) {
    if (channels_.find(*it) == channels_.end()) {
      reply(cs, ERR_NOSUCHCHANNEL, "PART", paramsVec);
    }
    else if (!channels_[*it].leave(cs)) {
      reply(cs, ERR_NOTONCHANNEL, "PART", paramsVec);
    }
    else {
      outMessages_[cs].append("leaves room [" + *it + "]\n");
      channels_[*it].leave(cs);

      if (channels_[*it].member.size() == 0) {
        channels_.erase(*it);
      }
      else {
        std::set<int>::iterator mit = channels_[*it].member.begin();
        while (mit != channels_[*it].member.end()) {
          outMessages_[*mit++].append("leaves [" + users_[cs].nick + "]\n");
        }
      }

    }
    ++it;
  }
}

/*
https://datatracker.ietf.org/doc/html/rfc2812#section-3.2.7
3.2.2 Part message

      Command: PART
   Parameters: <channel> *( "," <channel> ) [ <Part Message> ]

   The PART command causes the user sending the message to be removed
   from the list of active members for all given channels listed in the
   parameter string.  If a "Part Message" is given, this will be sent
   instead of the default message, the nickname.  This request is always
   granted by the server.

   Servers MUST be able to parse arguments in the form of a list of
   target, but SHOULD NOT use lists when sending PART messages to
   clients.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_NOTONCHANNEL

   Examples:

   PART #twilight_zone             ; Command to leave channel
                                   "#twilight_zone"

   PART #oz-ops,&group5            ; Command to leave both channels
                                   "&group5" and "#oz-ops".

   :WiZ!jto@tolsun.oulu.fi PART #playzone :I lost
                                   ; User WiZ leaving channel
                                   "#playzone" with the message "I
                                   lost".

*/
