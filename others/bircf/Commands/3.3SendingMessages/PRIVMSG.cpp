#include "../../MessageManager.hpp"
#include <cassert>

void  MessageManager::NOTICE(int cs, std::vector<std::string> paramsVec) {
  SENDMSG(cs, paramsVec, true);
}

void  MessageManager::PRIVMSG(int cs, std::vector<std::string> paramsVec) {
  SENDMSG(cs, paramsVec, false);
}


void  MessageManager::SENDMSG(int cs, std::vector<std::string> paramsVec, bool isNotice) {

  if ( paramsVec.size() == 1 ||
       (paramsVec.size() == 2 && paramsVec[1].empty()) ) {
    if (!isNotice)
      reply(cs, ERR_NOTEXTTOSEND, "PRIVMSG", paramsVec); //412
    return;
  }

  if ( paramsVec.size() == 0 ||
       (paramsVec.size() == 1 && paramsVec[0].empty()) ) {
    if (!isNotice)
      reply(cs, ERR_NORECIPIENT, "PRIVMSG", paramsVec); //411
    return;
  }
  std::string msgtarget = paramsVec[0];
  std::string msg = paramsVec[1];
  std::vector<std::string> msgtoVec = SS::splitString(msgtarget, COMMA);

  //duplicated msgto
  std::string duplicatedMsgtos = SS::duplicateWordString(msgtoVec);
  if (duplicatedMsgtos.length()) {
    paramsVec[0] = duplicatedMsgtos;
    paramsVec[1] = "Duplicate recipients. No message delivered";
    if (!isNotice)
      reply(cs, ERR_TOOMANYTARGETS, "PRIVMSG", paramsVec); //407
    return;
  }

  std::vector<std::string>::iterator it;
  for(it = msgtoVec.begin(); it != msgtoVec.end(); ++it) {
    std::string msgto = *it;
    PRIVMSGHelper(cs, msgto, msg, isNotice);
  }

}

void  MessageManager::PRIVMSGHelper(int cs, const std::string& msgto, const std::string& msg, bool isNotice) {
  std::vector<std::string> paramsVec;
  paramsVec.push_back(msgto);
  paramsVec.push_back("");

  if (msgto[0] != '#' &&
    nickFdPair_.find(msgto) == nickFdPair_.end()) {
    if (!isNotice)
      reply(cs, ERR_NOSUCHNICK, "PRIVMSG", paramsVec); //401
    return;
  }
  if (msgto[0]  == '#' &&
    ( msgto.length() == 1 || channels_.find(msgto) == channels_.end()) ) {
    if (!isNotice)
      reply(cs, ERR_NOSUCHNICK, "PRIVMSG", paramsVec); //401
    return;
  }

  //seongcho: dealing with an operator is out of scope
  //if cs is an operator, cs can use wildcards (*, ?)
  //SS::matchStringVector(usersVec, pattern) then, usersVec only keeps memebers matched the pattern

  int recipient;
  std::string message = std::string(":" + hostmask(cs) + " PRIVMSG " + msgto  + " :").append(msg).append(NEWLINE);

  //to user
  if (msgto[0] != '#') {
    recipient = nickFdPair_[msgto];
    users_[recipient].wbuff.append(message);

    if (!users_[recipient].away.empty()) {
      paramsVec[1] = users_[recipient].away;
      if (!isNotice)
        reply(cs, RPL_AWAY, "AWAY", paramsVec);
    }
    // if (!users_[nickfdit->second].away.empty()) {
    //   paramsVec[1] = users_[nickfdit->second].away;
    //   reply(cs, RPL_AWAY, "AWAY", paramsVec);
    // }

  }
  //to channel
  //seongcho: need to check channel modes
  else {
    std::string title = channels_.find(msgto)->first;
    Channel channel =  channels_[title];
    std::set<int> member = channel.member;
    if ((member.size() == 1 &&  *(member.begin()) == cs) ||
        (channel.isMode('n') && member.find(cs) == member.end()) ||
        (channel.isMode('m') && channel.channelSpeaker.find(cs) == channel.channelSpeaker.end())) {
      if (!isNotice)
        reply(cs, ERR_CANNOTSENDTOCHAN, "PRIVMSG", paramsVec); //404
      return;
    }

    announceToChannel(cs, channel.title, message);
  }


}


