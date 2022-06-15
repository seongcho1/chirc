#ifndef __REPLY_H__
#define __REPLY_H__

#define TEST                                 000

#define RPL_WELCOME                          001
#define RPL_YOURHOST                         002
#define RPL_CREATED                          003
#define RPL_MYINFO                           004

#define RPL_BOUNCE                           005

#define RPL_USERHOST                         302 //coding in *.cpp directly
#define RPL_ISON                             303 //coding in *.cpp directly

#define RPL_AWAY                             301
#define RPL_UNAWAY                           305
#define RPL_NOWAWAY                          306

#define RPL_WHOISUSER                        311 //coding in *.cpp directly
#define RPL_WHOISSERVER                      312 //coding in *.cpp directly
#define RPL_WHOISOPERATOR                    313
#define RPL_WHOISIDLE                        317
#define RPL_ENDOFWHOIS                       318
#define RPL_WHOISCHANNELS                    319

#define RPL_WHOWASUSER                       314

#define RPL_ENDOFWHOWAS                      369


// #define RPL_LISTSTART                     321  //Obsolete. Not used.
// #define RPL_LIST                          322  //coding in LIST.cpp directly
#define RPL_LISTEND                          323
#define RPL_UNIQOPIS                         325
#define RPL_CHANNELMODEIS                    324

#define RPL_NOTOPIC                          331
#define RPL_TOPIC                            332

#define RPL_INVITING                         341
// #define RPL_SUMMONING                     342
#define RPL_INVITELIST                       346
#define RPL_ENDOFINVITELIST                  347
#define RPL_EXCEPTLIST                       348
#define RPL_ENDOFEXCEPTLIST                  349

#define RPL_VERSION                          351
#define RPL_WHOREPLY                         352
#define RPL_ENDOFWHO                         315

#define RPL_NAMREPLY                         353
#define RPL_ENDOFNAMES                       366

// #define RPL_LINKS                         364 //not in the scope <-server to server
// #define RPL_ENDOFLINKS                    365 //not in the scope <-server to server

#define RPL_BANLIST                          367 //coding in *.cpp directly
#define RPL_ENDOFBANLIST                     368

#define RPL_INFO                             371
#define RPL_ENDOFINFO                        374

#define RPL_MOTDSTART                        375
#define RPL_MOTD                             372
#define RPL_ENDOFMOTD                        376

// #define RPL_YOUREOPER                     381 //not in the scope <-server to server
// #define RPL_REHASHING                     382 //not in the scope <-server to server
// #define RPL_YOURESERVICE                  383 //not in the scope <-server to server

#define RPL_TIME                             391

#define RPL_USERSSTART                       392
#define RPL_USERS                            393
#define RPL_ENDOFUSERS                       394
#define RPL_NOUSERS                          395

// #define RPL_TRACELINK                     200 //not in the scope <-trace
// #define RPL_TRACECONNECTING               201 //not in the scope <-trace
// #define RPL_TRACEHANDSHAKE                202 //not in the scope <-trace
// #define RPL_TRACEUNKNOWN                  203 //not in the scope <-trace
// #define RPL_TRACEOPERATOR                 204 //not in the scope <-trace
// #define RPL_TRACEUSER                     205 //not in the scope <-trace
// #define RPL_TRACESERVER                   206 //not in the scope <-trace
// #define RPL_TRACESERVICE                  207 //not in the scope <-trace
// #define RPL_TRACENEWTYPE                  208 //not in the scope <-trace
// #define RPL_TRACECLASS                    209 //not in the scope <-trace
// #define RPL_TRACERECONNECT                210 //not in the scope <-trace
// #define RPL_TRACELOG                      261 //not in the scope <-trace
// #define RPL_TRACEEND                      262 //not in the scope <-trace

