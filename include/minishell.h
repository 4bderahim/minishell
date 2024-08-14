#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define SINGLE_QUOTE '\''
#define DOUBLE_QUOTE '\"'
#define PIPE '|'
#define SPACE ' '
#define IN_RED '<'
#define OUT_RED '>'
#define VAR_SIGN '$'
#define BACK_SLASH '\\'

# define NC "\e[0m"
# define YELLOW "\e[1;33m"
# define GREEN "\x1b[32m"
# define CYAN "\x1b[36m"
# define RED "\x1b[31m"
# define BLUE "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define WHITE "\x1b[37m"

typedef struct s_command_line {
  char *cmd; // The command name (e.g., "echo", "ls")
  char *full_path; // The full path to the command (e.g., "/bin/echo", "/bin
  char **args; // Array of arguments including the command
  int  arg_count; // Number of arguments
  char *in_file; // For input redirection (<)
  char *out_file; // For output redirection (>)
  char *append_file; // For append redirection (>>)
  // char *append_from_file; // For append redirection (>>)
  char *heredoc_delimiter; // For heredoc (<<)
  char *heredoc_content; // Content of heredoc
  int   pipe; // 1 if this command pipes to next, 0 otherwise
  struct s_command_line *next; // Pointer to next command in pipeline
} t_cmd;

typedef struct s_lexer
{
  int i;
  int j;
  char *buffer;
  int pipe_idx;
  int words;
  char **args;
  size_t length;
  int pipe;
} t_lexer;

typedef struct s_env{
  char *variable;
  char *value;
  struct s_env *next;
  struct s_env *prev;
} t_env;

typedef struct s_exp{
  char *variable;
  char *value;
  struct s_exp *prev;
  struct s_exp *next;
} t_exp;

typedef struct s_all
{
  t_cmd    *cmd; // our parsing struct
  t_env      *env; // environment variables list
  t_exp     *exp; // exported variables list
  size_t     nums_of_cmds;
} t_all;

// libc functions
char *ft_strdup(char *str);
char	*ft_strjoin(char   *s1, char   *s2);
size_t ft_strlen(char *str);
char	*ft_substr(char   *s, unsigned int start, size_t len);
size_t	ft_strlcpy(char *dst,   char *src, size_t dstsize);
char    **ft_split(char   *s, char c);
int ft_strcmp(char *s1, char *s2);
int ft_isalpha(char c);
int ft_strchr(char *str, char c);
int ft_strchr_pro(char *str, char c1, char c2, bool inside_quotes);
int ft_isspace(char c);

// ft_list.c
t_cmd	*ft_lstnew(char **args, int args_nbr, int pipe);
void	ft_lstadd_back(t_cmd **lst, t_cmd *);
// void  ft_init(t_shell *shell);
void    ft_lstclear(t_cmd **lst);

// ft_lexer.c
int ft_lexer(char *command, t_all **all);
bool is_symbol(char c);

// utils_1.c
void throw_error(char *msg);
int find_pipe_index(char *str);
size_t args_counter(char *str, int len);
void ft_free(char **args);
void print_list(t_cmd *head);
char *find_and_remove(char *str, char c);
void skip_reds(char *str, int *i, char c);

// cmd_infos.c
char *get_path(char *cmd);
char *get_input_redirection_file(char **args);
char *get_output_redirection_file(char **args);
// char *get_append_from_file(char **args);
char *get_append_to_file(char **args);

// ----------------------------------------------
void    change_dir(t_all *all, char *new_dir);
void    reset_signal_handlers() ;
void    signal_handler(int signo);
void    setup_signal_handlers();
void    env_exp_lists_clear(t_all *all);
void    free_env_list(t_all *all);
void    ft_error(t_all *all);
int     exec_built_ins(t_all *all);
void    setup_signal_handlers();
void    set_lists(t_all *all, char **env);
void    exec_piped_built_ins(t_all *all, int pipes[2]);
void    heredoc_check(t_all *all);
t_exp   *new_exp_(t_env *env);
int     spliter_index(char *str);
t_exp   *exp_new(char *new_line);// not used
void    exp_addback(t_exp    *head, t_exp    *);
t_exp   *set_export_list(t_all *all, char **env);
void    identifier_error(char *indentifer);
void    ft_write(char *str, int fd);
void    change_dir(t_all *all, char *new_dir);
void    ft_pwd(t_all *all);
void    parse_indetifier(t_all *all, char *str);
void    unset_env(t_all *all);
void    unset_exp(t_all *all);
void    env_addback(t_env *head, t_env *);
char    *ft_strjoin(char *s1, char *s2);
t_env   *env_new(char *new_line);
t_env   *env_getlast(t_env *env);
t_env   *create_env_list(char **env);
size_t	ft_strlen(char *s);
void    ft_echo(char **str, int fd);
char    *heredoc(char *heredoc_str, int fd, t_all *all);
void execution(t_all **all, char *envp[]);
int match_word(char *neadle, char *str);

#endif
