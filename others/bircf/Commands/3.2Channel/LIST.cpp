#include "../../MessageManager.hpp"

void MessageManager::LIST(int cs, std::vector<std::string> paramsVec) {

  // reply(cs, RPL_LISTSTART, "LIST", paramsVec);
  outMessages_[cs].append("[LIST BEGIN]\n");
  if (paramsVec.size()) {
    std::vector<std::string>::iterator it = paramsVec.begin();
    while (it != paramsVec.end()) {
      if (channels_.find(*it) != channels_.end())
        // reply(cs, RPL_LIST, "LIST", paramsVec);
        outMessages_[cs].append("[").append(channels_[*it].title).append("]:[").append(channels_[*it].topic).append("]\n");
      ++it;
    }
  }
  else {
    std::map<std::string, Channel>::iterator it = channels_.begin();
    while (it != channels_.end()) {
      // reply(cs, RPL_LIST, "LIST", paramsVec);
      outMessages_[cs].append("[").append(it->second.title).append("]:[").append(it->second.topic).append("]\n");
      ++it;
    }
  }

  // reply(cs, RPL_LISTEND, "LIST", paramsVec);
  outMessages_[cs].append("[LIST END]\n");
}


/*
https://datatracker.ietf.org/doc/html/rfc2812#section-3.2.6
3.2.6 List message

      Command: LIST
   Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]

   The list command is used to list channels and their topics.  If the
   <channel> parameter is used, only the status of that channel is
   displayed.

   If the <target> parameter is specified, the request is forwarded to
   that server which will generate the reply.

   Wildcards are allowed in the <target> parameter.

   Numeric Replies:

           ERR_TOOMANYMATCHES
           ERR_NOSUCHSERVER
           RPL_LIST
           RPL_LISTEND

   Examples:

   LIST                            ; Command to list all channels.

   LIST #twilight_zone,#42         ; Command to list channels
                                   #twilight_zone and #42
*/
