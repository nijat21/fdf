.PHONY: all clean fclean re

NAME=fdf
CC=cc
CFLAGS= -g -Wall -Wextra -Werror

SRCS:= fdf.c
OBJS:=$(SRCS:%.c=%.o)

LIBFT_DIR=Libft
LIBFT_MAKE=make -C $(LIBFT_DIR)
LIBFT_LIB=$(LIBFT_DIR)/libft.a

MLX_DIR=mlx
MLX_MAKE=make -C $(MLX_DIR)
MLX_LIB=$(MLX_DIR)/libmlx.a

# LINKS = -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
LINKS = $(LIBFT_LIB) $(MLX_LIB) -framework OpenGL -framework AppKit


all:$(NAME)

$(NAME):$(LIBFT_LIB) $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS)$(OBJS) $(LINKS) -o $(NAME)

$(LIBFT_LIB):
	$(LIBFT_MAKE) all

$(MLX_LIB):
	$(MLX_MAKE) all

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(LIBFT_MAKE) clean
	$(MLX_MAKE) clean

fclean:clean
	rm -f $(NAME)
	$(LIBFT_MAKE) fclean

re: fclean all