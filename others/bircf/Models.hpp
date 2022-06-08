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
  int c_creator;
  std::set<int> c_operator;
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
  bool initialized;
  bool wait_pong;
  time_t dead;
  time_t alive;
  std::set<std::string> engaged;

  User() {}
  User(int const &fd) : fd(fd)      {}
  bool IsAlive(void)                { return time(NULL) < alive; }
  bool IsDead(void)                 { return dead < time(NULL); }
  void KeepAlive(void)              { alive = time(NULL) + TIMEOUT; dead = alive + WAIT_TIME; }

  bool clientRead(std::string &buffer)    {
    // int r = recv(fd, read, BUF_SIZE, 0);
    // char read[BUF_SIZE + 1];

    // read[BUF_SIZE] = 0;
    // if (0 < r && r < BUF_SIZE)
    //   buffer.append(read);
    (void)buffer;
    return false;
  }

  void clientWrite(std::string &message)  {
    if (message.empty())
      return;

    unsigned long endset, offset = 0;
    while (offset < message.length()) {
      if (message.length() > BUF_SIZE + offset)
        endset = BUF_SIZE;
      else
        endset = message.length();
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
