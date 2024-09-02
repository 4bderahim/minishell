/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/27 15:54:27 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_var_and_unset_env(t_all *all, t_env *env, char *var, t_env *env_)
{
	if (env && match_word(var, env_->variable))
	{
		unset_env_list(all, env_->variable);
		return (1);
	}
	return (0);
}

int	check_var_and_unset_exp(t_all *all, t_exp *exp, char *var, t_exp *exp_)
{
	if (exp && match_word(var, exp_->variable))
	{
		unset_exp_list(all, exp_->variable);
		return (1);
	}
	return (0);
}

int	exp_and_env_unset(t_env *env, t_exp *exp, t_all *all, int i)
{
	int		break_;
	t_exp	*exp_;
	t_env	*env_;

	break_ = 0;
	exp_ = all->exp;
	env_ = all->env;
	while (env_)
	{
		break_ = check_var_and_unset_env(all, env, all->cmd->args[i], env_);
		if (break_)
			break ;
		env_ = env_->next;
	}
	exp_ = all->exp;
	while (exp_)
	{
		break_ = check_var_and_unset_exp(all, exp, all->cmd->args[i], exp_);
		if (break_)
			break ;
		exp_ = exp_->next;
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
				break ;
			if (env != NULL)
				env = env->next;
			if (exp != NULL)
				exp = exp->next;
		}
		i++;
	}
}
