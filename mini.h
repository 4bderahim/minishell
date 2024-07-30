
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <fcntl.h>
#include <readline/history.h>
#include <string.h>
#include <signal.h>
# include <unistd.h>
# include <stdlib.h>
#include <errno.h>
int match_word(char *neadle, char *str);
size_t	ft_strlen(char *s);

typedef struct s_env{
  char *line;
  struct s_env *next;
} t_env;

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

t_env *env;
struct s_command_line *next;
} t_cmd;
void parse_indetifier(t_cmd *cmd, char *str);
void    env_addback(t_env *head, t_env *new);
char    *ft_strjoin(char *s1, char *s2);
t_env   *env_new(char *new_line);
t_env   *env_getlast(t_env *env);
t_env   *create_env_list(char **env);
size_t	ft_strlen(char *s);
void    ft_echo(char **str, int fd);
int     write_fd(char *str, int fd);
char    *heredoc(char *heredoc_str, int fd);

typedef struct s_all {
  t_cmd *cmd;
  t_env *env;
} t_all;