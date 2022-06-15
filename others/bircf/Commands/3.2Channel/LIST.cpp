#include "../../MessageManager.hpp"

void MessageManager::LIST(int cs, std::vector<std::string> paramsVec) {
  paramsVec.push_back(std::string());
  paramsVec.push_back(std::string());
  paramsVec.push_back(std::string());

  // ** RPL_LISTSTART
  if (3 < (int)paramsVec.size()) {

// list #1irc
// :*.freenode.net 321 gello2 Channel :Users Name
// :*.freenode.net 322 gello2 #1irc 1 :[+k <key>] 
// :*.freenode.net 323 gello2 :End of channel list.
    //<channel> *( "," <channel> )
    //split paramsVec[0] with COMMA first
    std::vector<std::string> chns = SS::splitString(paramsVec[0], COMMA);
    std::vector<std::string>::iterator it = chns.begin();
    while (it != chns.end()) {
      if (channels_.find(*it) != channels_.end()) {
        paramsVec[0] = *it;
        paramsVec[1] = channels_[*it].member.size();
        paramsVec[2] = channels_[*it].isMode('p') ? "" : channels_[*it].topic;
        reply(cs, RPL_LIST, "LIST", paramsVec);
      }
      ++it;
    }
  }
  else {
    std::map<std::string, Channel>::iterator it = channels_.begin();
    while (it != channels_.end()) {
      paramsVec[0] = it->second.title;
      paramsVec[1] = SS::toString(channels_[paramsVec[0]].member.size());
      paramsVec[2] = channels_[paramsVec[0]].isMode('p') ? "" : channels_[paramsVec[0]].topic;
      reply(cs, RPL_LIST, "LIST", paramsVec);
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
