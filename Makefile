NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Iincludes
RM = rm -f

SRC =	src/main.c \
	src/builtins.c \
	src/cd_1.c src/cd_2.c \
	src/echo.c \
	src/env.c \
	src/execute_1.c src/execute_2.c src/execute_3.c src/execute_4.c \
	src/exit.c \
	src/expand_env_1.c src/expand_env_2.c src/expand_env_3.c src/expand_env_4.c src/expand_env_5.c \
	src/export_1.c src/export_2.c src/export_3.c \
	src/free.c \
	src/heredocs_1.c src/heredocs_2.c \
	src/init.c \
	src/is_something.c \
	src/lexer_1.c src/lexer_2.c src/lexer_3.c \
	src/parser_1.c src/parser_2.c src/parser_3.c \
	src/prompt.c \
	src/redirections_1.c src/redirections_2.c \
	src/shell_loop.c \
	src/signals.c \
	src/unset.c \
	src/utils_1.c src/utils_2.c

OBJ_SRC = $(SRC:.c=.o)
LIBFT_DIR = includes/libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) 

$(NAME): $(OBJ_SRC) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ_SRC) $(LIBFT) -lreadline -o $(NAME)

clean:
	$(MAKE) clean -C $(LIBFT_DIR) 
	@$(RM) $(OBJ_SRC)

fclean: clean
	@$(RM) $(LIBFT) $(NAME)
	@$(RM) .tmp*

re: fclean all
