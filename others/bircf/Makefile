SRCS =	main.cpp Ircserv.cpp MessageManager.cpp StringUtils.cpp reply.cpp \
				Commands/3.1ConnectionRegistration/PASS.cpp \
				Commands/3.1ConnectionRegistration/NICK.cpp \
				Commands/3.1ConnectionRegistration/USER.cpp \
				Commands/3.1ConnectionRegistration/QUIT.cpp \
				Commands/3.1ConnectionRegistration/PING.cpp \
				Commands/3.1ConnectionRegistration/PONG.cpp \
				Commands/3.2Channel/AWAY.cpp \
				Commands/3.2Channel/JOIN.cpp \
				Commands/3.2Channel/PART.cpp \
				Commands/3.2Channel/LIST.cpp \
				Commands/3.2Channel/KICK.cpp \
				Commands/3.2Channel/NAMES.cpp \
				Commands/3.2Channel/TOPIC.cpp \
				Commands/3.2Channel/INVITE.cpp \
				Commands/3.3SendingMessages/PRIVMSG.cpp \
				Commands/3.4Mode/MODE.cpp \


OBJS = ${SRCS:.cpp=.o}

NAME = ircserv

CXXFLAGS = -I. -g3 -Wall -Wextra -Werror #-fsanitize=address
LDFLAGS =

CXX = c++
RM = rm -f

${NAME}:	${OBJS}
		@${CXX} ${CXXFLAGS} -o ${NAME} ${OBJS} ${LDFLAGS}

all:		${NAME}

clean:
		${RM} ${OBJS} *~ #*#

fclean:		clean
		${RM} ${NAME}

re:		fclean all
