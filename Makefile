CC = cc 
CFLAGS = #-Wall -Wextra -Werror 
CFILES = ft_heredoc.c utils.c ft_export.c exec.c
SFLAG = -lreadline
OFILES = $(CFILES:.c=.o)

PROG_NAME = mini
all : $(PROG_NAME)
$(PROG_NAME) : $(OFILES) 
	$(CC) $(CFLAGS) $(OFILES) -o $(PROG_NAME) $(SFLAG)

%.o: %.c mini.h
	$(CC) $(CFLAGS)  -c $< -o $@
clean: 
	rm -f $(OFILES) $(OBFILES) 
	
fclean: clean 
	 rm -f $(OFILES) $(PROG_NAME) 
	 
re: fclean all 


# Example Makefile
