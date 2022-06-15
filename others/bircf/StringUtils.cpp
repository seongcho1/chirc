#include "StringUtils.hpp"


// whildcard *, ?
bool SS::matchString(const char* source, const char* pattern) {
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

bool SS::matchString(const std::string& source, const std::string& pattern) {
  return matchString(source.c_str(), pattern.c_str());
}

bool SS::matchString(const std::string& source, const std::vector<std::string>& patternVec) {
  std::vector<std::string>::const_iterator it;
  bool result = false;

  for (it = patternVec.begin(); it != patternVec.end(); ++it) {
    result = matchString(source, *it);
    if (result) return result;
  }
  return false;
}

std::vector<std::string>& SS::matchStringVector(std::vector<std::string>& sourceVec, std::string pattern) {
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
std::string& SS::ltrim(std::string& s, const char* t) {
  s.erase(0, s.find_first_not_of(t));
  return s;
}

//trim from right
std::string& SS::rtrim(std::string& s, const char* t) {
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}

//trim from left & right
std::string& SS::trim(std::string& s, const char* t) {
  return ltrim(rtrim(s, t), t);
}

bool SS::containExceptChar(std::string& subject, const std::string& exceptChars) {
  if (subject.empty())
    return false;
  for (size_t i = 0; i < exceptChars.length(); ++i) {
    std::string exceptChar;
    exceptChar.push_back(exceptChars[i]);
    size_t pos = 0;
    if ((pos = subject.find(exceptChar, 0)) != std::string::npos)
      return true;
  }
  return false;
}

bool SS::containExceptChar(std::vector<std::string> stringVec, const std::string& exceptChars) {
  std::vector<std::string>::iterator it;
  bool  result;
  for (it = stringVec.begin(); it != stringVec.end(); ++it) {
    result = containExceptChar(*it, exceptChars);
    if (result)
      return result;
  }
  return false;
}

std::string SS::join(const std::string_view& separator, const std::list<std::string>& c) {
  std::stringstream ss;

  bool add_separator = false;
  std::list<std::string>::const_iterator it;
  for (it = c.begin(); it != c.end(); ++it) {
      if (add_separator) ss << separator;
      ss << *it;
      add_separator = true;
  }
  return ss.str();
}

std::list<std::string> SS::duplicateWordList(const std::vector<std::string>& words) {
    std::map<std::string, int> temp;
    std::list<std::string> ret;
    for (std::vector<std::string>::const_iterator iter = words.begin(); iter != words.end(); ++iter) {
        temp[*iter] += 1;
        // only add the word to our return list on the second copy
        // (first copy doesn't count, third and later copies have already been handled)
        if (temp[*iter] == 2) {
            ret.push_back(*iter);
        }
    }
    ret.sort();
    return ret;
}

std::string SS::duplicateWordString(const std::vector<std::string>& words) {
  std::string result;
  std::list<std::string> duplicated;

  duplicated = duplicateWordList(words);
  result = join(", ", duplicated);
  return result;
}


std::string& SS::eraseFirstWord(std::string& subject, const std::string& longWord, const std::string& shortWord) {

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


std::string& SS::replaceString(std::string& subject, const std::string& search, const std::string& replace) {
  size_t pos = 0;
  while((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
  return subject;
}

std::string& SS::replaceString(std::string& subject, std::vector<std::string> const &searchVec, std::vector<std::string> const &replaceVec) {
  if (searchVec.size() != replaceVec.size())
    return subject;
  size_t  i = 0;
  while (i < searchVec.size()) {
    replaceString(subject, searchVec[i], replaceVec[i]);
    i++;
  }
  return subject;
}



bool SS::isNumber(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

std::string  SS::toUpper(const std::string &s)
{
  std::string str(s);

  for (std::size_t i = 0; i < str.size(); i++) {

    str[i] = (std::toupper(str[i]));

    // 123:{ => 91:[
    // 124:| => 92:\
    // 125:} => 93:]
    // 94:^ => 126:~
    switch(str[i]) {

      case '{' : str[i] = '[';
            break;
      case '|' : str[i] = '\\';
            break;
      case '}' : str[i] = ']';
            break;
      case '^' : str[i] = '~';
            break;
      default:
            break;
    }
  }

  //std::transform(str.begin(), str.end(), str.begin(), ::toupper);

  //int (*tu)(int) = toupper; // Select that particular overload
  //std::transform(str.begin(),str.end(),str.begin(),tu );

  //Since C++11, we could use a lambda:
  //std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
  return str;
}


std::vector<std::string>  SS::splitString(std::string &s, std::string delimiter,
                                            bool bSkipLast,
                                            bool bClearProcessedString,
                                            bool bCutOnce) {

  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> result;
  int i = 0;
  if (s.empty() && !bCutOnce)
    return result;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {

    token = s.substr(pos_start, pos_end - pos_start); //+ delimiter.length());
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
  // make empty trailing
  // "aaa"      returns (aaa)
  // "aaa :bbb" returns (aaa), (bbb)
  // "aaa :"    returns (aaa), ()
  // " :bbb"    returns (), (bbb)
  // " :"       returns (),()
  // NULL       returns NULL

  if ( bCutOnce && pos_end != std::string::npos &&
        (pos_start - pos_end == delim_len) && result.size() == 1) {
    result.push_back(std::string());
  }
  return result;
}


// CHANNEL_MODE_PARAM_FLAGS_STRING
char   SS::first(std::string &s, std::string const &cmp) {
  for (int i = 0; i < (int)s.length(); ++i) {
    if (compare(s[i], cmp))
      return s[i];
  }
  return 0;
}

bool SS::compare(char const &s, std::string const &cmp) {
  for (int i = 0; i < (int)cmp.length(); ++i) {
    if (cmp[i] == s)
      return true;
  }
  return false;
}

void SS::charPrint(std::string &s) {
  std::string::iterator si = s.begin();
  while (si != s.end())
    std::cout << "{" << (int)*si++ << "}";
  std::cout << std::endl;
}

bool SS::isNickChar(std::string &s) {
  if (!isalpha(s[0]))
    return false;

  for (int i = 0; i < (int)s.length(); ++i) {
    if (isalpha(s[i]))
      continue;
    if (isnumber(s[i]))
      continue;
    if (compare(s[i], SPECIAL))
      continue;
    return false;
  }
  return true;
}

std::string SS::makeOneString(std::vector<std::string>::iterator const &begin, std::vector<std::string>::iterator const &end) {
  std::string result;
  std::vector<std::string>::iterator it = begin;
  while (it != end)
    result.append(*it++).append(" ");

  if (*result.end() == ' ')
    result.pop_back();

  return result;
}

int    SS::x_int(int err, int res, char *str, char *file, int line) {
  if (res == err) {
    fprintf(stderr, "%s error (%s, %d): %s\n",
    // str, file, line, strerror(errno));
    str, file, line, "[windows] can not use strerror :(");
    exit (1);
  }
  return (res);
}

void  *SS::x_void(void *err, void *res, char *str, char *file, int line) {
  if (res == err) {
    fprintf(stderr, "%s error (%s, %d): %s\n",
    // str, file, line, strerror(errno));
    str, file, line, "[windows] can not use strerror :(");
    exit (1);
  }
  return (res);
}
