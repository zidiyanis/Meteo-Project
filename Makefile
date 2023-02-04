NAME	=	meteo
CC	=	gcc
RM	=	rm -f
SRCS	=	 $(wildcard src/*.c)
OBJS	=	$(SRCS:.c=.o)
CFLAGS	=	-Wall -g -I include/ -lm -w

all:	$(NAME)
$(NAME):	$(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(CFLAGS)

clean:
	$(RM) $(OBJS)

