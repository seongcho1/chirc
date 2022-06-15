#include "../../MessageManager.hpp"

void MessageManager::NAMES(int cs, std::vector<std::string> paramsVec) {
  if (paramsVec.size()) {

    std::vector<std::string> chn = SS::splitString(paramsVec[0], COMMA);
    std::vector<std::string>::iterator it = chn.begin();
    while (it != chn.end()) {

      if (channels_.find(*it) != channels_.end()) {
        announceOneUser(cs, std::string(users_[cs].srvPrefix() + SS::toString(RPL_NAMREPLY) + " " + users_[cs].nick + " @ " + *it + " :" + channelMemberToString(*it)));
        announceOneUser(cs, std::string(users_[cs].srvPrefix() + SS::toString(RPL_ENDOFNAMES) + " " + users_[cs].nick + " " + *it + " :End of NAMES list"));
      }
      ++it;
    }
  }
  else {
    std::map<std::string, Channel>::iterator it = channels_.begin();
    while (it != channels_.end()) {
      announceOneUser(cs, std::string(users_[cs].srvPrefix() + SS::toString(RPL_NAMREPLY) + " " + users_[cs].nick + " @ " + it->second.title + " :" + channelMemberToString(it->second.title)));
      announceOneUser(cs, std::string(users_[cs].srvPrefix() + SS::toString(RPL_ENDOFNAMES) + " " + users_[cs].nick + " " + it->second.title + " :End of NAMES list"));
      ++it;
    }
  }
}

/*

3.2.5 Names message

      Command: NAMES
   Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]

   By using the NAMES command, a user can list all nicknames that are
   visible to him. For more details on what is visible and what is not,
   see "Internet Relay Chat: Channel Management" [IRC-CHAN].  The
   <channel> parameter specifies which channel(s) to return information
   about.  There is no error reply for bad channel names.

   If no <channel> parameter is given, a list of all channels and their
   occupants is returned.  At the end of this list, a list of users who
   are visible but either not on any channel or not on a visible channel
   are listed as being on `channel' "*".

   If the <target> parameter is specified, the request is forwarded to
   that server which will generate the reply.

   Wildcards are allowed in the <target> parameter.

   Numerics:

           ERR_TOOMANYMATCHES
           ERR_NOSUCHSERVER
           RPL_NAMREPLY           :353
           RPL_ENDOFNAMES         :366


   Examples:

   NAMES #twilight_zone,#42        ; Command to list visible users on
                                   #twilight_zone and #42

   NAMES                           ; Command to list all visible
                                   channels and users

*/
