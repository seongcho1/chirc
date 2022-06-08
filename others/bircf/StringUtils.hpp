#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <cerrno>
#include <iostream>
#include <sstream>

#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

#define NEWLINE		"\n"
#define SPACE		" "
#define SPACE_COLON	" :"


class SS {
  public:

	template <typename T>
	static std::string	toString ( T number ) {
		std::ostringstream ss;
		ss << number;
		return ss.str();
	}

	static std::string	toUpper(std::string &s)
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

	static std::vector<std::string>	splitString(std::string &s, std::string delimiter,
												bool bSkipLast = false, bool bClearProcessedString = false,
												bool bCutOnce = false) {
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();
		std::string token;
		std::vector<std::string> result;
		int i = 0;
		if (s.empty())
			return result;

		while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
			token = s.substr(pos_start, pos_end - pos_start); //+ delimiter.length());
			if ( (bCutOnce && i == 0 && token.empty()) ||
				(!token.empty()) ) {
				result.push_back(token);  //in case of vector
				i++;
				if (bClearProcessedString == true) {
					s.erase(pos_start, pos_end + delim_len);
					pos_end = pos_end - token.length();
				}
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
		return result;
	}

	static int		x_int(int err, int res, char *str, char *file, int line) {
		if (res == err) {
			fprintf(stderr, "%s error (%s, %d): %s\n",
			str, file, line, strerror(errno));
			exit (1);
		}
		return (res);
	}

	static	void	*x_void(void *err, void *res, char *str, char *file, int line)
	{
		if (res == err) {
			fprintf(stderr, "%s error (%s, %d): %s\n",
			str, file, line, strerror(errno));
			exit (1);
		}
		return (res);
	}



};


#endif
