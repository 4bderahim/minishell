CC = cc
CFLAGS = #-fsanitize=address -g #-Wall -Wextra -Werror
XFLAGS = #-I ~/.brew/opt/readline/include
X_FLAGS = #-L ~/.brew/Cellar/readline/8.2.13/lib

HEADER = -I ./include/

NAME = minishell

PARSING_SRCS = ./parsing/main.c ./parsing/ft_libc1.c ./parsing/ft_split.c \
								./parsing/ft_list.c ./parsing/lexer.c ./parsing/utils_1.c \
								./parsing/cmd_infos.c ./parsing/variables.c ./parsing/cmd_infos_2.c \
								./parsing/executables.c ./parsing/ft_itoa.c
PARSING_OBJS = ${PARSING_SRCS:.c=.o}

EXEC_SRCS = ./execution/execute_piped_ins.c ./execution/lst_utils.c ./execution/redirs_heredoc.c  ./execution/env_utils.c ./execution/ft_chdir.c ./execution/exec.c  ./execution/pre_exit.c ./execution/execute_builtins.c ./execution/unset_vars.c \
 						./execution/export_list.c  ./execution/ft_heredoc.c ./execution/utils.c \
						./execution/ft__export.c 
EXEC_RD = ./exec/signals_utils.o 
EXEC_OBJS = ${EXEC_SRCS:.c=.o}

all: $(NAME)

$(NAME): $(PARSING_OBJS) $(EXEC_OBJS) $(EXEC_RD)
	$(CC) $(CFLAGS)  $(X_FLAGS) $(EXEC_RD) $(PARSING_OBJS) $(EXEC_OBJS) -o $(NAME) -lreadline $(XFLAGS)

./parsing/%.o: ./parsing/%.c ./include/minishell.h
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $@
./execution/%.o: ./execution/%.c ./include/minishell.h
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $@
./exec/signals_utils.o: ./exec/signals_utils.c ./include/minishell.h
	$(CC) $(CFLAGS) $(HEADER)  $(XFLAGS) -c  $< -o $@  
clean:
	rm -f $(PARSING_OBJS) $(EXEC_OBJS) $(EXEC_RD)

fclean: clean
	rm -rf $(NAME)

re: fclean all
