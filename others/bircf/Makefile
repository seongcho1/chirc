SRCS =	main.cpp MessageManager.cpp \
				Commands/Pass.cpp \
				Commands/Nick.cpp \
				Commands/Privmsg.cpp \
				Commands/TESTMSG.cpp \
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
