NAME = libft.a

CC = gcc

CFLAGS = -Wall -Werror -Wextra

INC_PATH = ./

INC_DIR = ./libft.h

FILES = ft_atoi.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_toupper.c \
			ft_isclear.c ft_isdigit.c ft_islower.c ft_isprint.c ft_isupper.c\
			ft_itoa.c ft_memalloc.c ft_memccpy.c ft_memchr.c ft_memcmp.c \
			ft_memcpy.c ft_memdel.c ft_memmove.c ft_memset.c ft_putchar.c \
			ft_putchar_fd.c ft_putendl.c ft_putendl_fd.c ft_putnbr.c \
			ft_putnbr_fd.c ft_putstr.c ft_putstr_fd.c ft_strcat.c \
			ft_strchr.c ft_strclr.c ft_strcmp.c ft_strcpy.c ft_strdel.c \
			ft_strdup.c ft_strequ.c ft_striter.c ft_striteri.c ft_strjoin.c\
			ft_strlcat.c ft_strlen.c ft_strmap.c ft_strmapi.c ft_strncat.c \
			ft_strncmp.c ft_strncpy.c ft_strnequ.c ft_strnew.c ft_strnstr.c\
			ft_bzero.c ft_strrchr.c ft_strsplit.c ft_strstr.c ft_strsub.c\
			ft_strtrim.c ft_tolower.c ft_lstadd.c ft_lstdel.c ft_lstdelone.c\
			ft_lstiter.c ft_lstmap.c ft_lstnew.c ft_strrev.c \
			ft_sort_integer_table.c get_next_line.c ft_replace_spaces_inside_quotes.c \
			ft_strsplit_quote.c ft_strsplit_quote_new.c ft_strndup.c \
			ft_dlstdel.c ft_dlstdelone.c ft_dlstadd.c ft_dlstnew.c

SRC = $(FILES)

GREEN = \033[0;32m

OBJ = $(SRC:.c=.o)

INC = $(addprefix -I,$(INC_PATH))

all: $(NAME)

$(NAME): $(OBJ)
		@ar rcs $(NAME) $(OBJ)

%.o: %.c $(INC_DIR) 
		@printf "$(GREEN)Created $@\r"
		@$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
		@/bin/rm -f $(OBJ)

fclean: clean
		@/bin/rm -f $(NAME) $(NAME).dSYM


re: fclean all

.PHONY : clean fclean all re