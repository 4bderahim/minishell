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
			mirroring_exp_and_env(all);
			free(exp_->value);
			free(exp_->variable);
			free(exp_);
			return (1);
		}
		exp = exp->next;
	}
	return (0);
}

int	unset_env_from_beg(t_env *env, t_all *all)
{
	t_env	*tmp;

	if (env->prev == NULL)
	{
		if (env->next != NULL)
		{
			tmp = all->env;
			all->env = env->next;
			all->env->prev = NULL;
			free(tmp->value);
			free(tmp->variable);
			free(tmp);
			return (1);
		}
	}
	return (0);
}

void	cd_error_exit(t_all *all)
{
	ft_write("minishell: ", 2);
	ft_write(strerror(errno), 2);
	ft_write("\n", 1);
	all->exit_status = 1;
	return ;
}

void	free_last_var_env(t_env *env)
{
	free(env->value);
	free(env->variable);
	free(env);
}

void	unset_env_list(t_all *all, char *var)
{
	t_env	*env;

	env = all->env;
	while (env != NULL)
	{
		if (match_word(var, env->variable))
		{
			if (unset_env_from_beg(env, all))
				return ;
			else if (env->next == NULL)
			{
				if (env->prev != NULL)
					env->prev->next = NULL;
				else
					all->env = NULL;
				free_last_var_env(env);
				break ;
			}
			env->prev->next = env->next;
			env->next->prev = env->prev;
			free_last_var_env(env);
			break ;
		}
		env = env->next;
	}
}
