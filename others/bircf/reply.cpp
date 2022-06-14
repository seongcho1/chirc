#include "MessageManager.hpp"
#include <fstream>
#include <string>

int    MessageManager::initReplies(std::string configFile) {

  std::ifstream fileStream(configFile);

  if ( fileStream.fail() )
    return -1;

  std::string line;
  while (std::getline(fileStream, line))  {
    std::istringstream is_line(line);

    std::string valueLine;
    if (!std::getline(is_line, valueLine, '#'))
      continue;

    std::string key;
    std::string value;

    std::vector<std::string> valueVec = SS::splitString(valueLine, "=", false, false, true);

    if ( (valueVec.size() != 2)  ||
         (valueVec[0].empty() && valueVec[1].empty()) )
      continue;

    key = SS::trim(valueVec[0]);
    value = SS::trim(valueVec[1]);

    // replies_[std::stoi(key)] = value; //c++11
    replies_[atoi(key.c_str())] = value;
  }
  return 0;
}

void  MessageManager::reply(int cs, int code, std::string command, std::vector<std::string> paramsVec) {

  std::map<int, std::string>::iterator it = replies_.find(code);
  if (it == replies_.end()) {
    return;
  }

  std::string msg = it->second;
  std::vector<std::string>      sVec;
  std::vector<std::string>      rVec;

  switch (code) {

    case 0                       :  sVec.push_back("<...>"); rVec.push_back(paramsVec[0]);
                                    break; // TEST 0

    //001   = Welcome to the <network> Network, <nick>[!<user>@<host>]
    case  RPL_WELCOME             : sVec.push_back("<network>");   rVec.push_back("FT_IRC"); //server's property
                                    if (!users_[cs].user.empty()){
                                      sVec.push_back("<nick>[!<user>@<host>]");
                                      rVec.push_back(std::string(users_[cs].nick + "!" + users_[cs].user + "@" + users_[cs].host));
                                    } else {
                                      sVec.push_back("<nick>[!<user>@<host>]");
                                      rVec.push_back(users_[cs].nick);
                                    }
                                    break;  //001


    //work start

    //work end

    // not defined begin

    //321   = Obsolete. Not used.
    // case RPL_LISTSTART        :  break;  // 321
    //322   = <channel> <# visible> :<topic>
    // case RPL_LIST             :  break;  // 322
    //323   = :End of LIST
    case RPL_LISTEND             :  break;  // 323


//working here

    case RPL_NAMREPLY            :  sVec.push_back("<...>");      rVec.push_back(paramsVec[0]);
                                    break;  // 353
    case RPL_ENDOFNAMES          :  sVec.push_back("<...>");      rVec.push_back(paramsVec[0]);
                                    break;  // 366
    // not defined end


    //401    =  <nickname> :No such nick/channel
    case  ERR_NOSUCHNICK         :  sVec.push_back("<nickname>");      rVec.push_back(paramsVec[0]);
                                    break;  //401
    //402    =  <server name> :No such server
    case  ERR_NOSUCHSERVER       :  sVec.push_back("<server name>");   rVec.push_back(paramsVec[0]);
                                    break;  //402
    //403    =  <channel name> :No such channel
    case  ERR_NOSUCHCHANNEL      :  sVec.push_back("<channel name>");  rVec.push_back(paramsVec[0]);
                                    break;  //403
    //404    =  <channel name> :Cannot send to channel
    case  ERR_CANNOTSENDTOCHAN   :  sVec.push_back("<channel name>");  rVec.push_back(paramsVec[0]);
                                    break;  //404
    //405    =  <channel name> :You have joined too many channels
    case  ERR_TOOMANYCHANNELS    :  sVec.push_back("<channel name>");  rVec.push_back(paramsVec[0]);
                                    break;  //405
    //406    =  <nickname> :There was no such nickname
    case  ERR_WASNOSUCHNICK      :  sVec.push_back("<nickname>");      rVec.push_back(paramsVec[0]);
                                    break;  //406
    //407    =  <target> :<error code> recipients. <abort message>
    case  ERR_TOOMANYTARGETS     :  // sVec.push_back("<target>");      rVec.push_back(users_[cs].nick);        ???
                                    sVec.push_back("<error code>");    rVec.push_back(SS::toString(code));
                                    // sVec.push_back("<abort message>");rVec.push_back(trailing);              ???
                                    break;  //407
    //408    =  <service name> :No such service
    case  ERR_NOSUCHSERVICE      :  // sVec.push_back("<service name>");  rVec.push_back(??);                    ???
                                    break;  //408
    //409    =  :No origin specified
    case  ERR_NOORIGIN           :  break;  //409



    //411    =  :No recipient given (<command>)
    case  ERR_NORECIPIENT        :  sVec.push_back("<command>");      rVec.push_back(command);
                                    break;  //411
    //412    =  :No text to send
    case  ERR_NOTEXTTOSEND       :  break;  //412
    //413    =  <mask> :No toplevel domain specified
    // case  ERR_NOTOPLEVEL      :  // sVec.push_back("<mask>");            rVec.push_back(??);
    //                              break;  //413 not in the scope <-server to server
    //414    =  <mask> :Wildcard in toplevel domain
    // case  ERR_WILDTOPLEVEL    :  // sVec.push_back("<mask>");            rVec.push_back(??);
    //                              break;  //414 not in the scope <-server to server
    //415    =  <mask> :Bad Server/host mask
    // case  ERR_BADMASK         :  // sVec.push_back("<mask>");            rVec.push_back(??);
    //                              break;  //415 not in the scope <-server to server


    //421    =  <command> :Unknown command
    case  ERR_UNKNOWNCOMMAND     :  sVec.push_back("<command>");      rVec.push_back(paramsVec[0]);
                                    break;  //421
    //422    =  :MOTD File is missing
    case  ERR_NOMOTD             :  break;  //422
    //423    =  <server> :No administrative info available
    case  ERR_NOADMININFO        :  // sVec.push_back("<server>");          rVec.push_back(??);                    ???
                                    break;  //423
    //424    =  :File error doing <file op> on <file>
    case  ERR_FILEERROR          :  // sVec.push_back("<file op>");        rVec.push_back(??);                    ???
                                    // sVec.push_back("<file>");            rVec.push_back(??);                    ???
                                    break;  //424


    //431    =  :No nickname given
    case  ERR_NONICKNAMEGIVEN    :  break;  //431
    //432    =  <nick> :Erroneous nickname
    case  ERR_ERRONEUSNICKNAME   :  sVec.push_back("<nick>");          rVec.push_back(paramsVec[0]);
                                    break;  //432
    //433    =  <nick> :Nickname is already in use
    case  ERR_NICKNAMEINUSE      :  sVec.push_back("<nick>");          rVec.push_back(paramsVec[0]);
                                    break;  //433
    //436    =  <nick> :Nickname collision KILL from <user>@<host>
    // case  ERR_NICKCOLLISION   :  break;  //436 not in the scope <-server to server
    //437    =  <nick/channel> :Nick/channel is temporarily unavailable
    case  ERR_UNAVAILRESOURCE    :  sVec.push_back("<nick/channel>");  rVec.push_back(paramsVec[0]);
                                    break;  //437


    //441    =  <nick> <channel> :They are not on that channel
    case  ERR_USERNOTINCHANNEL   :  // sVec.push_back("<nick>");          rVec.push_back(??);                    ???
                                    // sVec.push_back("<channel>");      rVec.push_back(??);                    ???
                                    break;  //441
    //442    =  <channel> :You are not on that channel
    case  ERR_NOTONCHANNEL       :  sVec.push_back("<channel>");      rVec.push_back(paramsVec[0]);
                                    break;  //442
    //443    =  <user> <channel> :is already on channel
    case  ERR_USERONCHANNEL      :  // sVec.push_back("<user>");            rVec.push_back(paramsVec[0]);          ???
                                    // sVec.push_back("<channel>");        rVec.push_back(paramsVec[1]);          ???
                                    break;  //443
    //444    =  <user> :User not logged in
    case  ERR_NOLOGIN            :  sVec.push_back("<user>");          rVec.push_back(paramsVec[0]);
                                    break;  //444
    //445    =  :SUMMON has been disabled
    case  ERR_SUMMONDISABLED     :  break;  //445
    //446    =  :USERS has been disabled
    case  ERR_USERSDISABLED      :  break;  //446


    //451    =  :You have not registered
    case  ERR_NOTREGISTERED      :  break;  //451


    //461    =  <command> :Not enough parameters
    case  ERR_NEEDMOREPARAMS     :  sVec.push_back("<command>");      rVec.push_back(command);
                                    break;  //461
    //462    =  :Unauthorized command (already registered)
    case  ERR_ALREADYREGISTRED   :  break;  //462
    //463    =  :Your host is not among the privileged
    case  ERR_NOPERMFORHOST      :  break;  //463
    //464    =  :Password incorrect
    case  ERR_PASSWDMISMATCH     :  break;  //464
    //465    =  :You are banned from this server
    case  ERR_YOUREBANNEDCREEP   :  break;  //465
    //466    =  ERR_YOUWILLBEBANNED
    case  ERR_YOUWILLBEBANNED    :  break;  //466
    //467    =  <channel> :Channel key already set
    case  ERR_KEYSET             :  sVec.push_back("<channel>");  rVec.push_back(paramsVec[0]);
                                    break;  //467


    //471    =  <channel> :Cannot join channel (+l)
    case  ERR_CHANNELISFULL      :  sVec.push_back("<channel>");  rVec.push_back(paramsVec[0]);
                                    break;  //471
    //472    =  <char> :is unknown mode char to me for <channel>
    case  ERR_UNKNOWNMODE        :  // sVec.push_back("<char>");      rVec.push_back(??);                    ???
                                    // sVec.push_back("<channel>");  rVec.push_back(??);                    ???
                                    break;  //472
    //473    =  <channel> :Cannot join channel (+i)
    case  ERR_INVITEONLYCHAN    :   sVec.push_back("<channel>");  rVec.push_back(paramsVec[0]);
                                    break;  //473
    //474    =  <channel> :Cannot join channel (+b)
    case  ERR_BANNEDFROMCHAN    :   sVec.push_back("<channel>");  rVec.push_back(paramsVec[0]);
                                    break;  //474
    //475    =  <channel> :Cannot join channel (+k)
    case  ERR_BADCHANNELKEY      :  sVec.push_back("<channel>");  rVec.push_back(paramsVec[0]);
                                    break;  //475
    //476    =  <channel> :Bad Channel Mask
    case  ERR_BADCHANMASK        :  sVec.push_back("<channel>");  rVec.push_back(paramsVec[0]);
                                    break;  //476
    //477    =  <channel> :Channel does not support modes
    case  ERR_NOCHANMODES        :  sVec.push_back("<channel>");  rVec.push_back(paramsVec[0]);
                                    break;  //477
    //478    =  <channel> <char> :Channel list is full
    case  ERR_BANLISTFULL        :  sVec.push_back("<channel>");  rVec.push_back(paramsVec[0]);
                                    break;  //478


    //481    =  :Permission Denied- You are not an IRC operator
    case  ERR_NOPRIVILEGES       :  break;  //481
    //482    =  <channel> :You are not channel operator
    case  ERR_CHANOPRIVSNEEDED   :  sVec.push_back("<channel>");  rVec.push_back(paramsVec[0]);
                                    break;  //482
    //483    =  :You can not kill a server!
    case  ERR_CANTKILLSERVER     :  break;  //483
    //484    =  :Your connection is restricted!
    case  ERR_RESTRICTED         :  break;  //484
    //485    =  :You are not the original channel operator
    case  ERR_UNIQOPPRIVSNEEDED  :  break;  //485


    //491    =  :No O-lines for your host
    case  ERR_NOOPERHOST         :  break;  //491


    //501    =  :Unknown MODE flag
    case  ERR_UMODEUNKNOWNFLAG   :  break;  //501
    //502    =  :Cannot change mode for other users
    case  ERR_USERSDONTMATCH     :  break;  //502

    default                      :  break;
  }

  SS::replaceString(msg, sVec, rVec);
  msg = std::string(":FT_IRC " + SS::toString(code, 3) + " " + users_[cs].nick + " ").append(msg).append(NEWLINE);
  outMessages_[cs].append(msg);
}


