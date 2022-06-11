#ifndef __REPLY_H__
#define __REPLY_H__

#define RPL_WELCOME                          001
#define RPL_YOURHOST                         002
#define RPL_CREATED                          003
#define RPL_MYINFO                           004

#define RPL_BOUNCE                           005

#define RPL_USERHOST                         302
#define RPL_ISON                             303

#define RPL_AWAY                             301
#define RPL_UNAWAY                           305
#define RPL_NOWAWAY                          306

#define RPL_WHOISUSER                        311
#define RPL_WHOISSERVER                      312
#define RPL_WHOISOPERATOR                    313
#define RPL_WHOISIDLE                        317
#define RPL_ENDOFWHOIS                       318
#define RPL_WHOISCHANNELS                    319

#define RPL_WHOWASUSER                       314
#define RPL_ENDOFWHOWAS                      369


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
