NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror

RM = rm -f
AR = ar rcs

SRCS = ft_putchar_fd.c \
       ft_putstr_fd.c \
       ft_atoi.c \
       ft_split.c \
       ft_strchr.c \
       ft_strdup.c \
       ft_strjoin.c \
       ft_strncpy.c \
       ft_strlcpy.c \
       ft_strlen.c \
       ft_strncmp.c \
       ft_strcmp.c \
       ft_strnstr.c \
       ft_isalnum.c \
       ft_memmove.c \
       ft_itoa.c \
       ft_atol.c \
       ft_isalpha.c \
       ft_isdigit.c \
       ft_isspace.c \
       ft_putendl_fd.c \
       ft_substr.c


BONUS_SRCS = ft_lstadd_back.c \
             ft_lstadd_front.c \
             ft_lstnew.c \
             ft_lstsize.c \
             ft_lstlast.c \
             ft_lstclear.c


OBJS = $(SRCS:.c=.o) $(BONUS_SRCS:.c=.o)

all: $(NAME)


$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)
 
re: fclean all

.PHONY: all clean fclean re
