#include "../../MessageManager.hpp"

void MessageManager::NAMES(int cs, std::vector<std::string> paramsVec) {
  // reply(cs, RPL_NAMREPLY, "NAMES", paramsVec);

  outMessages_[cs].append("[NAMES BEGIN]\n");
  if (paramsVec.size()) {
    std::vector<std::string>::iterator it = paramsVec.begin();
    while (it != paramsVec.end()) {

      std::map<std::string, Channel>::iterator cit = channels_.find(*it);

SS::charPrint(*it);

      if (cit != channels_.end())
        
        // reply(cs, RPL_NAMREPLY, "NAMES", namesVec(cit->second.member), trailing);
        outMessages_[cs].append("[").append(channelMemberToString(paramsVec[0])).append("]\n");
      ++it;
    }
  }
  else {
    std::map<std::string, Channel>::iterator it = channels_.begin();
    while (it != channels_.end()) {
      // reply(cs, RPL_LIST, "NAMES", namesVec(it->second.member), trailing);
      outMessages_[cs].append("[").append(channelMemberToString(paramsVec[0])).append("]\n");
      ++it;
    }
  }

  // reply(cs, RPL_ENDOFNAMES, "NAMES", paramsVec, trailing);
  outMessages_[cs].append("[NAMES END]\n");
}

// std::vector<std::string> MessageManager::namesVec(std::set<int> fds) {
//   std::set<int>::iterator si = fds.begin();
//   std::vector<std::string> result;

//   result.reserve(fds.size());

//   while (si != fds.end()) {
//     result.push_back(users_[*si].nick);
//     ++si;
//   }
//   return result;
// }

// std::string MessageManager::namesVec_TEST(std::set<int> fds) {

//   std::set<int>::iterator si = fds.begin();
//   std::string result;

//   while (si != fds.end()) {
//     result.append(users_[*si].nick).append(" ");
//     ++si;
//   }
//   return result;
// }


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
