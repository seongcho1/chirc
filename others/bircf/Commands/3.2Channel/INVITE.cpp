#include "../../MessageManager.hpp"

void MessageManager::INVITE(int cs, std::vector<std::string> paramsVec) {
// join #1i
// :*.freenode.net 473 gello2 #1i :Cannot join channel (invite only)
// invite
// :*.freenode.net 337 gello2 :End of INVITE list
// :gello!~a@freenode-ca7.4sl.2765s3.IP INVITE gello2 :#1i
// invite
// :*.freenode.net 336 gello2 :#1i
// :*.freenode.net 337 gello2 :End of INVITE list
  if (paramsVec.size() == 0) {
    
    if (0 < users_[cs].invited.size()) {
      std::string inv(users_[cs].srvPrefix() + users_[cs].nick);
      std::set<std::string>::iterator invited = users_[cs].invited.begin();
      while (invited != users_[cs].invited.end())
        inv.append(*invited++).append(" ");
      announceOneUser(cs, inv);
    }

    std::string end(users_[cs].srvPrefix() + users_[cs].nick + " :End of INVITE list");
    announceOneUser(cs, end);
    return;
  }

  if (paramsVec.size() < 2) {
    reply(cs, ERR_NEEDMOREPARAMS, "INVITE", paramsVec);
    return;
  }

  // std::string nick = paramsVec[0];
  std::string channel = paramsVec[1];
  std::map<std::string, int>::iterator nickfdit = nickFdPair_.find(paramsVec[0]);

  if (nickfdit == nickFdPair_.end()) {
    reply(cs, ERR_NOSUCHNICK, "INVITE", paramsVec);
    return;
  }

  if (users_[cs].engaged.find(paramsVec[1]) == users_[cs].engaged.end()) {
    reply(cs, ERR_NOTONCHANNEL, "INVITE", paramsVec);
    return;
  }

  if (users_[nickfdit->second].engaged.find(paramsVec[1]) != users_[nickfdit->second].engaged.end()) {
    reply(cs, ERR_USERONCHANNEL, "INVITE", paramsVec);
    return;
  }

  if (channels_[paramsVec[1]].unableFlag(cs, 'i')) {
    reply(cs, ERR_CHANOPRIVSNEEDED, "INVITE", paramsVec);
    return;
  }

// inv
// :*.freenode.net 341 gello2 gello :#3irc
// recv
// :gello!~a@freenode-ca7.4sl.2765s3.IP INVITE gello2 :#1i

  users_[nickfdit->second].invited.insert(paramsVec[1]);
  reply(cs, RPL_INVITING, "INVITE", paramsVec);
  announceOneUser(nickfdit->second, std::string(users_[cs].cmdPrefix("INVITE") + paramsVec[0] + " :" + paramsVec[1]));
  if (!users_[nickfdit->second].away.empty()) {
    paramsVec[1] = users_[nickfdit->second].away;
    reply(cs, RPL_AWAY, "AWAY", paramsVec);
  }
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
