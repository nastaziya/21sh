.PHONY: all clean fclean re

NOCOLOR=\033[0m
VERT=\033[32;05m
JAUNE=\033[33m

NAME =		21sh
CC = 		gcc
FLAGS = 	-g -Wall -Werror -Wextra
DEL =		rm -rf

SRC_PATH = ./src/
INC_PATH = ./inc/
OBJ_PATH = ./obj/
LIB_PATH = ./libft/

SRC = $(addprefix $(SRC_PATH),$(N_SRCS))
OBJ = $(addprefix $(OBJ_PATH), $(N_SRCS:.c=.o))
INC = -I $(INC_PATH) -I $(LIB_PATH)inc/

N_SRCS = 	executor/exec.c\
			executor/main.c\
			executor/prepare_env.c\
			expansion/expansion_tools.c\
			expansion/expansion.c\
			expansion/init_array.c\
			expansion/recover_env.c\
			expansion/treat_special_chars.c\
			lexer/collect_lines.c\
			lexer/lexer.c\
			lexer/quote_management.c\
			lexer/tokenization.c\
			lexer/utils_lexer.c\
			parser/free_functions.c\
			parser/parser_alloc.c\
			parser/parser_tools.c\
			parser/parser.c\
			parser/print.c\
			termcaps/alt_copy.c\
			termcaps/alt_right_left_key.c\
			termcaps/alt_up_down_key.c\
			termcaps/control_commands.c\
			termcaps/cursor_position.c\
			termcaps/del_key.c\
			termcaps/home_end.c\
			termcaps/left_right_key.c\
			termcaps/pos_char.c\
			termcaps/print_buf.c\
			termcaps/print.c\
			termcaps/signals.c\
			termcaps/termcap.c\
			termcaps/termios.c\
			termcaps/up_down_key.c\
			termcaps/utils.c\

all: $(NAME)

$(NAME):	$(OBJ)
	@$(MAKE) -C $(LIB_PATH)
	@$(CC) $(FLAGS) $(OBJ) $(INC) -L$(LIB_PATH) -lncurses -lft -o $(NAME)
	@echo "$(VERT)$(NAME): compilation success!$(NOCOLOR)"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(OBJ_PATH)/executor
	@mkdir -p $(OBJ_PATH)/expansion
	@mkdir -p $(OBJ_PATH)/lexer
	@mkdir -p $(OBJ_PATH)/parser
	@mkdir -p $(OBJ_PATH)/termcaps
	$(CC) -c $(FLAGS) $(INC) -o $@ $<

clean:
	@$(MAKE) -C $(LIB_PATH) clean
	@$(DEL) $(OBJ_PATH)
	@echo "$(JAUNE)$(NAME): Removing $(NAME) ./obj/$(NOCOLOR)"

fclean: clean
	@$(MAKE) -C $(LIB_PATH) fclean
	@$(DEL) $(NAME)
	@echo "$(JAUNE)$(NAME): Removing executable$(NOCOLOR)"

re: fclean all
