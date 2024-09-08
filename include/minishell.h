/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 00:13:46 by mben-jad          #+#    #+#             */
/*   Updated: 2024/09/07 17:43:43 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

int							g_signaled;

typedef struct s_command_line
{
	char					*cmd;
	char					*full_path;
	char					**args;
	int						arg_count;
	char					*in_file;
	char					*out_file;
	char					*append_file;
	char					**heredoc_delimiter;
	char					*heredoc_content;
	int						pipe;
	struct s_command_line	*next;
	bool					cmd_not_found;
	char					*varibale;
	bool					is_red_to_var;
}							t_cmd;

typedef struct s_lexer
{
	int						i;
	int						j;
	char					*buffer;
	int						pipe_idx;
	int						words;
	char					**args;
	int						length;
	int						pipe;
}							t_lexer;

typedef struct s_vars
{
	char					**envpp;
	int						pr_fd;
	pid_t					*pids;

}							t_vars;

typedef struct s_env
{
	char					*variable;
	char					*value;
	struct s_env			*next;
	struct s_env			*prev;
}							t_env;

typedef struct s_exp
{
	char					*variable;
	char					*value;
	struct s_exp			*prev;
	struct s_exp			*next;
}							t_exp;

typedef struct s_all
{
	t_cmd					*cmd;
	t_env					*env;
	t_exp					*exp;
	int						nums_of_cmds;
	int						pipes_num;
	t_vars					*_vars;
	bool					error;
	int						exit_status;
}							t_all;

typedef struct s_fix_cmd
{
	int						i;
	int						j;
	int						k;
	char					*line;
	char					quote;
	int						reds_nums;
}							t_fix_cmd;

char						*ft_strdup(char *str);
char						*ft_strjoin(char *s1, char *s2);
int							ft_strlen(char *str);
char						*ft_substr(char *s, int start, int len);
size_t						ft_strlcpy(char *dst, char *src, int dstsize);
char						**ft_split(char *s, char c);
int							ft_strcmp(char *s1, char *s2);
int							ft_isalpha(char c);
int							ft_strchr(char *str, char c);
int							ft_strchr_pro(char *str, char c1, char c2);
int							ft_isspace(char c);
char						*ft_strndup(char *str, int n);
char						*ft_itoa(int n);
t_cmd						*ft_lstnew(t_all **all, char **args, int args_nbr,
								int pipe);
void						ft_lstadd_back(t_cmd **lst, t_cmd *new);
void						ft_lstclear(t_cmd **lst);
int							ft_lexer(char *command, t_all **all);
bool						is_symbol(char c);
void						skip_str_inside_quote(char *cmd, int *i, char c);
char						*fix_cmd(char *cmd, t_all *all);
bool						no_herdoc_delemiter(char *cmd, int i);
void						throw_error(char *msg, t_all *all, int exit_status);
int							find_pipe_index(char *str);
int							args_counter(char *str, int len);
void						ft_free(char **args, int len);
char						*find_and_remove(char *str, char c);
int							skip_reds(char *str, int *i, char c, t_all *all);
int							get_vars_length(char *str);
char						*get_var_value(char *str, t_env *env);
char						*get_path(char *cmd, t_env *env);
char						*get_input_redirection_file(char **args,
								t_all *all);
char						*extract_var_name(char *str, int *i);
void						get_output_redirection_file(char **args, t_all *all,
								t_cmd *node);
char						**get_herdoc_delimiter(char **args, t_all *all);
int							get_arr_len(char **arr);
int							ft_isalnum(int c);
char						*get_executable(char *cmd);
char						*handle_variables(char *str, t_env *env,
								t_all *all);
void						skip_spaces(char *cmd, int *i);
char						*ft_strtok(char *str);
bool						correct_path(char *path, char *cmd, bool is_path);
int							nums_of_chars(char *str, char c);
char						*get_str_in_quotes(char *command, int *i, char c,
								t_all *all);
char						*get_str_without_quotes(char *command, int *i);
bool						is_pipe_after(char *str);
int							create_cmd(t_all **all, char **args, int words,
								int is_pipe);
char						**allocate_args(char *command, int *pipe_idx,
								int *words, int i);
char						**ft_args_dup(char **args, t_all *all);
void						handle_var_as_cmd(char **result, char *arg, int *j,
								t_all *all);
char						*catch_arg(char *arg, t_all *all);
int							is_redirection(char *str, char *next);
int							count_valid_args(char **args);
char						*check_filename(char **args, int i, t_all *all);
char						*fix_cmd(char *cmd, t_all *all);
int							count_commands(t_cmd *cmd);
bool						is_correct_cmd(char *cmd, t_all *all);
int							reds_counter(char *cmd, t_all *all);
bool						is_incorrect_cmd(char *cmd, int *pipe, t_all *all);
int							find_len(char *str);
bool						full_of_spaces(char *buffer);
char						**n_env(t_all *all, char **env);
void						heredoc_sig_(int signo);
int							pwd_found(t_env *tmp);
void						set_sigs(void);
t_env						*new_env_(t_exp *exp);
t_env						*new_empty_env(t_all *all);
int							handle_exit(t_all *all);
void						wait_ps(pid_t *pids, t_all *all);
void						ignore_sigs(void);
void						exiting_execution_loop(t_vars *vars, t_all *all);
void						handle_sigs(int sig);
int							sh_atoi(char *s);
void						cd_error_exit(t_all *all);
void						unset_env_list(t_all *all, char *var);
void						heredoc_(t_cmd *doc, t_all *all);
void						add_it_to_env(t_all *all, char *new,
								t_exp *new_exp);
int							check_before_env(char *s);
void						parse_indetifier(t_all *all, char *str);
int							__unset_exp(t_all *all, t_exp *last, int ret,
								char *tmp_str);
void						executing_commands(t_all *all, char **envpp);
void						unset_exp_list(t_all *all, char *var);
void						mirroring_exp_and_env(t_all *all);
t_exp						*exp_getlast(t_exp *exp);
void						*shell_calloc(size_t size, int count);
void						ft_error(t_all *all, int child);
void						mirroring_env_and_exp(t_all *all);
void						heredoc_pipe(t_all *all);
void						redirect_in_out_to_pipe(int index, int pipe[],
								int *pr_fd, t_all *all);
void						redirections_set(t_all *all);
void						reset_signal_handlers(void);
void						signal_handler(int signo);
void						setup_signal_handlers(void);
void						env_exp_lists_clear(t_all *all);
void						free_env_list(t_all *all);
int							exec_built_ins(t_all *all);
void						set_lists(t_all *all, char **env);
void						exec_piped_built_ins(t_all *all);
void						heredoc_check(t_all *all);
t_exp						*new_exp_(t_env *env);
int							spliter_index(char *str);
t_exp						*exp_new(char *new_line);
void						exp_addback(t_exp *head, t_exp *new);
t_exp						*set_export_list(t_all *all);
void						ft_write(char *str, int fd);
void						change_dir(t_all *all, char *new_dir);
void						ft_pwd(t_all *all);
void						parse_indetifier(t_all *all, char *str);
void						unset_env(t_all *all);
int							unset_exp(t_all *all, t_exp *exp_, int ret);
void						env_addback(t_env *head, t_env *new);
char						*ft_strjoin(char *s1, char *s2);
t_env						*env_new(char *new_line);
t_env						*env_getlast(t_env *env);
t_env						*create_env_list(char **env);
void						ft_echo(char **str, int fd);
char						*heredoc(char *heredoc_str, t_all *all);
void						execution(t_all **all, char *envp[]);
int							match_word(char *neadle, char *str);
#endif