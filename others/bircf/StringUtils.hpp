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
#define COMMA         ","
#define NUL           "\0"
#define CR            "\r"
#define CR_NUL        "\r\0"

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

  static std::vector<std::string>& matchStringVector(std::vector<std::string>& sourceVec, std::string pattern) {
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
    return sourceVec;
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

  static bool containExceptChar(std::string& subject, const std::string& exceptChars) {
    if (subject.empty())
      return false;
    for (size_t i = 0; i < exceptChars.length(); ++i) {
      std::string exceptChar;
      exceptChar.push_back(exceptChars[i]);
      size_t pos = 0;
      if ((pos = subject.find(exceptChar, 0)) != std::string::npos)
        return true;
      pos = std::string(subject).find("\r", 0);
    }
    return false;
  }

  static bool containExceptChar(std::vector<std::string> stringVec, std::string exceptChars) {
    std::vector<std::string>::iterator it;
    bool  result;
    for (it = stringVec.begin(); it != stringVec.end(); ++it) {
      result = containExceptChar(*it, exceptChars);
      if (result)
        return result;
    }
    return false;
  }


  static std::string& eraseFirstWord(std::string& subject, const std::string& longWord, const std::string& shortWord) {

    size_t  prefix_length;
    if(!subject.empty() && subject[0] == longWord[0] && subject[0] == shortWord[0] &&
        ((prefix_length = subject.find(SPACE, 0)) != std::string::npos) ) {

      std::string toErase;
      if (prefix_length > shortWord.length())
        toErase = std::string(longWord + " ");
      else
        toErase = std::string(shortWord + " ");
      size_t pos = subject.find(toErase);
      if (pos == 0)
        subject.erase(pos, toErase.length());
      SS::trim(subject);
    }
    return subject;
  }

  static std::string& replaceString(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
      subject.replace(pos, search.length(), replace);
      pos += replace.length();
    }
    return subject;
  }

  static std::string& replaceString(std::string& subject, std::vector<std::string> const &searchVec, std::vector<std::string> const &replaceVec) {
    if (searchVec.size() != replaceVec.size())
      return subject;
    size_t  i = 0;
    while (i < searchVec.size()) {
      replaceString(subject, searchVec[i], replaceVec[i]);
      i++;
    }
    return subject;
  }

  static bool isNumber(const std::string& s) {
      std::string::const_iterator it = s.begin();
      while (it != s.end() && std::isdigit(*it)) ++it;
      return !s.empty() && it == s.end();
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
                                              bool bCutOnceMakeTwo = false) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> result;
    int i = 0;
    if (s.empty() && !bCutOnceMakeTwo)
      return result;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
      token = s.substr(pos_start, pos_end - pos_start); //+ delimiter.length());
      if ( (bCutOnceMakeTwo && i == 0 && token.empty()) ||
           (!token.empty()) ) {
        result.push_back(token);  //in case of vector
        i++;
      }

      if (bClearProcessedString == true) {
        s.erase(pos_start, pos_end + delim_len);
        pos_end = pos_end - (token.length() + delim_len);
      }

      pos_start = pos_end + delim_len;
      if (bCutOnceMakeTwo)
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

    //if bCutOnceMakeTwo is true, the result always have two (empty) elements
    if (bCutOnceMakeTwo && result.size() == 0)
      result.push_back(std::string());
    if (bCutOnceMakeTwo && result.size() == 1)
      result.push_back(std::string());

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
