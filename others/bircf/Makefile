
SRCS =	main.cpp MessageManager.cpp \

OBJS = ${SRCS:.cpp=.o}

NAME = bircf

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
