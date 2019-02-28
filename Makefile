.PHONY : clean fclean all re

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
EXEC = executor/
EXEC_FILES = 	env_from_file.c\
				exec.c\
				exec_2.c\
				exec_all_cases.c\
				exec_err_return.c\
				exec_norm.c\
				exec_utils.c\
				main.c\
				pipe2.c\
				prepare_env.c\
				redirections.c\
				utils_fd.c\

EXEC_SRC = $(addprefix $(EXEC), $(EXEC_FILES)) 
TERM = termcaps/
TERM_FILES = 	alt_copy.c\
				alt_right_left_key.c\
				alt_up_down_key.c\
				control_commands.c\
				ctrl_d.c\
				cursor_position.c\
				del_key.c\
				expanse_heredoc.c\
				home_end.c\
				left_right_key.c\
				norm_termcap.c\
				pos_char.c\
				print.c\
				print_2.c\
				print_buf.c\
				signals.c\
				termcap.c\
				termios.c\
				up_down_key.c\
				utils.c\
				win_resize.c\

TERM_SRC = $(addprefix $(TERM), $(TERM_FILES)) 
LEX = lexer/
LEX_FILES =		collect_lines.c\
				lexer.c\
				quote_management.c\
				tokenization.c\
				utils_lexer.c\

LEX_SRC = $(addprefix $(LEX), $(LEX_FILES)) 
PARSER = parser/
PARSER_FILES =	free_functions.c\
				heredoc.c\
				parser.c\
				parser_alloc.c\
				parser_tools.c\
				print.c\
				utils_heredoc.c\
				utils_parser.c\

PARSER_SRC = $(addprefix $(PARSER), $(PARSER_FILES)) 
BUILTINS = builtins/
BUILT_FILES = 	builtin.c\
				cd.c\
				cd2.c\
				cd_utils.c\
				echo.c\
				env.c\
				exit.c\
				setenv.c\
				unsetenv.c\
				utils_2.c\
				utils_env.c\
				utils_env_2.c\

BUILTINS_SRC = $(addprefix $(BUILTINS), $(BUILT_FILES)) 
EXP = expansion/
EXP_FILES =		expansion.c\
				expansion_tools.c\
				init_array.c\
				recover_env.c\
				treat_special_chars.c\

EXP_SRC = $(addprefix $(EXP), $(EXP_FILES)) 
SRC = $(addprefix $(SRC_PATH), $(EXP_SRC) $(BUILTINS_SRC) $(LEX_SRC) $(PARSER_SRC) $(TERM_SRC) $(EXEC_SRC))
OBJ = $(addprefix $(OBJ_PATH), $(EXP_SRC:.c=.o) $(BUILTINS_SRC:.c=.o) $(LEX_SRC:.c=.o) $(PARSER_SRC:.c=.o) $(TERM_SRC:.c=.o) $(EXEC_SRC:.c=.o))

INC = $(addprefix -I, $(INC_PATH))

INC_FILES = 	builtin.h\
				exec.h\
				expansion.h\
				lexer.h\
				parser.h\
				sh.h\
				termcap.h\

INC_DIR = $(addprefix $(INC_PATH), $(INC_FILES))

all: $(NAME) libft/libft.a

$(NAME): $(OBJ)
	@$(MAKE) -C ./libft
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME) -L $(LIB_PATH) -lft -lncurses
	@echo "$(VERT)$(NAME): compilation success!$(NOCOLOR)\n"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INC_DIR) libft/libft.h
	@mkdir -p $(OBJ_PATH)/builtins
	@mkdir -p $(OBJ_PATH)/executor
	@mkdir -p $(OBJ_PATH)/expansion
	@mkdir -p $(OBJ_PATH)/lexer
	@mkdir -p $(OBJ_PATH)/parser
	@mkdir -p $(OBJ_PATH)/termcaps
	$(CC) -c $(FLAGS) $(INC) -o $@ $<

clean:
	@/bin/rm -rf $(OBJ_PATH)
	@$(MAKE) clean -C  libft
	@echo "$(JAUNE)$(NAME): Removing $(NAME) ./obj/$(NOCOLOR)"

fclean: clean
	@/bin/rm -f $(NAME)
	@$(MAKE) fclean -C libft
	@echo "$(JAUNE)$(NAME): Removing executable$(NOCOLOR)"

re: fclean all

