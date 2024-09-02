#include "minishell.h"

size_t count_commands(t_cmd *cmd)
{
  size_t counter;

  counter = 0;
  while (cmd)
  {
    cmd = cmd->next;
    counter++;
  }
  return counter;
}

void check_leaks()
{
  system("leaks -q minishell");
}

void skip_spaces(char *cmd, int *i)
{
  while (cmd[*i] && ft_isspace(cmd[*i]))
    *i += 1;
}

bool is_symbol_in_cmd(char c)
{
  if(c == SLASH || c == BACK_SLASH)
    return true;
  return false;
}

bool is_correct_cmd(char *cmd, t_all *all)
{
  int i;
  int len;

  i = 0;
  skip_spaces(cmd, &i);
  if(cmd[i] == PIPE)
    return (throw_error("syntax error near unexpected token", all, 258), false);
  while(i < ft_strlen(cmd))
  {
    skip_spaces(cmd, &i);
    if(cmd[i] == SINGLE_QUOTE || cmd[i] == DOUBLE_QUOTE)
    {
      i++;
      len = ft_strchr(cmd + i, cmd[i - 1]);
      if(len == -1)
        return throw_error("single quote must be closed", all, 1), 0;
      skip_str_inside_quote(cmd, &i, cmd[i - 1]);
    }
    else if(cmd[i] == PIPE)
    {
      if(cmd[++i] && cmd[i] == PIPE)
        i++;
      skip_spaces(cmd, &i);
      if(is_symbol_in_cmd(cmd[i]) || cmd[i] == PIPE)
        return (throw_error("syntax error near unexpected token", all, 258), false);
    }
    i++;
  }
  return true;
}

void shell_init(t_all *all, char **env)
{
  using_history();
  set_lists(all, env);
  setup_signal_handlers();
  all->exit_status = 0;
}

int start_shell(char *read, t_all **all, char **env)
{
  add_history(read);
  if(!is_correct_cmd(read, *all))
  {
    free(read);
    read = NULL;
    return 0;
  }
  read = fix_cmd(read, *all);
  if((*all)->error || !ft_lexer(read, all) || !(*all)->cmd)
  {
    free(read);
    read = NULL;
    return 0;
  }
  (*all)->nums_of_cmds = count_commands((*all)->cmd);
  if(!(*all)->error)
    execution(all, env);
  free((*all)->_vars->pids);
  free((*all)->_vars);
  free(read);
  read = NULL;
  ft_lstclear(&(*all)->cmd);
  return 1;
}

int main(int ac, char **av, char **env)
{
  t_all *all;

  all = malloc(sizeof(t_all));
  all->cmd = NULL;
  shell_init(all, env);
  // atexit(check_leaks);
  all->exit_status = 0;
  while(1)
  {
    all->error = false;
    char *read = readline("minishell > ");
    if(!read)
    {
      env_exp_lists_clear(all);
     // check_leaks();
      exit(0);
    }
    if (ft_strlen(read))
    {
      if(!start_shell(read, &all, env))
      {
       // check_leaks();
        continue;
      }
    }
    else
      free(read);
    //check_leaks();
  }
   //check_leaks();
  return 0;
}
