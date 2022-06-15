#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <cerrno>
#include <iostream>
#include <sstream>
// #include <cerrno>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <string>
#include <cassert>

#define NEWLINE       "\r\n"
#define SPACE         " "
#define SPACE_COLON   " :"
#define COMMA         ","
#define NUL           "\0"
#define CR            "\r"
#define CR_NUL        "\r\0"
#define SPECIAL       "-[]\\`^{}"

class SS {
public:

  // whildcard *, ?
  static bool matchString(const char* source, const char* pattern);
  static bool matchString(const std::string& source, const std::string& pattern);
  static bool matchString(const std::string& source, const std::vector<std::string>& patternVec);
  static std::vector<std::string>& matchStringVector(std::vector<std::string>& sourceVec, std::string pattern);

  //trim from left
  static std::string& ltrim(std::string& s, const char* t = " \t"); //\n\r\f\v")
  //trim from right
  static std::string& rtrim(std::string& s, const char* t = " \t"); //\n\r\f\v")
  //trim from left & right
  static std::string& trim(std::string& s, const char* t = " \t"); //\n\r\f\v")

  static bool containExceptChar(std::string& subject, const std::string& exceptChars);
  static bool containExceptChar(std::vector<std::string> stringVec, const std::string& exceptChars);

  static std::string join(const std::string& separator, const std::list<std::string>& c);
  static std::list<std::string> duplicateWordList(const std::vector<std::string>& words);
  static std::string  duplicateWordString(const std::vector<std::string>& words);
  static std::string& eraseFirstWord(std::string& subject, const std::string& longWord, const std::string& shortWord);

  static std::string& replaceString(std::string& subject, const std::string& search, const std::string& replace);
  static std::string& replaceString(std::string& subject, std::vector<std::string> const &searchVec, std::vector<std::string> const &replaceVec);
  static bool         isNumber(const std::string& s);

  template <typename T>
  static std::string  toString ( T number ) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
  }

  template <typename T>
  static std::string  toString ( T number, size_t nRightZeros) {
    std::string result = toString(number);
    while (result.size() < nRightZeros)
      result = std::string("0").append(result);
    return result;
  }


  static std::string               toUpper(const std::string &s);
  static std::vector<std::string>  splitString(std::string &s, std::string delimiter,
                                              bool bSkipLast = false,
                                              bool bClearProcessedString = false,
                                              bool bCutOnce = false);



  // CHANNEL_MODE_PARAM_FLAGS_STRING
  static char         first(std::string &s, std::string const &cmp);
  static bool         compare(char const &s, std::string const &cmp);

  static int          x_int(int err, int res, char *str, char *file, int line);
  static void         *x_void(void *err, void *res, char *str, char *file, int line);

  static void         charPrint(std::string &s);
  static bool         isNickChar(std::string &s);
  static std::string  makeOneString(std::vector<std::string>::iterator const &begin, std::vector<std::string>::iterator const &end);

};

#endif
