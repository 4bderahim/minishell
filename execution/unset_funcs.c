/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


int	unset_exp(t_all *all, t_exp *exp_, int ret)
{
	t_exp	*exp;

	exp = all->exp;
	while (exp != NULL)
	{
		if (match_word(exp_->variable, exp->variable))
		{
			if (ret == -1)
				return (1);
			exp->value = ft_strdup(exp_->value);
			//if (exp_->value != NULL)
			mirroring_exp_and_env(all);
			return (1);
		}
		exp = exp->next;
	}
	return (0);
}
int unset_env_from_beg(t_env *env, t_all *all)
{
	if (env->prev == NULL)
	{
		if (env->next != NULL)
		{
			all->env = env->next;

			free(env->value);
			free(env->variable);
			free(env);
			return 1;
		}
	}
	return (0);
}
void	unset_env_list(t_all *all, char *var)
{
	t_env	*env;

	env = all->env;
	while (env != NULL)
	{
		if (match_word(var, env->variable))
		{
			if (unset_env_from_beg(env , all))
				return ;
			else if (env->next == NULL)
				env->prev->next = NULL;
			else
			{
				
				env->prev->next = env->next;
				env->next->prev = env->prev;
			}
			free(env);
			break ;
		}
		env = env->next;
	}
}
int exp_and_env_unset(t_env *env, t_exp *exp, t_all *all, int i)
{
	int break_;

	break_ = 0;
	if (env && match_word(all->cmd->args[i], env->variable) )
	{
		
		unset_env_list(all, env->variable);
		break_ = 1;
	}
	if (exp && match_word(all->cmd->args[i], exp->variable))
	{
		unset_exp_list(all, all->cmd->args[i]);
		break_ = 1;
	}
	return (break_);
}
void	unset_env(t_all *all)
{
	t_env	*env;
	t_exp	*exp;
	int		i;

	i = 1;
	while (all->cmd->args[i])
	{
		env = all->env;
		exp = all->exp;
		while (env != NULL || exp != NULL)
		{
			if (exp_and_env_unset(env, exp, all, i))
				break;
			if (env != NULL)
				env = env->next;
			if (exp != NULL)
				exp = exp->next;

		}
		i++;
	}
}