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
#define WAIT_TIME 3000
#define CHANNEL_MEMBER_LIMIT 32
#define USER_ENGAGED_LIMIT 32
#define PING_REQUEST "PING :"
#define PONG_RESULT "FT_IRC"

#define MODE_FLAGS_MAP_SIZE 26
#define MODE_LENGTH   3
#define CHN_M_A_FLAGS "00000000i0klmn0p000t000000"
#define CHN_M_FLAGS   "imnpt"
#define CHN_M_P_FLAGS "kl"
#define CHN_M_U_FLAGS "ov"
#define USR_M_A_FLAGS "00000000i00000000000000000"
#define USR_M_FLAGS   "i"

#define AUTH_LEVEL1 0x1 // pass
#define AUTH_LEVEL2 0x2 // nick
#define AUTH_LEVEL3 0x4 // user
#define AUTH_MASK   0x7 // all


#define Xv(err,res,str)		(SS::x_void(err,res,str,(char *)__FILE__,__LINE__))
#define X(err,res,str)		(SS::x_int(err,res,str,(char *)__FILE__,__LINE__))
#define MAX(a,b)					((a > b) ? a : b)
#define MIN(a,b)					((a > b) ? b : a)


// // following CHANNEL_PREFIX sequence (MUST)
// enum CreateOption {
//   NORMAL,   // #
//   LOCAL,    // &
//   NO_MODES,  // +
//   AS_CREATOR,  // !
// };

/*
The user creating a channel automatically becomes channel operator
   with the notable exception of channels which name is prefixed by the
   character '+'
*/
class BaseModel {
public:
  unsigned int mode;
  int limit;

  std::string currentMode(std::string filter) {

std::cout << "filter = " << filter << std::endl;
    std::string result;
    for (int i = 0; i < MODE_FLAGS_MAP_SIZE; ++i) {
      if (((mode >> i) & 1))
        result += filter[i];
    }
    if (result.size() > 0)
      result = "+" + result;
    return result;
  }

  bool isMode(char flag) {
    return (mode >> (flag - 'a') & 1);
  }

  bool setMode(bool add, char mode) {
    unsigned int bitmap = this->mode;
    if (add)
      bitmap |= 1 << (mode - 'a');
    else
      bitmap &= ~(1 << (mode - 'a'));

    return isChanged(this->mode, bitmap);
  }

  bool isChanged(unsigned int &to, unsigned int const &from) {
    if (to == from)
      return false;
    to = from;
    return true;
  }
};



class Channel : public BaseModel {
public:
  std::string title;
  std::string topic;
  std::string key;
  // int channelCreator;
  std::set<int> channelOperators;
  std::set<int> channelSpeaker;
  std::set<int> member;

  Channel() {}
  Channel(int cs, std::string &title) :
    title(title),
    topic(),
    key() {
      BaseModel::mode = 0;
      BaseModel::limit = CHANNEL_MEMBER_LIMIT;
      channelOperators.insert(cs);
      member.insert(cs);
    }

  bool leave(int fd) {
    channelOperators.erase(fd);
    channelSpeaker.erase(fd);
    return member.erase(fd);
  }

  bool isOper(int fd) {
    return channelOperators.find(fd) != channelOperators.end();
  }

  bool unableFlag(int fd, char flag) {
    return !isOper(fd) && isMode(flag);
  }
};



class User : public BaseModel {
public:
  int fd;
  std::string nick;
  std::string user;
  std::string real;
  std::string host;
  std::string away;
  std::string pnik;
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
    nick(),
    host(host),
    away(),
    pnik(),
    authenticated(auth),
    quit(false),
    dead(time(NULL) + WAIT_TIME),
    alive(time(NULL) + TIMEOUT) {
      BaseModel::mode = 0;
      BaseModel::limit = USER_ENGAGED_LIMIT;
    }
  bool isAlive(void)                { return time(NULL) < alive; }
  bool isDead(void)                 { return dead < time(NULL); }
  void toQuit(void)                 { quit = true; }
  void keepAlive(void)              { alive = time(NULL) + TIMEOUT; dead = alive + WAIT_TIME; }

  bool clientRead(std::string &buffer) {
    char read[BUF_SIZE + 1];
    int r = recv(fd, read, BUF_SIZE + 1, 0);

    if (r <= 0 ||
        BUF_SIZE < buffer.length() + r ||
        (r == BUF_SIZE && read[BUF_SIZE - 1] != '\n'))
      return false;

    read[r] = 0;
    buffer.append(read);

std::cout << ">> " << buffer;
SS::charPrint(buffer);

    return true;
  }

  void clientWrite(std::string &message) {
    if (message.empty())
      return;


std::vector<std::string> msg = SS::splitString(message, NEWLINE);
for (int i = 0; i < (int)msg.size(); ++i) {
  msg[i].append(NEWLINE);
  send(fd, msg[i].c_str(), msg[i].length(), 0);
std::cout << "<< " << msg[i];
SS::charPrint(msg[i]);
}
message.clear();

    // unsigned long endset, offset = 0;

    // while (offset < message.length()) {
    //   endset = MIN(BUF_SIZE, message.length() - offset);
    //   send(fd, message.c_str() + offset, endset, 0);
    //   offset += endset;
    // }
    // message.clear();
  }

  bool isAuthenticated(void) { return authenticated == AUTH_MASK; }

  //https://datatracker.ietf.org/doc/html/rfc2813#section-5.9.1
  std::string prefix(void) {
    return std::string(nick + "!~" + user + "@" + host);
    // return std::string(nick + "!" + user + "@0");
  }

  std::string legacyPrefix(void) {
    return std::string(pnik + "!~" + user + "@" + host);
    // return std::string(pnik + "!" + user + "@0");
  }

  std::string systemPrefix(void) {
    return std::string().append("FT_IRC!FT_IRC@42.net");
    // return std::string().append("*.").append(host);
    //return host;
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
