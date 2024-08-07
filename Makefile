CC = cc 
CFLAGS = -fsanitize=address -g #-Wall -Wextra -Werror 
CFILES = execute_builtins.c unset_vars.c export_list.c  ft_heredoc.c utils.c ft_export.c exec.c
LIBS = 
SFLAG = -lreadline
OFILES = $(CFILES:.c=.o)

PROG_NAME = mini
all : $(PROG_NAME)
#lib/libreadline.a
$(PROG_NAME) : $(OFILES) 
	$(CC) $(CFLAGS) $(OFILES) -o $(PROG_NAME) -lreadline  -L  /.brew/Cellar/readline/8.2.10/lib  -I /.brew/Cellar/readline/8.2.10/include

%.o: %.c mini.h
	$(CC) $(CFLAGS)  -c $< -o $@
clean: 
	rm -f $(OFILES) $(OBFILES) 
	
fclean: clean 
	 rm -f $(OFILES) $(PROG_NAME) 

re: fclean all 