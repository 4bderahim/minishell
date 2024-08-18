/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/18 13:16:19 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_env_list(t_all *all)
{
	t_env	*tmp;

	while (all->env)
	{
		tmp = all->env;
		all->env = all->env->next;
		free(tmp->value);
		free(tmp->variable);
		free(tmp);
	}
}

void free_exp_list(t_all *all)
{
	t_exp	*tmp;

	while (all->exp)
	{
		tmp = all->exp;
		all->exp = all->exp->next;
		free(tmp);
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
void	exit_way(t_all *all)
{
    env_exp_lists_clear(all);
		free(all);
    exit(1);
}
void	ft_error(t_all *all)
{
	ft_write(strerror(errno), 2);
	write(2, "\n", 1);
	exit_way(all);
}
