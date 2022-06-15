#include "../../MessageManager.hpp"

void MessageManager::TOPIC(int cs, std::vector<std::string> paramsVec) {

  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "TOPIC", paramsVec); //461
    return;
  }

// topic #1irc
// :*.freenode.net 332 gello2 #1irc :hello world~
// :*.freenode.net 333 gello2 #1irc gello2!~1@freenode-ca7.4sl.2765s3.IP :1655288888
  if (paramsVec.size() < 2) {
    paramsVec.push_back(channels_[paramsVec[0]].topic);
    reply(cs, RPL_TOPIC, "TOPIC", paramsVec);
    return;
  }

  if (paramsVec[0][0] != '#' ||
      users_[cs].engaged.find(paramsVec[0]) == users_[cs].engaged.end()) {
    reply(cs, ERR_NOTONCHANNEL, "TOPIC", paramsVec); //442
    return;
  }

  if (channels_[paramsVec[0]].unableFlag(cs, 't')) {
    reply(cs, ERR_CHANOPRIVSNEEDED, "TOPIC", paramsVec);
    return;
  }

// topic #1irc hello world~
// :gello2!~1@freenode-ca7.4sl.2765s3.IP TOPIC #1irc :hello world~

// :gello!~a@freenode-ca7.4sl.2765s3.IP TOPIC #1irc :hello haha~2
  std::string topic = SS::makeOneString(++paramsVec.begin(), paramsVec.end());
  channels_[paramsVec[0]].topic = topic;
  
  topic = users_[cs].cmdPrefix("TOPIC") + paramsVec[0] + " :" + topic;
  announceToChannel(cs, paramsVec[0], topic, true);
}


/*
https://datatracker.ietf.org/doc/html/rfc2812#section-3.2.4
3.2.4 Topic message

      Command: TOPIC
   Parameters: <channel> [ <topic> ]

   The TOPIC command is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic>
   given.  If the <topic> parameter is present, the topic for that
   channel will be changed, if this action is allowed for the user
   requesting it.  If the <topic> parameter is an empty string, the
   topic for that channel will be removed.

   Numeric Replies:

           ERR_NEEDMOREPARAMS         :461 done
           ERR_NOTONCHANNEL           :442 done
           RPL_NOTOPIC                :331
           RPL_TOPIC                  :332
           ERR_CHANOPRIVSNEEDED       :482 done
           ERR_NOCHANMODES            :477

   Examples:

   :WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic ; User Wiz setting the
                                   topic.

   TOPIC #test :another topic      ; Command to set the topic on #test
                                   to "another topic".

   TOPIC #test :                   ; Command to clear the topic on
                                   #test.

   TOPIC #test                     ; Command to check the topic for
                                   #test.
*/
