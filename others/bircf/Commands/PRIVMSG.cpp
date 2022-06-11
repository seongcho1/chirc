#include "../MessageManager.hpp"
#include <cassert>

void MessageManager::PRIVMSG(int cs, std::vector<std::string> paramsVec, std::string trailing) {
	if (paramsVec.size() != 1) {
		reply(cs, ERR_NORECIPIENT, "PRIVMSG", paramsVec, trailing); //411
		return;
	}
	assert(!paramsVec.empty());
	if (trailing.length() == 0) {
		reply(cs, ERR_NOTEXTTOSEND, "PRIVMSG", paramsVec, trailing); //412
		return;
	}
	std::string msgtarget = paramsVec[0];
	if (msgtarget[0] != '#' &&
		nickFdPair_.find(msgtarget) == nickFdPair_.end()) {
		reply(cs, ERR_NOSUCHNICK, "PRIVMSG", paramsVec, trailing); //401
		return;
	}
	if (msgtarget[0]  == '#' &&
		( msgtarget.length() == 1 || channels_.find(msgtarget) == channels_.end()) ) {
		reply(cs, ERR_NOSUCHNICK, "PRIVMSG", paramsVec, trailing); //401
		return;
	}

	//if cs is an operator, cs can use wildcards (*, ?)
	//SS::matchStringVector(usersVec, pattern) then, usersVec only keeps memebers matched the pattern

	int recipient;
	std::string msg;

	//to user
	if (msgtarget[0] != '#') {
		recipient = nickFdPair_[msgtarget];
		msg = std::string(":" + prefix(cs) + " PRIVMSG " + msgtarget  + " :").append(trailing).append(NEWLINE);
		outMessages_[recipient].append(msg);
	}
	//to channel
	else {
		std::string title = channels_.find(msgtarget)->first;
		Channel channel =  channels_[title];
		std::set<int> member = channel.member;
		if (member.find(cs) == member.end() ||
        (member.size() == 1 &&  *(member.begin()) == cs)) {
			reply(cs, ERR_CANNOTSENDTOCHAN, "PRIVMSG", paramsVec, trailing); //404
			return;
		}
		for (std::set<int>::iterator it = member.begin(); it != member.end(); ++it) {
			recipient = *it;
			if (recipient == cs)
				continue;
			msg = std::string(":" + prefix(cs) + " PRIVMSG " + msgtarget  + " :").append(trailing).append(NEWLINE);
			outMessages_[recipient].append(msg);
		}
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
        ERR_NOTOPLEVEL                  :not in the scope <-server to server
        ERR_WILDTOPLEVEL                :not in the scope <-server to server
        ERR_TOOMANYTARGETS              :
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
