/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists_utils__.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/09/06 22:15:05 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	alloc_new_env_node(t_all *all)
{
	t_env	*env_;
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

int	pwd_found(t_env *tmp)
{
	if (ft_strlen(tmp->variable) > 2 && tmp->variable[0] == 'P'
		&& tmp->variable[1] == 'W' && tmp->variable[2] == 'D')
		return (1);
	return (0);
}

void	add_to_env_end(t_all *all, t_exp *exp)
{
	t_env	*env_last;

	env_last = env_getlast(all->env);
	env_last->next = new_env_(exp);
	env_last->next->prev = env_last;
}

int	matched__(t_all *all, t_exp *exp)
{
	t_env	*env;

	env = env_getlast(all->env);
	while (exp != NULL)
	{
		if (match_word(exp->variable, env->variable) && match_word(exp->value,
				env->value))
			return (1);
		exp = exp->next;
	}
	return (0);
}

t_env	*set_env_list_(t_all *all)
{
	t_env	*env;
	t_exp	*exp;
	t_env	*new;

	exp = all->exp;
	env = new_env_(exp);
	exp = exp->next;
	while (exp != NULL)
	{
		new = new_env_(exp);
		if (new == NULL)
			return (NULL);
		env_addback(env, new);
		exp = exp->next;
	}
	return (env);
}

void	mirroring_exp_and_env(t_all *all)
{
	free_env_list(all);
	all->env = set_env_list_(all);
}
