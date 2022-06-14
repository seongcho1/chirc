#include "../../MessageManager.hpp"

void MessageManager::KICK(int cs, std::vector<std::string> paramsVec) {

  if (paramsVec.size() < 2) {
    reply(cs, ERR_NEEDMOREPARAMS, "KICK", paramsVec);
    return;
  }

// change to loop

  if (channels_.find(paramsVec[0]) == channels_.end()) {
    reply(cs, ERR_NOSUCHCHANNEL, "KICK", paramsVec);
    return;
  }

  if (users_[cs].engaged.find(paramsVec[0]) == users_[cs].engaged.end()) {
    reply(cs, ERR_NOTONCHANNEL, "KICK", paramsVec);
    return;
  }

  std::map<std::string, int>::iterator nfit = nickFdPair_.find(paramsVec[1]);
  if (nfit == nickFdPair_.end()) {
    reply(cs, ERR_NOTONCHANNEL, "KICK", paramsVec);
    return;
  }

  if (channels_[paramsVec[0]].member.find(nfit->second) == channels_[paramsVec[0]].member.end()) {
    reply(cs, ERR_NOTONCHANNEL, "KICK", paramsVec);
    return;
  }

  if (channels_[paramsVec[0]].channelOperators.find(cs) == channels_[paramsVec[0]].channelOperators.end()) {
    reply(cs, ERR_CHANOPRIVSNEEDED, "KICK", paramsVec);
    return;
  }

  users_[nfit->second].engaged.erase(paramsVec[0]);
  channels_[paramsVec[0]].member.erase(nfit->second);

  reply(cs, RPL_AWAY, "KICK", paramsVec);
  reply(nfit->second, RPL_AWAY, "KICK", paramsVec);
}


/*

https://datatracker.ietf.org/doc/html/rfc2812#section-3.2.8
3.2.8 Kick command

      Command: KICK
   Parameters: <channel> *( "," <channel> ) <user> *( "," <user> )
               [<comment>]

   The KICK command can be used to request the forced removal of a user
   from a channel.  It causes the <user> to PART from the <channel> by
   force.  For the message to be syntactically correct, there MUST be
   either one channel parameter and multiple user parameter, or as many
   channel parameters as there are user parameters.  If a "comment" is
   given, this will be sent instead of the default message, the nickname
   of the user issuing the KICK.

   The server MUST NOT send KICK messages with multiple channels or
   users to clients.  This is necessarily to maintain backward
   compatibility with old client software.

   Numeric Replies:

           ERR_NEEDMOREPARAMS
           ERR_NOSUCHCHANNEL
           ERR_BADCHANMASK
           ERR_CHANOPRIVSNEEDED
           ERR_USERNOTINCHANNEL
           ERR_NOTONCHANNEL

   Examples:

   KICK &Melbourne Matthew         ; Command to kick Matthew from
                                   &Melbourne

   KICK #Finnish John :Speaking English
                                   ; Command to kick John from #Finnish
                                   using "Speaking English" as the
                                   reason (comment).

   :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
                                   ; KICK message on channel #Finnish
                                   from WiZ to remove John from channel
*/
