/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_exp_list(t_all *all, char *var)
{
	t_exp	*exp;

	exp = all->exp;
	while (exp != NULL)
	{
		if (match_word(var, exp->variable))
		{
			if (exp->prev == NULL)
			{
				if (exp->next != NULL)
					all->exp = exp;
			}
			else if (exp->next == NULL)
				exp->prev->next = NULL;
			else
			{
				exp->prev->next = exp->next;
				exp->next->prev = exp->prev;
			}
			free(exp);
			break ;
		}
		exp = exp->next;
	}
}

void	mirroring_exp_and_env(t_all *all)
{
	t_env	*env;
	t_exp	*exp;

	env = all->env;
	exp = all->exp;
	while (env != NULL && exp != NULL)
	{
		if (match_word(env->variable, exp->variable) & !match_word(env->value,
				exp->value))
			env->value = exp->value;
		exp = exp->next;
		env = env->next;
	}
}
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

void	unset_env_list(t_all *all, char *var)
{
	t_env	*env;

	env = all->env;
	while (env != NULL)
	{
		if (match_word(var, env->variable))
		{
			if (env->prev == NULL)
			{
				if (env->next != NULL)
					all->env = env;
			}
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
void	unset_env(t_all *all)
{
	t_env	*env;
	int		i;

	i = 1;
	while (all->cmd->args[i])
	{
		env = all->env;
		while (env != NULL)
		{
			if (match_word(all->cmd->args[i], env->variable))
			{
				unset_env_list(all, env->variable);
				unset_exp_list(all, all->cmd->args[i]);
				break ;
			}
			env = env->next;
		}
		i++;
	}
}