/*
https://datatracker.ietf.org/doc/html/rfc2812#section-3.3.1
3.3.1 Private messages
 Command: PRIVMSG
   Parameters: <msgtarget> <text to be sent>

   PRIVMSG is used to send private messages between users, as well as to
   send messages to channels.  <msgtarget> is usually the nickname of
   the recipient of the message, or a channel name.

   The <msgtarget> parameter may also be a host mask (#<mask>) or server
   mask ($<mask>).  In both cases the server will only send the PRIVMSG
   to those who have a server or host matching the mask.  The mask MUST
   have at least 1 (one) "." in it and no wildcards following the last
   ".".  This requirement exists to prevent people sending messages to
   "#*" or "$*", which would broadcast to all users.  Wildcards are the
   '*' and '?'  characters.  This extension to the PRIVMSG command is
   only available to operators.

   Numeric Replies:

        ERR_NORECIPIENT                 :411 done
        ERR_NOTEXTTOSEND                :412 done
        ERR_CANNOTSENDTOCHAN            :404 done
        ERR_NOTOPLEVEL                  :413 not in the scope <-server to server
        ERR_WILDTOPLEVEL                :414 not in the scope <-server to server
        ERR_TOOMANYTARGETS              :407 done how many is too many??? go with 15
        ERR_NOSUCHNICK                  :401 done
        RPL_AWAY                        :

   Examples:

   :Angel!wings@irc.org PRIVMSG Wiz :Are you receiving this message ?
                                   ; Message from Angel to Wiz.

   PRIVMSG Angel :yes I'm receiving it !
                                   ; Command to send a message to Angel.

   PRIVMSG jto@tolsun.oulu.fi :Hello !
                                   ; Command to send a message to a user
                                   on server tolsun.oulu.fi with
                                   username of "jto".

   PRIVMSG kalt%millennium.stealth.net@irc.stealth.net :Are you a frog?
                                   ; Message to a user on server
                                   irc.stealth.net with username of
                                   "kalt", and connected from the host
                                   millennium.stealth.net.

   PRIVMSG kalt%millennium.stealth.net :Do you like cheese?
                                   ; Message to a user on the local
                                   server with username of "kalt", and
                                   connected from the host
                                   millennium.stealth.net.

   PRIVMSG Wiz!jto@tolsun.oulu.fi :Hello !
                                   ; Message to the user with nickname
                                   Wiz who is connected from the host
                                   tolsun.oulu.fi and has the username
                                   "jto".

   PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
                                   ; Message to everyone on a server
                                   which has a name matching *.fi.

   PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
                                   ; Message to all users who come from
                                   a host which has a name matching
                                   *.edu.
*/

/*
host is my server
server is another server which is connected to the host
the terms (host, server) is out of scope
*/

/*
What Is A Hostmask?
When you connect to GeekShed, the server reads information from your client and sets a hostmask
that is used to identify you on the network.
Perhaps obviously, your hostmask hides (or masks) some of the specifics
about your connection (in other words, your host).

Your hostmask will take this general format:
nick!user@host

For example, here’s the hostmask for the LordBaconCheeseburger:
LordBaconCheeseburger!~TFlash@protectedhost-DC62ACB5.hsd1.va.comcast.net

For that example:
nick = LordBaconCheeseburger

It’s whatever nickname the person is using.

user = ~TFlash
It’s the name set in your client. If there’s a leading tilde (~), the client is not running identd.

host = protectedhost-DC62ACB5.hsd1.va.comcast.net
It’s the masked hostname of your connection. Your unmasked hostname is never publicly available.
*/
