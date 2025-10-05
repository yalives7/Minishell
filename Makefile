NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

LIBFT_DIR = libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a


READLINE_INCLUDE = -I/opt/homebrew/opt/readline/include
READLINE_LIB = -L/opt/homebrew/opt/readline/lib


INCLUDES = -I includes -I $(LIBFT_DIR) $(READLINE_INCLUDE)


LIBS = -L$(LIBFT_DIR) -lft $(READLINE_LIB) -lreadline



MAIN_SRCS = srcs/main.c \
            srcs/init.c


LEXER_SRCS = srcs/lexer/lexer.c \
             srcs/lexer/lexer_metachar.c \
             srcs/lexer/lexer_utils.c \
             srcs/lexer/lexer_word.c 
PARSER_SRCS = srcs/parser/parser.c \
              srcs/parser/parser_command.c \
              srcs/parser/parser_redirection.c \
              srcs/parser/parser_syntax.c 
EXPANDER_SRCS = srcs/expander/expander.c \
                srcs/expander/expander_main.c \
                srcs/expander/expander_dollar.c \
                srcs/expander/expander_quotes.c \
                srcs/expander/expander_word_split.c \
                srcs/expander/expander_utils.c \
                srcs/expander/expander_utils2.c \
                srcs/expander/expander_split_utils.c 
EXECUTOR_SRCS = srcs/executor/executor.c \
                srcs/executor/executor_utils.c \
                srcs/executor/executor_child.c \
                srcs/executor/executor_command.c \
                srcs/executor/executor_process.c \
                srcs/executor/executor_pipeline.c \
                srcs/executor/heredoc.c \
                srcs/executor/heredoc_utils.c \
                srcs/executor/heredoc_utils2.c \
                srcs/executor/redirection.c 
BUILTINS_SRCS = srcs/builtins/builtins.c \
                srcs/builtins/builtin_cd.c \
                srcs/builtins/builtin_echo.c \
                srcs/builtins/builtin_env.c \
                srcs/builtins/builtin_export.c \
                srcs/builtins/builtin_pwd.c \
                srcs/builtins/builtin_unset.c \
                srcs/builtins/builtin_exit.c
BUILTINS_UTILS_SRC = srcs/builtins/utils/cd_utils.c \
                     srcs/builtins/utils/env_utils.c \
                     srcs/builtins/utils/exit_utils.c \
                     srcs/builtins/utils/exit_utils2.c \
                     srcs/builtins/utils/error_builtins.c \
                     srcs/builtins/utils/export_utils.c \
                     srcs/builtins/utils/export2_utils.c \
                     srcs/builtins/utils/unset_utils.c 
UTILS_SRCS = srcs/signal/signals.c
MEMORY_SRCS = srcs/memory/memory_alloc.c \
              srcs/memory/memory_array.c \
              srcs/memory/memory_structs.c
ERROR_SRCS = srcs/error/error_handler.c \
             srcs/error/error_handler2.c


SRCS = $(MAIN_SRCS) $(LEXER_SRCS) $(PARSER_SRCS) $(EXPANDER_SRCS) $(EXECUTOR_SRCS) $(BUILTINS_SRCS) $(UTILS_SRCS) $(BUILTINS_UTILS_SRC) $(MEMORY_SRCS) $(ERROR_SRCS)


OBJS = $(SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(LIBFT_LIB):
	@make -C $(LIBFT_DIR)



%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all