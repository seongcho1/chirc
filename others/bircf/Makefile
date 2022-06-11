SRCS =	main.cpp MessageManager.cpp \
				Commands/3.1ConnectionRegistration/PASS.cpp \
				Commands/3.1ConnectionRegistration/NICK.cpp \
				Commands/3.1ConnectionRegistration/USER.cpp \
				Commands/3.3SendingMessages/PRIVMSG.cpp \
				Commands/TESTMSG.cpp \
				Commands/PUBLICMSG.cpp \
				Commands/SELFMSG.cpp \
				reply.cpp \

OBJS = ${SRCS:.cpp=.o}

NAME = ircserv

CXXFLAGS = -I. -g3 -Wall -Wextra -Werror #-fsanitize=address
LDFLAGS =

CXX = c++
RM = rm -f

${NAME}:	${OBJS}
		${CXX} ${CXXFLAGS} -o ${NAME} ${OBJS} ${LDFLAGS}

all:		${NAME}

clean:
		${RM} ${OBJS} *~ #*#

fclean:		clean
		${RM} ${NAME}

re:		fclean all
