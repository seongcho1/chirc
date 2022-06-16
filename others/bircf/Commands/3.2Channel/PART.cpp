#include "../../MessageManager.hpp"

void MessageManager::PART(int cs, std::vector<std::string> paramsVec) {
  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "PART", paramsVec); //461
    return;
  }

// :gello2!~1@freenode-n9q.738.2oh04d.IP PART :#1irc
  std::vector<std::string> chns = SS::splitString(paramsVec[0], COMMA);
  std::vector<std::string>::iterator it = chns.begin();
  while (it != chns.end()) {
    if (channels_.find(*it) == channels_.end()) {
      chns[0] = *it;
      reply(cs, ERR_NOSUCHCHANNEL, "PART", chns);
      ++it;
      continue;
    }

    if (channels_[*it].member.find(cs) == channels_[*it].member.end()) {
      chns[0] = *it;
      reply(cs, ERR_NOTONCHANNEL, "PART", chns);
      ++it;
      continue;
    }
    std::string msg(users_[cs].cmdPrefix("PART") + ":" + *it);
    announceToChannel(cs, *it, msg, true);
    channels_[*it].leave(cs);
    users_[cs].engaged.erase(*it);
    if (channels_[*it].member.size() == 0)
      channels_.erase(*it);
    
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
