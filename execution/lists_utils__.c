/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists_utils__.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/09/03 22:24:08 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	alloc_new_env_node(t_all *all)
{
	t_env	*env_;
	t_env	*env_tmp;
	t_env	*env;
	t_exp	*exp;

	exp = all->exp;
	env_ = all->env;
	while (exp)
	{
		env = env_;
		env_ = new_empty_env(all);
		env_->variable = ft_strdup(exp->variable);
		env_->value = ft_strdup(exp->value);
		env_->prev = env;
		if (env_->prev == NULL)
			env = env_;
		exp = exp->next;
		if (exp && env_)
			env_ = env_->next;
	}
	all->env = env;
}

void	add_to_env_end(t_all *all, t_exp *exp)
{
	t_env	*env_last;

	env_last = env_getlast(all->env);
	env_last->next = new_env_(exp);
	env_last->next->prev = env_last;
}

void	mirroring_exp_and_env(t_all *all)
{
	t_env	*env;
	t_exp	*exp;
	char	*prv;

	env = all->env;
	exp = all->exp;
	if (env == NULL && exp != NULL)
		alloc_new_env_node(all);
	while (env != NULL && exp != NULL)
	{
		if (match_word(env->variable, exp->variable) & !match_word(env->value,
				exp->value))
		{
			prv = env->value;
			env->value = ft_strdup(exp->value);
			free(prv);
		}
		exp = exp->next;
		env = env->next;
		if (env == NULL && exp != NULL)
			add_to_env_end(all, exp);
	}
}
