.PHONY: all clean fclean re

NAME=fdf
CC=cc
CFLAGS= -g -Wall -Wextra -Werror

SRCS:= fdf.c utils.c hooks.c
OBJS:=$(SRCS:%.c=%.o)

LIBFT_DIR=Libft
LIBFT_MAKE=make -C $(LIBFT_DIR)
LIBFT_LIB=$(LIBFT_DIR)/libft.a

MLX_DIR=mlx
MLX_MAKE=make -C $(MLX_DIR)

# For macOS - current setup
ifeq ($(shell uname), Darwin)
    MLX_LIB=$(MLX_DIR)/libmlx.dylib
	LINKS = $(LIBFT_LIB) -L$(MLX_DIR) -lmlx  -Wl,-rpath,$(shell pwd)/$(MLX_DIR) -framework OpenGL -framework AppKit
endif

# For Linux - future setup
ifeq ($(shell uname), Linux)
    MLX_LIB=$(MLX_DIR)/libmlx.a
    LINKS = $(LIBFT_LIB) $(MLX_LIB) -lXext -lX11 -lm -lz
endif


all: $(NAME)

$(NAME): $(LIBFT_LIB) $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LINKS) -o $(NAME)

$(LIBFT_LIB):
	$(LIBFT_MAKE) all

$(MLX_LIB):
	$(MLX_MAKE) all
	install_name_tool -id @rpath/libmlx.dylib $(MLX_LIB)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(LIBFT_MAKE) clean
	$(MLX_MAKE) clean

fclean: clean
	rm -f $(NAME)
	$(LIBFT_MAKE) fclean

re: fclean all