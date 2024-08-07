
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
typedef struct s_all t_all;
typedef struct s_env{
  char *variable;
  char *value;
  struct s_env *next;
  struct s_env *prev;
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

t_all *all;
t_env *env;
struct s_command_line *next;
} t_cmd;

typedef struct s_exp{
  char *variable;
  char *value;
  struct s_exp *prev;
  struct s_exp *next;
} t_exp;

typedef struct s_dec{
  char *variable;
  char *value;
  struct s_dec *next;
} t_dec;

typedef struct s_all
{
   t_cmd    *cmd; // our parsing struct
   t_env    *env; // environment variables list
   t_exp    *exp; // exported variables list
  // t_dec     *dec; // declared variables list
  // ... any other structs
  
} t_all;
// void    print_exp_list(t_all *all, int pipes[2]);// for test
// void    print_env_list(t_all *all);// for test
void    exec_piped_built_ins(t_all *all, int pipes[2]);
void    heredoc_check(t_all *all);
t_exp   *new_exp_(t_env *env);
int     spliter_index(char *str);
t_exp   *exp_new(char *new_line);// not used 
void    exp_addback(t_exp    *head, t_exp    *new);
t_exp   *set_export_list(t_all *all, char **env);
void    identifier_error(char *indentifer);
void    ft_write(char *str, int fd);
void    change_dir(t_all *all, char *new_dir);
void    ft_pwd(t_all *all);
void    parse_indetifier(t_all *all, char *str);
void    unset_env(t_all *all);
void    unset_exp(t_all *all);
void    env_addback(t_env *head, t_env *new);
char    *ft_strjoin(char *s1, char *s2);
t_env   *env_new(char *new_line);
t_env   *env_getlast(t_env *env);
t_env   *create_env_list(char **env);
size_t	ft_strlen(char *s);
void    ft_echo(char **str, int fd);
char    *heredoc(char *heredoc_str, int fd);

// typedef struct s_all {
//   t_cmd *cmd;
//   t_env *env;
// } t_all;