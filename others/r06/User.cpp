#include "User.hpp"

int	User::last_id_ = 0;

User::User(int const &fd) : fd_(fd) {
	id_ = getNewId();
}

User::User(User const &other) : id_(other.id_), fd_(other.fd_) {}

User &User::operator=(User const &other) {
	if (this != &other) {
		id_ = other.id_;
		fd_ = other.fd_;
	}
	return *this;
}

User::~User() {
}


int		User::getNewId() {
	return ++last_id_;
}
