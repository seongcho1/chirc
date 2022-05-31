#ifndef Channel_HPP
#define Channel_HPP

#include <iostream>
#include <map>

class Channel {

  private:

	int		id_;
	std::string name_;
	std::string topic_;
	std::map<int, User> userMap;

  public:

	int	const	&id(void) const { return _fd; }
	std::string const &getName() {return name_; }
	std::string const &getTopic() { return topic_; }

	void	setName(std::string const &name) { name_ = name; }
	void	setTopic(std::string const &topic) { topic_ = topic; }

	Channel();
	Channel(Channel const &other);
	Channel &operator=(Channel const &other);
	~Channel();

};

#endif
