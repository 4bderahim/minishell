
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

int match_word(char *neadle, char *str);
size_t	ft_strlen(char *s);
typedef struct cmd
{
  char *command;
  char *flags;
  char dir_file;
  struct cmd *next;
  int pipes_n;
} cmd_t;
size_t	ft_strlen(char *s);
void ft_echo(char **str, int fd);
int write_fd(char *str, int fd);
char *heredoc(char *heredoc_str, int fd);
typedef struct s_command_line {
char *cmd; 
char *full_path; 
char **args; 
int arg_count; 
char *in_file;
char *out_file; 
char *append_file; 
char *heredoc_delimiter; 
char *heredoc_content; 
int pipe; 
struct s_command_line *next;
} t_cmd;