// #define RPL_STATSLINKINFO                 211 //not in the scope <-stats
// #define RPL_STATSCOMMANDS                 212 //not in the scope <-stats
// #define RPL_ENDOFSTATS                    219 //not in the scope <-stats
// #define RPL_STATSUPTIME                   242 //not in the scope <-stats
// #define RPL_STATSOLINE                    243 //not in the scope <-stats

#define RPL_UMODEIS                          221

// #define RPL_SERVLIST                      234 //not in the scope <-servlist
// #define RPL_SERVLISTEND                   235 //not in the scope <-servlist


// #define RPL_LUSERCLIENT                   251 //not in the scope <-server to server
// #define RPL_LUSEROP                       252 //not in the scope <-server to server
// #define RPL_LUSERUNKNOWN                  253 //not in the scope <-server to server
// #define RPL_LUSERCHANNELS                 254 //not in the scope <-server to server
// #define RPL_LUSERME                       255 //not in the scope <-server to server

// #define RPL_ADMINME                       256 //not in the scope <-admin
// #define RPL_ADMINLOC1                     257 //not in the scope <-admin
// #define RPL_ADMINLOC2                     258 //not in the scope <-admin
// #define RPL_ADMINEMAIL                    259 //not in the scope <-admin

#define RPL_TRYAGAIN                         263


/////////////////////////////////


#define ERR_NOSUCHNICK                       401
#define ERR_NOSUCHSERVER                     402
#define ERR_NOSUCHCHANNEL                    403
#define ERR_CANNOTSENDTOCHAN                 404
#define ERR_TOOMANYCHANNELS                  405
#define ERR_WASNOSUCHNICK                    406
#define ERR_TOOMANYTARGETS                   407
#define ERR_NOSUCHSERVICE                    408
#define ERR_NOORIGIN                         409

#define ERR_NORECIPIENT                      411
#define ERR_NOTEXTTOSEND                     412
#define ERR_NOTOPLEVEL                       413
#define ERR_WILDTOPLEVEL                     414
#define ERR_BADMASK                          415

#define ERR_UNKNOWNCOMMAND                   421
#define ERR_NOMOTD                           422
#define ERR_NOADMININFO                      423
#define ERR_FILEERROR                        424

#define ERR_NONICKNAMEGIVEN                  431
#define ERR_ERRONEUSNICKNAME                 432
#define ERR_NICKNAMEINUSE                    433
// #define ERR_NICKCOLLISION                 436 not in the scope <-server to server
#define ERR_UNAVAILRESOURCE                  437

#define ERR_USERNOTINCHANNEL                 441
#define ERR_NOTONCHANNEL                     442
#define ERR_USERONCHANNEL                    443
#define ERR_NOLOGIN                          444
#define ERR_SUMMONDISABLED                   445
#define ERR_USERSDISABLED                    446

#define ERR_NOTREGISTERED                    451

#define ERR_NEEDMOREPARAMS                   461
#define ERR_ALREADYREGISTRED                 462
#define ERR_NOPERMFORHOST                    463
#define ERR_PASSWDMISMATCH                   464
#define ERR_YOUREBANNEDCREEP                 465
#define ERR_YOUWILLBEBANNED                  466
#define ERR_KEYSET                           467

#define ERR_CHANNELISFULL                    471
#define ERR_UNKNOWNMODE                      472
#define ERR_INVITEONLYCHAN                   473
#define ERR_BANNEDFROMCHAN                   474
#define ERR_BADCHANNELKEY                    475
#define ERR_BADCHANMASK                      476
#define ERR_NOCHANMODES                      477
#define ERR_BANLISTFULL                      478

#define ERR_NOPRIVILEGES                     481
#define ERR_CHANOPRIVSNEEDED                 482
#define ERR_CANTKILLSERVER                   483
#define ERR_RESTRICTED                       484
#define ERR_UNIQOPPRIVSNEEDED                485

#define ERR_NOOPERHOST                       491

#define ERR_UMODEUNKNOWNFLAG                 501
#define ERR_USERSDONTMATCH                   502

#endif
