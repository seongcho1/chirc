#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <cerrno>
#include <iostream>
#include <sstream>
// #include <cerrno>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

#define NEWLINE       "\n"
#define SPACE         " "
#define SPACE_COLON   " :"


class SS {
public:

  // whildcard *, ?
  static bool matchString(const char* source, const char* pattern) {
    // If we reach at the end of both strings, we are done
    if (*pattern == '\0' && *source == '\0')
        return true;

    // Make sure to eliminate consecutive '*'
    if (*pattern == '*') {
        while (*(pattern + 1) == '*')
            pattern++;
    }

    // Make sure that the characters after '*' are present
    // in source string. This function assumes that the
    // pattern string will not contain two consecutive '*'
    if (*pattern == '*' && *(pattern + 1) != '\0' && *source == '\0')
        return false;

    // If the pattern string contains '?', or current
    // characters of both strings match
    if (*pattern == '?' || *pattern == *source)
        return matchString(source + 1, pattern + 1);

    // If there is *, then there are two possibilities
    // a) We consider current character of source string
    // b) We ignore current character of source string.
    if (*pattern == '*')
        return matchString(source, pattern + 1)
              || matchString(source + 1, pattern);
    return false;
  }

  static bool matchString(std::string source, std::string pattern) {
    return matchString(source.c_str(), pattern.c_str());
  }

  static void matchStringVector(std::vector<std::string> &sourceVec, std::string pattern) {
    std::vector<std::string>::iterator it, temp;

    it = sourceVec.begin();
    while (it != sourceVec.end()) {
      if (it->empty())
        continue;
      temp = it;
      ++it;

      if (!matchString(*temp, pattern))
        sourceVec.erase(temp);
    }
  }


  //trim from left
  static std::string& ltrim(std::string& s, const char* t = " \t") { //\n\r\f\v")
    s.erase(0, s.find_first_not_of(t));
    return s;
  }

  //trim from right
  static std::string& rtrim(std::string& s, const char* t = " \t") { //\n\r\f\v")
    s.erase(s.find_last_not_of(t) + 1);
    return s;
  }

  //trim from left & right
  static std::string& trim(std::string& s, const char* t = " \t") { //\n\r\f\v")
    return ltrim(rtrim(s, t), t);
  }

  static std::string replaceString1(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
      subject.replace(pos, search.length(), replace);
      pos += replace.length();
    }
    return subject;
  }

  static void replaceString2(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
      subject.replace(pos, search.length(), replace);
      pos += replace.length();
    }
  }

  static void replaceString(std::string& subject, std::vector<std::string> const &searchVec, std::vector<std::string> const &replaceVec) {
    if (searchVec.size() != replaceVec.size())
      return;
    size_t  i = 0;
    while (i < searchVec.size()) {
      replaceString2(subject, searchVec[i], replaceVec[i]);
      i++;
    }
  }


  template <typename T>
  static std::string  toString ( T number ) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
  }

  static std::string  toUpper(std::string &s)
  {
    for (std::size_t i = 0; i < s.size(); i++) {

      s[i] = (std::toupper(s[i]));

      // 123:{ => 91:[
      // 124:| => 92:\
      // 125:} => 93:]
      // 94:^ => 126:~
      switch(s[i]) {

        case '{' : s[i] = '[';
              break;
        case '|' : s[i] = '\\';
              break;
        case '}' : s[i] = ']';
              break;
        case '^' : s[i] = '~';
              break;
        default:
              break;
      }
    }

    //std::transform(s.begin(), s.end(), s.begin(), ::toupper);

    //int (*tu)(int) = toupper; // Select that particular overload
    //std::transform(s.begin(),s.end(),s.begin(),tu );

    //Since C++11, we could use a lambda:
    //std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
    return s;
  }

  static std::vector<std::string>  splitString(std::string &s, std::string delimiter,
                                              bool bSkipLast = false,
                                              bool bClearProcessedString = false,
                                              bool bCutOnce = false) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> result;
    int i = 0;
    if (s.empty())
      return result;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
      token = s.substr(pos_start, pos_end - pos_start); //+ delimiter.length());
      //std::cout << "end=" << pos_end << "[" << token << "]->" << std::endl;
      if ( (bCutOnce && i == 0 && token.empty()) ||
        (!token.empty()) ) {
        result.push_back(token);  //in case of vector
        i++;
      }

      if (bClearProcessedString == true) {
        s.erase(pos_start, pos_end + delim_len);
        pos_end = pos_end - (token.length() + delim_len);
      }

      pos_start = pos_end + delim_len;
      if (bCutOnce)
        break;
    }

    //in case of "aaa\nbbb", deals with "bbb"
    if (bSkipLast == false && pos_start < s.length()) {
      token = s.substr(pos_start);
      if (!token.empty()) {
        result.push_back(token);
        if (bClearProcessedString == true)
          s.clear();
      }
    }
    //std::cout << std::endl;
    return result;
  }

  static int    x_int(int err, int res, char *str, char *file, int line) {
    if (res == err) {
      fprintf(stderr, "%s error (%s, %d): %s\n",
      // str, file, line, strerror(errno));
      str, file, line, "[windows] can not use strerror :(");
      exit (1);
    }
    return (res);
  }

  static  void  *x_void(void *err, void *res, char *str, char *file, int line)
  {
    if (res == err) {
      fprintf(stderr, "%s error (%s, %d): %s\n",
      // str, file, line, strerror(errno));
      str, file, line, "[windows] can not use strerror :(");
      exit (1);
    }
    return (res);
  }

};


#endif
