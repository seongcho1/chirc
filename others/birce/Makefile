
SRCS =	main.cpp init_env.cpp clean_fd.cpp get_opt.cpp x.cpp main_loop.cpp \
	init_fd.cpp do_select.cpp check_fd.cpp \
	srv_create.cpp srv_accept.cpp \
	client_read.cpp client_write.cpp

OBJS = ${SRCS:.cpp=.o}

NAME = birce

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
