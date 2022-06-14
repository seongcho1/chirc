#include "../../MessageManager.hpp"

void MessageManager::LIST(int cs, std::vector<std::string> paramsVec) {

  // ** RPL_LISTSTART
  if (paramsVec.size()) {

    //<channel> *( "," <channel> )
    //split paramsVec[0] with COMMA first
    std::vector<std::string>::iterator it = paramsVec.begin();
    while (it != paramsVec.end()) {
      if (channels_.find(*it) != channels_.end()) {
        outMessages_[cs].append("[").append(channels_[*it].title).append("]");
        if (!channels_[*it].isMode('p'))
          outMessages_[cs].append(":[").append(channels_[*it].topic).append("]");
        outMessages_[cs].append(NEWLINE);
//        // reply(cs, RPL_LIST, "LIST", paramsVec);
//        // outMessages_[cs].append("[").append(channels_[*it].title).append("]:[").append(channels_[*it].topic).append("]\n"); // ** RPL_LIST
      }
      ++it;
    }
  }
  else {
    std::map<std::string, Channel>::iterator it = channels_.begin();
    while (it != channels_.end()) {
      //322 RPL_LIST   string format "<channel> <# visible> :<topic>"
      //322 RPL_LIST string format "<channel> <# visible> :<topic>"
      //format to send to reply -> #42irc 1 :[+nt] :<topic>
      // reply(cs, RPL_LIST, "LIST", paramsVec);
      outMessages_[cs].append(it->second.title).append(" <# visible> :").append(it->second.topic).append(NEWLINE);
      ++it;
    }
  }

  reply(cs, RPL_LISTEND, "LIST", paramsVec);
}

/*




*/

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
           ERR_NOSUCHSERVER       :402 not in the scope <-server to server
           RPL_LIST               :322
           RPL_LISTEND            :323

   Examples:

   LIST                            ; Command to list all channels.

   LIST #twilight_zone,#42         ; Command to list channels
                                   #twilight_zone and #42
*/
