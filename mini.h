
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <readline/readline.h>
#include <fcntl.h>
#include <readline/history.h>
#include <string.h>
# include <unistd.h>
# include <stdlib.h>


typedef struct cmd
{
  char *command;
  char *flags;
  char dir_file;
  struct cmd *next;
  int pipes_n;
} cmd_t;