#include "StringUtils.hpp"
#include <map>
#include<unistd.h>

//#define NEWLINE		"\n"
#define BUF_SIZE	10

class MessageManager {
private:
  typedef void(MessageManager::*FuncPtr)(std::string command);
  std::map<std::string, FuncPtr> functionCallMap;
  void PRIVMSG(std::string command) { std::cout << "[PRIVMSG]" << command << std::endl; }
  void SELFMSG(std::string command) { std::cout << "[SELFMSG]" << command << std::endl; }
  void PUBLICMSG(std::string command) { std::cout << "[PUBLICMSG]" << command << std::endl; }

public:
  MessageManager() {
    functionCallMap["PRIVMSG"] = &MessageManager::PRIVMSG;
    functionCallMap["SELFMSG"] = &MessageManager::SELFMSG;
    functionCallMap["PUBLICMSG"] = &MessageManager::PUBLICMSG;
  }

  void executeCommand (std::string command) {
    (this->*functionCallMap[command])(command);
  }
};

int clientWriteTest() {

  std::string out_commands = "aaa\n";;
  std::string token;
  char	buf_write[BUF_SIZE + 1];

  int buf_size;
  while (out_commands.length()) {
    if (out_commands.length() > BUF_SIZE)
      buf_size = BUF_SIZE;
    else
      buf_size = out_commands.length();
    //token = out_commands.substr(0, BUF_SIZE);
    //std::cout << token;
    sprintf(buf_write, "%s", out_commands.substr(0, buf_size).c_str());
    buf_write[buf_size] = 0;
    std::cout << buf_write;
    //send(cs, token.c_str(), token.length(), 0);
    out_commands.erase(0, buf_size);
  }
  return 0;
}

int splitSstringTest() {

  int i;
  std::string a1 = "NICK hello\r\nUSER 1 0 * 2\r\n";
  std::string a2 = "";


  std::vector<std::string> avec1 = SS::splitString(a1, NEWLINE, true, true);

  std::cout << "vec size=" << avec1.size() << std::endl;
  i = 0;
  while (i < (int) avec1.size()) {
    std::cout << "(" << avec1[i] << ")->";
    i++;
  }
  std::cout << std::endl << "a1=(" << a1 << ")" << std::endl;

/*
  //std::vector<std::string> avec2 = SS::splitString(a2, SPACE_COLON, false, false, true);

  std::vector<std::string> avec2 = SS::splitString(a2, SPACE);

  std::cout << "vec size=" << avec2.size() << " " << avec2.empty() << std::endl;
  i = 0;
  while (i < (int) avec2.size()) {
    std::cout << "(" << avec2[i] << ")->";
    i++;
  }
  std::cout << std::endl << "a2=(" << a2 << ")" << std::endl;
*/
  return 0;
}


int toUpperTest() {
  std::string a = "a{|}^z";
  a = SS::toUpper(a);
  std::cout << a << std::endl;

  return 0;
}

int	matchStringTest() {
  std::string str = "baaabab";
  //char str[] = "baaabab";
  //char pattern[] = "*****ba*****ab";
  // char pattern[] = "ba*****ab";
  std::string pattern = "*ba*?*ab*";

  // char pattern[] = "*ba*?*ab*";
  // char pattern[] = "a*ab";
  // char pattern[] = "a*****ab";
  // char pattern[] = "*a*****ab";
  // char pattern[] = "ba*ab****";
  // char pattern[] = "****";
  // char pattern[] = "*";
  // char pattern[] = "aa?ab";
  // char pattern[] = "b*b";
  // char pattern[] = "a*a";
  // char pattern[] = "baaabab";
  // char pattern[] = "?baaabab";
  // char pattern[] = "*baaaba*";

  //if (strmatch(str, pattern, strlen(str), strlen(pattern)))
  if (SS::matchString(str, pattern))
      std::cout << "Yes" << std::endl;
  else
      std::cout << "No" << std::endl;

  return 0;
}

int trimStringTest() {

  //std::string source = ":hello privmsg bbb :hello";
  std::string source = "privmsg bbb :hello";

  SS::ltrim(source, std::string(":hello ").c_str());

  std::cout << source << std::endl;



  return 0;
}



bool stop = false;


//signal handling function that will except ctrl-\ and ctrl-c
void sig_handler(int signo) {
    //looks for ctrl-c which has a value of 2
    if (signo == SIGINT) {
        printf("\nreceived SIGINT\n");
        //stop = true;
    }
    //looks for ctrl-\ which has a value of 9
    else if (signo == SIGQUIT) {
        printf("\nreceived SIGQUIT\n");
        //stop = true;
    }
}


int main() {

  //MessageManager c;
  //c.executeCommand("PRIVMSG");
  //clientWriteTest();
  splitSstringTest();
  //toUpperTest();
  //matchStringTest();
  //trimStringTest();

  signal(SIGINT, sig_handler);
  signal(SIGQUIT, sig_handler);

    // //these if statement catch errors
    // if (signal(SIGINT, sig_handler) == SIG_ERR)
    //     printf("\ncan't catch SIGINT\n");
    // if (signal(SIGQUIT, sig_handler) == SIG_ERR)
    //     printf("\ncan't catch SIGQUIT\n");

    //Runs the program infinitely so we can continue to input signals
    while(!stop) {
      std::cout << "running!!!" << std::endl;
      sleep(1);
    }

    printf("exiting safely\n");
    return 0;



  return 0;
}
