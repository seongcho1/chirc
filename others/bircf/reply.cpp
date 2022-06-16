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

std::string  MessageManager::reply(int cs, int code, std::string command, std::vector<std::string>& paramsVec, bool bDirectDelivery) {

  std::map<int, std::string>::iterator it = replies_.find(code);
  if (it == replies_.end()) {
    return std::string();
  }

  std::string msg = it->second;
  std::vector<std::string>      sVec;
  std::vector<std::string>      rVec;

  switch (code) {

    case TEST                     :  sVec.push_back("<...>"); rVec.push_back(paramsVec[0]);
                                    break; // TEST 0

    //001   = Welcome to the <network> Network, <nick>[!<user>@<host>]

    case  RPL_WELCOME             : sVec.push_back("<network>");              rVec.push_back(users_[cs].serverHostmask()); //server's property ???
                                    sVec.push_back("<nick>[!<user>@<host>]"); rVec.push_back(users_[cs].hostmask());
                                    break;  // 001

    //002   = Your host is <servername>, running version <version>
    case RPL_YOURHOST             : sVec.push_back("<servername>");   rVec.push_back(users_[cs].serverHostmask());        //server's property ???
                                    sVec.push_back("<version>");      rVec.push_back("1.001");                            //server's property ???
                                    break;  // 002

    //003   = This server was created <datetime>
    case RPL_CREATED              : sVec.push_back("<datetime>");      rVec.push_back("<datetime>");                      //server's property ???
                                    break;  // 003


    //004   = <servername> <version> <available user modes> <available channel modes>
    case RPL_MYINFO               : sVec.push_back("<servername>");              rVec.push_back(users_[cs].serverHostmask());   //server's property ???
                                    sVec.push_back("<version>");                 rVec.push_back("1.001");                       //server's property ???
                                    sVec.push_back("<available user modes>");    rVec.push_back("io");                          //user's property ???
                                    sVec.push_back("<available channel modes>"); rVec.push_back("iklmnpt");                     //channel's property ???
                                    break;  // 004

    //005   = Try server <server name>, port <port number>
    case RPL_BOUNCE               : sVec.push_back("<server name>");             rVec.push_back(users_[cs].serverHostmask());   //server's property ???
                                    sVec.push_back("<port number>");             rVec.push_back("6667");                        //server's property ???
                                    break;  // 005


    //302   = :<reply list>
    case RPL_USERHOST             : break;  // 302 coding in *.cpp directly

    //303   = :<nick list>
    case RPL_ISON                 : break;  // 303 coding in *.cpp directly

    //301   = <nick> :<away message>
    case RPL_AWAY                 : sVec.push_back("<nick>");           rVec.push_back(paramsVec[0]);
                                    sVec.push_back("<away message>");   rVec.push_back(paramsVec[1]);
                                    break;  // 301

    //305   = :You are no longer marked as being away
    case RPL_UNAWAY               : break;  // 305

    //306   = :You have been marked as being away
    case RPL_NOWAWAY              : break;  // 306


    //311   = <nick> <user> <host> * :<real name>
    case RPL_WHOISUSER            : sVec.push_back("<nick> <user> <host> * :<real name>"); rVec.push_back(paramsVec[0]);
                                    break;  // 311 not in the scope or coding in *.cpp directly


    //312   = <nick> <server> :<server info>
    case RPL_WHOISSERVER          : sVec.push_back("<nick> <server> :<server info>"); rVec.push_back(paramsVec[0]);
                                    break;  // 312 not in the scope or coding in *.cpp directly


    //313   = <nick> :is an IRC operator
    case RPL_WHOISOPERATOR        : sVec.push_back("<nick>");    rVec.push_back(paramsVec[0]);
                                    break;  // 313 not in the scope or coding in *.cpp directly


    //317   = <nick> <integer> :seconds idle
    case RPL_WHOISIDLE            : sVec.push_back("<nick>");    rVec.push_back(paramsVec[0]);
                                    sVec.push_back("<integer>"); rVec.push_back(paramsVec[1]);
                                    break;  // 317

    //318   = <nick> :End of WHOIS list
    case RPL_ENDOFWHOIS           : sVec.push_back("<nick>");    rVec.push_back(paramsVec[0]);
                                    break;  // 318

    //319   = <nick> :*  <@/+> <channel>
    case RPL_WHOISCHANNELS        : sVec.push_back("<nick>");             rVec.push_back(paramsVec[0]);
                                    sVec.push_back("*  <@/+> <channel>"); rVec.push_back(paramsVec[1]);
                                    break;  // 319 not in the scope or coding in *.cpp directly

    //314   = <nick> <user> <host> * :<real name>
    case RPL_WHOWASUSER            : break;  // 314 not in the scope or coding in *.cpp directly

    //369   = <nick> :End of WHOWAS
    case RPL_ENDOFWHOWAS           : break;  // 369 not in the scope or coding in *.cpp directly

    //321   = Obsolete. Not used.
    // case RPL_LISTSTART        :  break;  // 321
    // 322   = <channel> <# visible> :<topic>
    case RPL_LIST                :  sVec.push_back("<channel>");      rVec.push_back(paramsVec[0]);
                                    sVec.push_back("<visible>");      rVec.push_back(paramsVec[1]);
                                    sVec.push_back("<topic>");        rVec.push_back(paramsVec[2]);
                                    break;  // 322
    //323   = :End of LIST
    case RPL_LISTEND             :  break;  // 323

    //325   = <channel> <nickname>
    case RPL_UNIQOPIS            :  sVec.push_back("<channel>");      rVec.push_back(paramsVec[0]);
                                    sVec.push_back("<nickname>");     rVec.push_back("get it from the channel");   //??
                                    break;  // 325

    //324   = <channel> <mode> <mode params>
    case RPL_CHANNELMODEIS       :  sVec.push_back("<channel>");      rVec.push_back(paramsVec[0]);
                                    sVec.push_back("<mode>");         rVec.push_back(paramsVec[1]);   //??
                                    sVec.push_back("<mode params>");  rVec.push_back(paramsVec[2]);   //??
                                    break;  // 324

    //331   = <channel> :No topic is set
    case RPL_NOTOPIC             :  break;  // 331
    //332   = <channel> :<topic>
    case RPL_TOPIC               :  sVec.push_back("<channel>");      rVec.push_back(paramsVec[0]);
                                    sVec.push_back("<topic>");        rVec.push_back(paramsVec[1]);   //??
                                    break;  // 332

    //341   = <channel> <nick>
    case RPL_INVITING            :  sVec.push_back("<channel>");      rVec.push_back(paramsVec[1]);
                                    sVec.push_back("<nick>");         rVec.push_back(paramsVec[0]);                 //??
                                    break;  // 341

    //342   = <user> :Summoning user to IRC
    //case RPL_SUMMONING           :  sVec.push_back("<user>");      rVec.push_back(paramsVec[0]);                    //??
    //                                break;  // 342


    //346   = <channel> <invitemask>
    case RPL_INVITELIST          :  sVec.push_back("<channel>");      rVec.push_back(paramsVec[0]);
                                    sVec.push_back("<invitemask>");   rVec.push_back("get it from the channel");   //??
                                    break;  // 346

    //347   = <channel> :End of channel invite list
    case RPL_ENDOFINVITELIST     :  sVec.push_back("<channel>");      rVec.push_back(paramsVec[0]);
                                    break;  // 347

    //348   = <channel> <exceptionmask>
    case RPL_EXCEPTLIST          :  sVec.push_back("<channel>");      rVec.push_back(paramsVec[0]);
                                    sVec.push_back("<exceptionmask>");rVec.push_back("get it from the channel");   //??
                                    break;  // 348

    //349   = <channel> :End of channel exception list
    case RPL_ENDOFEXCEPTLIST     :  sVec.push_back("<channel>");      rVec.push_back(paramsVec[0]);
                                    break;  // 349

    //351   = <version>.<debuglevel> <server> :<comments>
    case RPL_VERSION             :  sVec.push_back("<version>");      rVec.push_back("1");
                                    sVec.push_back("<debuglevel>");   rVec.push_back("001");
                                    sVec.push_back("<server>");       rVec.push_back("FT_IRC");
                                    sVec.push_back("<comments>");     rVec.push_back("42");
                                    break;  // 351


    //352   =     <channel> <user> <host> <server> <nick> ( "H" / "G" > ["*"] [ ( "@" / "+" ) ] :<hopcount> <real name>
    // case RPL_WHOREPL            :break  //352

    //315   = <name> :End of WHO list
    case RPL_ENDOFWHO            :  sVec.push_back("<name>");      rVec.push_back(paramsVec[0]);
                                    break;  // 315

    //353   = ( "=" / "*" / "@" ) <channel> :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )
    // case RPL_NAMREPLY            :break  //353

    //366   = <channel> :End of NAMES list
    case RPL_ENDOFNAMES          :  sVec.push_back("<channel>");      rVec.push_back(paramsVec[0]);
                                    break;  // 366


    //364   = <mask> <server> :<hopcount> <server info>
    // case RPL_LINKS            :  break  //364 not in the scope <-server to server

    //365   = <mask> :End of LINKS list
    // case RPL_ENDOFLINKS       :  break  //365 not in the scope <-server to server

    //367   = <channel> <banmask>
    // case RPL_BANLIST          :  break  //367 coding in *.cpp directly

    //368   = <channel> :End of channel ban list
    case RPL_ENDOFBANLIST        :  sVec.push_back("<string>");      rVec.push_back(paramsVec[0]);
                                    break;  // 368

    //371   = :<string>
    case RPL_INFO                :  sVec.push_back("<string>");      rVec.push_back(paramsVec[0]);
                                    break;  // 371
    //374   = :End of INFO list
    case RPL_ENDOFINFO           :  break;  // 374

    //375   = :- <server> Message of the day -
    case RPL_MOTDSTART           :  sVec.push_back("<server>");      rVec.push_back(users_[cs].serverHostmask());
                                    break;  // 375
    //372   = :- <text>
    case RPL_MOTD                :  sVec.push_back("<text>");        rVec.push_back("Life is 42. This is the only message for you.");
                                    break;  // 372
    //376   = :End of MOTD command
    case RPL_ENDOFMOTD           :  break;  // 376


    //381   = :You are now an IRC operator
    // case RPL_YOUREOPER        :  break;  // 381 not in the scope <- operator
    //382   = <config file> :Rehashing
    // case RPL_REHASHING        :  break;  // 382 not in the scope <- operator
    //383   = You are service <servicename>
    // case RPL_YOURESERVICE     :  break;  // 383 not in the scope <- service

    //391   = <server> :<string showing local time of the server>
    case RPL_TIME                :  sVec.push_back("<server>");      rVec.push_back(users_[cs].serverHostmask());
                                    sVec.push_back("<string showing local time of the server>");
                                    rVec.push_back("need server starting time info somewhere");                //??
                                    break;  // 375

    //392   = :UserID   Terminal  Host
    case RPL_USERSSTART          :  break;  // 392
    //393   = :<username> <ttyline> <hostname>
    // case RPL_USERS            :  break;  // 393 coding in **.cpp directly
    //394   = :End of users
    case RPL_ENDOFUSERS          :  break;  // 394
    //395   = :Nobody logged in
    case RPL_NOUSERS             :  break;  // 395


    //200   = Link <version & debug level> <destination> <next server> V<protocol version> <link uptime in seconds> <backstream sendq> <upstream sendq>
    // case RPL_TRACELINK        :  break;  // 200 not in the scope <- trace
    //201   = Try. <class> <server>
    // case RPL_TRACECONNECTING  :  break;  // 201 not in the scope <- trace
    //202   = H.S. <class> <server>
    // case RPL_TRACEHANDSHAKE   :  break;  // 202 not in the scope <- trace
    //203   = ???? <class> [<client IP address in dot form>]
    // case RPL_TRACEUNKNOWN     :  break;  // 203 not in the scope <- trace
    //204   = Oper <class> <nick>
    // case RPL_TRACEOPERATOR    :  break;  // 204 not in the scope <- trace
    //205   = User <class> <nick>
    // case RPL_TRACEUSER        :  break;  // 205 not in the scope <- trace
    //206   = Serv <class> <int>S <int>C <server> <nick!user|*!*>@<host|server> V<protocol version>
    // case RPL_TRACESERVER      :  break;  // 206 not in the scope <- trace
    //207   = Service <class> <name> <type> <active type>
    // case RPL_TRACESERVICE     :  break;  // 207 not in the scope <- trace
    //208   = <newtype> 0 <client name>
    // case RPL_TRACENEWTYPE     :  break;  // 208 not in the scope <- trace
    //209   = Class <class> <count>
    // case RPL_TRACECLASS       :  break;  // 209 not in the scope <- trace
    //210   = Unused.
    // case RPL_TRACERECONNECT   :  break;  // 210 not in the scope <- trace

    //261   = File <logfile> <debug level>
    // case RPL_TRACELOG        :  break;  // 261 not in the scope <- trace
    //262   = <server name> <version & debug level> :End of TRACE
    // case RPL_TRACEEND        :  break;  // 262 not in the scope <- trace


    //211   = <linkname> <sendq> <sent messages> <sent Kbytes> <received messages> <received Kbytes> <time open>
    // case RPL_STATSLINKINFO   :  break;  // 211 not in the scope <- stats
    //212   = <command> <count> <byte count> <remote count>
    // case RPL_STATSCOMMANDS   :  break;  // 212 not in the scope <- stats
    //219   = <stats letter> :End of STATS report
    // case RPL_ENDOFSTATS      :  break;  // 219 not in the scope <- stats

    //242   = :Server Up %d days %d:%02d:%02d
    // case RPL_STATSUPTIME     :  break;  // 242 not in the scope <- stats
    //243   = O <hostmask> * <name>
    // case RPL_STATSOLINE      :  break;  // 243 not in the scope <- stats

    //221   = <user mode string>
    case  RPL_UMODEIS            :  sVec.push_back("<user mode string>"); rVec.push_back(paramsVec[0]);
                                    break;  //221

    //234   = <name> <server> <mask> <type> <hopcount> <info>
    // case RPL_SERVLIST         :  break;  // 234 not in the scope <- servlist
    //235   = <mask> <type> :End of service listing
    // case RPL_SERVLISTEND      :  break;  // 235 not in the scope <- servlist


    //251   = :There are <integer> users and <integer> services on <integer> servers
    // case RPL_LUSERCLIENT      :  break;  // 251 not in the scope <- lusers

    //252   = <integer> :operator(s) online
    // case RPL_LUSEROP          :  break;  // 252 not in the scope <- lusers

    //253   = <integer> :unknown connection(s)
    // case RPL_LUSERUNKNOWN     :  break;  // 253 not in the scope <- lusers

    //254   = <integer> :channels formed
    // case RPL_LUSERCHANNELS    :  break;  // 254 not in the scope <- lusers

    //255   = :I have <integer> clients and <integer> servers
    // case RPL_LUSERME          :  break;  // 255 not in the scope <- lusers


    //256   = <server> :Administrative info
    // case RPL_ADMINME          :  break;  // 256 not in the scope <- admin
    //257   = :<admin info>
    // case RPL_ADMINLOC1        :  break;  // 257 not in the scope <- admin
    //258   = :<admin info>
    // case RPL_ADMINLOC2        :  break;  // 258 not in the scope <- admin
    //259   = :<admin info>
    // case RPL_ADMINEMAIL       :  break;  // 259 not in the scope <- admin


    //263   = <command> :Please wait a while and try again.
    case  RPL_TRYAGAIN           :  sVec.push_back("<command>");      rVec.push_back(command);
                                    break;  // 263


    //////////////////////////////



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
    //PRIVMSG   <target> :Duplicate recipients. No message delivered"
    //JOIN      ???
    case  ERR_TOOMANYTARGETS     :  sVec.push_back("<target>");        rVec.push_back(paramsVec[0]);
                                    sVec.push_back("<error code> recipients. <abort message>");
                                    rVec.push_back(paramsVec[1]);
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
  msg = std::string(":" + users_[cs].serverHostmask() + " " + SS::toString(code, 3) + " " + users_[cs].nick + " ").append(msg).append(NEWLINE);

  if (bDirectDelivery)
    users_[cs].appendWbuff(std::string(msg));

  return msg;
}


