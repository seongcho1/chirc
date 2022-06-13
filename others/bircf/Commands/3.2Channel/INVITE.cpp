#include "../../MessageManager.hpp"

void MessageManager::INVITE(int cs, std::vector<std::string> paramsVec) {

  if (paramsVec.size() == 0) {
    std::set<std::string>::iterator invited = users_[cs].invited.begin();
    outMessages_[cs].append("invited: ");
    while (invited != users_[cs].invited.end())
      outMessages_[cs].append(*invited++).append(" ");
    outMessages_[cs].append(NEWLINE);
    outMessages_[cs].append("End of INVITE list\n");
    return;
  }

  if (paramsVec.size() < 2) {
    reply(cs, ERR_NEEDMOREPARAMS, "INVITE", paramsVec);
    return;
  }

  std::string channel = *(++paramsVec.begin());
  std::map<std::string, int>::iterator nickfdit = nickFdPair_.find(*paramsVec.begin());

  if (nickfdit == nickFdPair_.end()) {
    reply(cs, ERR_NOSUCHNICK, "INVITE", paramsVec);
    return;
  }

  if (users_[cs].engaged.find(channel) == users_[cs].engaged.end()) {
    reply(cs, ERR_NOTONCHANNEL, "INVITE", paramsVec);
    return;
  }

  if (users_[nickfdit->second].engaged.find(channel) != users_[nickfdit->second].engaged.end()) {
    reply(cs, ERR_USERONCHANNEL, "INVITE", paramsVec);
    return;
  }

  if (channels_[channel].isMode('i') &&
      channels_[channel].channelOperators.find(cs) == channels_[channel].channelOperators.end()) {
    reply(cs, ERR_CHANOPRIVSNEEDED, "INVITE", paramsVec);
    return;
  }

  users_[nickfdit->second].invited.insert(channel);
  announceToSelf(cs, std::string().append("inviting ").append(nickfdit->first).append(" to #").append(channel));
  announceToSelf(nickfdit->second, std::string().append(users_[cs].nick).append(" invite you to #").append(channel));
}

/*
https://datatracker.ietf.org/doc/html/rfc2812#section-3.2.7
3.2.7 Invite message

      Command: INVITE
   Parameters: <nickname> <channel>

   The INVITE command is used to invite a user to a channel.  The
   parameter <nickname> is the nickname of the person to be invited to
   the target channel <channel>.  There is no requirement that the
   channel the target user is being invited to must exist or be a valid
   channel.  However, if the channel exists, only members of the channel
   are allowed to invite other users.  When the channel has invite-only
   flag set, only channel operators may issue INVITE command.

   Only the user inviting and the user being invited will receive
   notification of the invitation.  Other channel members are not
   notified.  (This is unlike the MODE changes, and is occasionally the
   source of trouble for users.)

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_USERONCHANNEL
           ERR_CHANOPRIVSNEEDED
           RPL_INVITING                    RPL_AWAY

   Examples:

   :Angel!wings@irc.org INVITE Wiz #Dust

                                   ; Message to WiZ when he has been
                                   invited by user Angel to channel
                                   #Dust

   INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to
                                   #Twilight_zone
*/
