.PHONY: all clean fclean re

NAME=fdf
CC=CC
CFLAGS= -g -Wall -Wextra -Werror

SRCS:= fdf.c
OBJS:=$(SRCS:%.c=%.o)

LIBFT_DIR=Libft
LIBFT_MAKE=make -C $(LIBFT_DIR)
LIBFT_LIB=$(LIBFT_DIR)/libft.a

all:$(NAME)

$(NAME):$(LIBFT_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) -o $(NAME)

$(LIBFT_LIB):
	$(LIBFT_MAKE) all

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(LIBFT_MAKE) clean

fclean:clean
	rm -f $(NAME)
	$(LIBFT_MAKE) fclean

re: fclean all