CC = cc
CFLAGS = #-fsanitize=address -g #-Wall -Wextra -Werror
XFLAGS =  -L /Users/ael-krid/.brew/Cellar/readline/8.2.10/lib -I /Users/ael-krid/.brew/Cellar/readline/8.2.10/include 

HEADER = -I ./include/

NAME = minishell

PARSING_SRCS = ./parsing/main.c ./parsing/ft_libc1.c ./parsing/ft_split.c \
								./parsing/ft_list.c ./parsing/lexer.c ./parsing/utils_1.c ./parsing/cmd_infos.c
PARSING_OBJS = ${PARSING_SRCS:.c=.o}

EXEC_SRCS =  ./execution/execute_builtins.c ./execution/unset_vars.c \
 						./execution/export_list.c  ./execution/ft_heredoc.c ./execution/utils.c \
						./execution/ft_export.c 
EXEC_RD = ./exec/exec.o
EXEC_OBJS = ${EXEC_SRCS:.c=.o}

all: $(NAME)

$(NAME): $(PARSING_OBJS) $(EXEC_OBJS) $(EXEC_RD)
	$(CC) $(CFLAGS) $(PARSING_OBJS) $(EXEC_RD) $(EXEC_OBJS) -o $(NAME) -lreadline $(XFLAGS)

./parsing/%.o: ./parsing/%.c ./include/minishell.h
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

exec/%.o: ./exec/exec.c ./include/minishell.h
	$(CC) $(CFLAGS) $(XFLAGS) $(HEADER) -c $< -o $@
./execution/%.o: ./execution/%.c ./include/minishell.h
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $@
clean:
	rm -f $(PARSING_OBJS) $(EXEC_OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
