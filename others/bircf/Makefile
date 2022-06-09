SRCS =	main.cpp MessageManager.cpp \
				Commands/Pass.cpp \
				Commands/Nick.cpp \
				Commands/User.cpp \
				Commands/Privmsg.cpp \
				reply.cpp \
				Commands/TESTMSG.cpp \
				Commands/Publicmsg.cpp \
				Commands/Selfmsg.cpp \

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
