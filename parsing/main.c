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



// void check_leaks()
// {
//   system("leaks -q minishell");
// }

int main(int ac, char **av, char **env)
{
  t_all *all;

  all = malloc(sizeof(t_all));
  all->cmd = NULL;
  // all->nums_of_cmds = 1;
  // ft_init(shell);
  using_history();
  set_lists(all, env); // 
  // setup_signal_handlers();
  // atexit(check_leaks);
  while(1)
  {
    char *read = readline("minishell > ");
    if(!read)
    {
        env_exp_lists_clear(all);
        exit(0);
    }
    if (!ft_strlen(read))
      continue;
    add_history(read);
    if(!ft_lexer(read, &all))
    {
      // system("leaks -q minishell");
      // free(read);
      continue;
    }

    // free(read);
    // print_list(all->cmd);
    all->nums_of_cmds = count_commands(all->cmd);
    execution(&all, env);
    ft_lstclear(&all->cmd);
		// free_env_list(all);
    // env_exp_lists_clear(all);
    // system("leaks -q minishell");
  }
  return 0;
}
