#ifndef __MODELS_H__
#define __MODELS_H__

#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <set>

//#define BUFF_SIZE 4096
// #define R 13
#define BUF_SIZE	512
#define N 10
#define NICK_MAX_LENGTH 9
#define CHANNEL_MAX_LENGTH 50 // Channel names are case insensitive.
#define CHANNEL_PREFIX "#&+!" // [&] is local channel, [+] not support channel modes, [!] identified as the "channel creator",
#define CHANNEL_NOT_ALLOW "^G ," // ^G is ascii(7), blank, comma
#define MESSAGE_PREFIX ":" // not allow blank
#define TIMEOUT 300
#define WAIT_TIME 30
#define PING "PING "
#define PONG ":FT_IRC"

#define Xv(err,res,str)		(SS::x_void(err,res,str,(char *)__FILE__,__LINE__))
#define X(err,res,str)		(SS::x_int(err,res,str,(char *)__FILE__,__LINE__))
#define MAX(a,b)					((a > b) ? a : b)
#define MIN(a,b)					((a > b) ? b : a)


// following CHANNEL_PREFIX sequence (MUST)
enum CreateOption {
  NORMAL,   // #
  LOCAL,    // &
  NO_MODES,  // +
  AS_CREATOR,  // !
};

/*
The user creating a channel automatically becomes channel operator
   with the notable exception of channels which name is prefixed by the
   character '+'
*/
class Channel {
public:
  int mode;
  std::string title;
  std::string topic;
  int channelCreator;
  std::set<int> channelOperators;
  std::set<int> member;

  Channel() {}
  Channel(std::string &title) : title(title)  {}
  bool Leave(int fd)                          { return member.erase(fd); }
};

class User {
public:
  int fd;
  std::string nick;
  std::string user;
  int mode;
  std::string real;
  std::string host;
  std::string server;
  bool authenticated;
  bool waitPong;
  time_t dead;
  time_t alive;
  std::set<std::string> engaged;

  User() {}
  User(int const &fd, std::string const &host) : fd(fd), host(host) {}
  bool isAlive(void)                { return time(NULL) < alive; }
  bool isDead(void)                 { return dead < time(NULL); }
  void keepAlive(void)              { alive = time(NULL) + TIMEOUT; dead = alive + WAIT_TIME; }

  bool clientRead(std::string &buffer)    {
    char read[BUF_SIZE + 1];
    int r = recv(fd, read, BUF_SIZE + 1, 0);

    if (r <= 0 ||
        BUF_SIZE < buffer.length() + r ||
        (r == BUF_SIZE && read[BUF_SIZE - 1] != '\n'))
      return false;
    
    read[r] = 0;
    buffer.append(read);
    return true;
  }

  void clientWrite(std::string &message)  {
    if (message.empty())
      return;

    unsigned long endset, offset = 0;
    while (offset < message.length()) {
      endset = MIN(BUF_SIZE, message.length() - offset);
      send(fd, message.c_str() + offset, endset, 0);
      offset += endset;
    }
    message.clear();
  }
};

#endif


// toUpper extra coverage
// 123:{ => 91:[
// 124:| => 92:\
// 125:} => 93:]
// 94:^ => 126:~


// channel command
// - INVITE
// - KICK
// - MODE
// - PRIVMSG
// - TOPIC

// channel mode
// O - give "channel creator" status;
// o - give/take channel operator privilege;
// a - toggle the anonymous channel flag; [the origin is changed to "anonymous!anonymous@anonymous."]
// i - toggle the invite-only channel flag;
// m - toggle the moderated channel;
// n - toggle the no messages to channel from clients on the
//     outside;
// q - toggle the quiet channel flag;
// p - toggle the private channel flag;
// s - toggle the secret channel flag;
// r - toggle the server reop channel flag;
// t - toggle the topic settable by channel operator only flag;
// k - set/remove the channel key (password);
// l - set/remove the user limit to channel;
// b - set/remove ban mask to keep users out;
// e - set/remove an exception mask to override a ban mask;
// I - set/remove an invitation mask to automatically override
//     the invite-only flag;
