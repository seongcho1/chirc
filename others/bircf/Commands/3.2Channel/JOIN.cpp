#include "../../MessageManager.hpp"

void MessageManager::JOIN(int cs, std::vector<std::string> paramsVec) {

  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "JOIN", paramsVec); //461
    return;
  }

  if (USER_ENGAGED_LIMIT <= users_[cs].engaged.size()) {
    reply(cs, ERR_TOOMANYCHANNELS, "JOIN", paramsVec);
    return;
  }

  std::vector<std::string> channels = SS::splitString(paramsVec[0], COMMA);
  std::vector<std::string> keys;
  std::vector<std::string>::iterator it = channels.begin();

  if (1 < paramsVec.size())
    keys = SS::splitString(paramsVec[1], COMMA);

  for (int i = 0; i < (int)channels.size(); ++i) {
    if (channels_.find(*it) == channels_.end()) {
      channels_[*it] = Channel(cs, *it);
      users_[cs].engaged.insert(*it);

      if (i < (int)keys.size() && keys[i] != "") {
        channels_[*it].key = keys[i];
        channels_[*it].setMode(true, 'k');
        // std::string s = "+k";
        // MODE(cs, SS::splitString(s, ""));
      }

SS::charPrint(*it);

    }
    else {
      if (channels_[*it].member.find(cs) == channels_[*it].member.end()) {
        if (channels_[*it].isMode('k') && ((int)keys.size() <= i || channels_[*it].key != keys[i])) {
          reply(cs, ERR_BADCHANNELKEY, "JOIN", paramsVec);
          continue;
        }

        if (CHANNEL_MEMBER_LIMIT <= channels_[*it].member.size()) {
          reply(cs, ERR_CHANNELISFULL, "JOIN", paramsVec);
          continue;
        }

        if (channels_[*it].isMode('i') &&
            users_[cs].invited.find(*it) == users_[cs].invited.end()) {
          reply(cs, ERR_INVITEONLYCHAN, "JOIN", paramsVec);
          continue;
        }

        channels_[*it].member.insert(cs);
        users_[cs].engaged.insert(*it);
        users_[cs].invited.erase(*it);
      }
    }
    announceToChannel(cs, channels[i], std::string().append("join [").append(users_[cs].nick).append("]"));
    reply(cs, TEST, "JOIN", channels); // display topic
    reply(cs, TEST, "JOIN", channels); // display memberlist include me.
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
