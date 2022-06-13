#ifndef __MODELS_H__
#define __MODELS_H__

#include <sys/socket.h>
#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include <set>

#define BUF_SIZE	512
#define N 10
#define NICK_MAX_LENGTH 9
#define CHANNEL_MAX_LENGTH 200 // Channel names are case insensitive. (200 or 50?)
#define CHANNEL_PREFIX "#&+!" // [&] is local channel, [+] not support channel modes, [!] identified as the "channel creator",
#define CHANNEL_NOT_ALLOW "^G ," // ^G is ascii(7), blank, comma
#define MESSAGE_PREFIX ":" // not allow blank
// #define TIMEOUT 300
#define TIMEOUT 300
// #define WAIT_TIME 30
#define WAIT_TIME 30
#define PING_REQUEST "PING :"
#define PONG_RESULT "FT_IRC"
#define MODE_FLAGS_MAP_SIZE     26
#define MODE_FALGS_MAP          "abcdefghijklmnopqrstuvwxyz"
#define USER_MODE_FLAGS         "00000000i00000000000000000"
#define CHANNEL_MODE_FLAGS      "ab000000i0klmn0p00st000000"
#define CHANNEL_USER_MODE_FLAGS "00000000000000o000000v0000"

#define AUTH_LEVEL1 0x1 // pass
#define AUTH_LEVEL2 0x2 // nick
#define AUTH_LEVEL3 0x4 // user
#define AUTH_MASK   0x7 // all


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
  unsigned int mode;
  std::string title;
  std::string topic;
  // int channelCreator;
  std::set<int> channelOperators;
  std::set<int> channelSpeaker;
  std::set<int> member;

  Channel() {}
  Channel(std::string &title) : title(title)  {}

  bool leave(int fd) {
    channelOperators.erase(fd);
    channelSpeaker.erase(fd);
    return member.erase(fd); 
  }
  
  std::string currentMode(void)               {
    std::string result;
    for (int i = 0; i < MODE_FLAGS_MAP_SIZE; ++i) {
      if (((mode >> i) & 1))
        result += CHANNEL_MODE_FLAGS[i];
    }
    if (result.size() > 0)
      result = "+" + result;
    return result;
  }

  // void userMode(std::string mode, int member) {

  // }

  bool isMode(char flag) {
    return (mode >> (flag - 'a') & 1);
  }

  void setMode(std::string mode) {
    std::string::iterator it = mode.begin();
    bool add = false;
    while (it != mode.end()) {
      if (*it == '+') {
        add = true;
      }
      else if (*it == '-') {
        add = false;
      }
      else if ('a' <= *it && *it <= 'z' && USER_MODE_FLAGS[*it - 'a'] != '0') {
        if (add)
          this->mode |= 1 << (*it - 'a');
        else
          this->mode &= ~(1 << (*it - 'a'));
      }
      ++it;
    }
  }
};

class User {
public:
  int fd;
  std::string nick;
  std::string user;
  std::string real;
  std::string host;
  unsigned int mode;
  char authenticated;
  bool waitPong;
  bool quit;
  time_t dead;
  time_t alive;
  std::set<std::string> engaged;
  std::set<std::string> invited;

  User() {}
  User(int const &fd, std::string const &host, char auth) :
    fd(fd),
    host(host),
    mode(0),
    authenticated(auth),
    quit(false),
    dead(time(NULL) + WAIT_TIME),
    alive(time(NULL) + TIMEOUT) {}
  bool isAlive(void)                { return time(NULL) < alive; }
  bool isDead(void)                 { return dead < time(NULL); }
  void toQuit(void)                 { quit = true; }
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

  bool isAuthenticated(void) { return authenticated == AUTH_MASK; }
  
  std::string prefix(void) {
    if (!isAuthenticated())
      return std::string("");
    else if (user.length() && host.length())
	    return std::string(nick + "!" + user + "@" + host);
    return nick;
  }

  std::string currentMode(void) {
    std::string result;
    for (int i = 0; i < MODE_FLAGS_MAP_SIZE; ++i) {
      if (((mode >> i) & 1))
        result += USER_MODE_FLAGS[i];
    }
    if (result.size() > 0)
      result = "+" + result;
    return result;
  }

  bool isMode(char flag) {
    return (mode >> (flag - 'a') & 1);
  }

  void setMode(std::string mode) {
    std::string::iterator it = mode.begin();
    bool add = false;
    while (it != mode.end()) {
      if (*it == '+') {
        add = true;
      }
      else if (*it == '-') {
        add = false;
      }
      else if ('a' <= *it && *it <= 'z' && USER_MODE_FLAGS[*it - 'a'] != '0') {
        if (add)
          this->mode |= 1 << (*it - 'a');
        else
          this->mode &= ~(1 << (*it - 'a'));
      }
      ++it;
    }
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
