#include "../../MessageManager.hpp"

void MessageManager::JOIN(int cs, std::vector<std::string> paramsVec) {
  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "JOIN", paramsVec); //461
    return;
  }

  // if (!users_[cs].isAuthenticated()) {
  //   reply(cs, ERR_NOTREGISTERED, "PART", paramsVec);
  //   return;
  // }

  std::vector<std::string>::iterator it = paramsVec.begin();
  while (it != paramsVec.end()) {

    if (channels_.find(*it) == channels_.end()) {
      channels_[*it] = Channel(*paramsVec.begin());
      // channels_[*it].title = *paramsVec.begin();
      channels_[*it].channelOperators.insert(cs);
    }

    if (channels_[*it].member.find(cs) == channels_[*it].member.end()) {
      users_[cs].invited.erase(*it);

      std::set<int>::iterator mit = channels_[*it].member.begin();
      while (mit != channels_[*it].member.end()) {
        outMessages_[*mit++].append("join [" + users_[cs].nick + "]\n");
      }

      channels_[*it].member.insert(cs);
      users_[cs].engaged.insert(*it);
    }

    ++it;
  }
}

/*
https://datatracker.ietf.org/doc/html/rfc2813#section-4.2.1
4.2.1 Join message

      Command: JOIN
   Parameters: <channel>[ %x7 <modes> ]
               *( "," <channel>[ %x7 <modes> ] )

   The JOIN command is used by client to start listening a specific
   channel. Whether or not a client is allowed to join a channel is
   checked only by the local server the client is connected to; all
   other servers automatically add the user to the channel when the
   command is received from other servers.

   Optionally, the user status (channel modes 'O', 'o', and 'v') on the
   channel may be appended to the channel name using a control G (^G or
   ASCII 7) as separator.  Such data MUST be ignored if the message
   wasn't received from a server.  This format MUST NOT be sent to
   clients, it can only be used between servers and SHOULD be avoided.

   The JOIN command MUST be broadcast to all servers so that each server
   knows where to find the users who are on the channel.  This allows
   optimal delivery of PRIVMSG and NOTICE messages to the channel.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
           ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
           ERR_CHANNELISFULL               ERR_BADCHANMASK
           ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
           ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
           RPL_TOPIC

   Examples:

   :WiZ JOIN #Twilight_zone        ; JOIN message from WiZ
*/
