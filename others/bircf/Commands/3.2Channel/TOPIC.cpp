#include "../../MessageManager.hpp"

void MessageManager::TOPIC(int cs, std::vector<std::string> paramsVec) {

  if (paramsVec.size() < 1) {
    reply(cs, ERR_NEEDMOREPARAMS, "TOPIC", paramsVec);
    return;
  }

  if (paramsVec.size() < 2) {
    outMessages_[cs].append("TOPIC: ").append(channels_[*paramsVec.begin()].topic).append("\n");
    return;
  }

  if (channels_[*paramsVec.begin()].channelCreator != cs &&
      channels_[*paramsVec.begin()].channelOperators.find(cs) == channels_[*paramsVec.begin()].channelOperators.end()) {
    // reply(cs, ERR_BADCHANMASK, "KICK", paramsVec);
    reply(cs, ERR_CHANOPRIVSNEEDED, "TOPIC", paramsVec);
    return;
  }

  if (users_[cs].engaged.find(*paramsVec.begin()) == users_[cs].engaged.end()) {
    reply(cs, ERR_NOTONCHANNEL, "TOPIC", paramsVec);
    return;
  }

  std::string topic;
  std::vector<std::string>::iterator pit = ++paramsVec.begin();
  while (pit != paramsVec.end())
    topic.append(*pit++).append(" ");
  channels_[*paramsVec.begin()].topic = topic;

  std::string message;
  message.append(users_[cs].nick).append(" has set topic: ").append(topic).append("\n");
  announceToChannel(*paramsVec.begin(), message);
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

           ERR_NEEDMOREPARAMS
           ERR_NOTONCHANNEL
           RPL_NOTOPIC
           RPL_TOPIC
           ERR_CHANOPRIVSNEEDED
           ERR_NOCHANMODES

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
