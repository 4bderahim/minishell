#include "minishell.h"

void exit_way(t_all *all)
{
    env_exp_lists_clear(all);
    exit(1);
}

void free_env_list(t_all *all)
{

	while (all->env)
	{
		free(all->env);
		all->env = all->env->next;
	}

}
void free_exp_list(t_all *all)
{

	while (all->exp)
	{
		free(all->exp);
		all->exp = all->exp->next;
	}

}
void	env_exp_lists_clear(t_all *all)
{
	if (all->env)
		free_env_list(all);
	if (all->exp)
		free_exp_list(all);
	ft_lstclear(&all->cmd);
}