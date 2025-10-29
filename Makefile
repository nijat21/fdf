.PHONY: all clean fclean re

NAME=fdf
CC=cc
CFLAGS= -g -Wall -Wextra -Werror

SRCS:= fdf.c projection.c utils.c hooks.c
OBJS:=$(SRCS:%.c=%.o)

LIBFT_DIR=Libft
LIBFT_MAKE=make -C $(LIBFT_DIR)
LIBFT_LIB=$(LIBFT_DIR)/libft.a

# For macOS - current setup
ifeq ($(shell uname), Darwin)
	MLX_DIR=minilibx_macos_metal
    MLX_LIB=$(MLX_DIR)/libmlx.dylib
	LINKS=$(LIBFT_LIB) -L$(MLX_DIR) -lmlx  -Wl,-rpath,$(shell pwd)/$(MLX_DIR) -framework OpenGL -framework AppKit
endif

# For Linux - future setup
ifeq ($(shell uname), Linux)
	MLX_DIR=minilibx-linux
    MLX_LIB=$(MLX_DIR)/libmlx.a
    LINKS=$(LIBFT_LIB) $(MLX_LIB) -lXext -lX11 -lm -lz	
endif

MLX_MAKE=make -C $(MLX_DIR)


all: $(NAME)

$(NAME): $(LIBFT_LIB) $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LINKS) -o $(NAME)

$(LIBFT_LIB):
	$(LIBFT_MAKE) all

# For MacOS
ifeq ($(shell uname), Darwin)
$(MLX_LIB):
	$(MLX_MAKE) all
	install_name_tool -id @rpath/libmlx.dylib $(MLX_LIB)
endif

# For Linux
ifeq ($(shell uname), Linux)
$(MLX_LIB):
	$(MLX_MAKE) all
endif
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