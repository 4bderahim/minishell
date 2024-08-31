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

int unset_exp_from_beg(t_exp *exp, t_all *all)
{
	if (exp->prev == NULL)
	{
		if (exp->next != NULL)
		{
			all->exp = exp->next;
			free(exp->value);
			free(exp->variable);
			free(exp);
			return 1;
		}
	}
	return (0);
}
void	unset_exp_list(t_all *all, char *var)
{
	t_exp	*exp;

	exp = all->exp;
	while (exp != NULL)
	{
		if (match_word(var, exp->variable))
		{
			if (unset_exp_from_beg(exp , all))
				return ;
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

t_env	*new_env_(t_exp *exp)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->value = ft_strdup(exp->value);
	new->variable = ft_strdup(exp->variable);
	new->next = NULL;
	new->prev = NULL;
	return (new);
}
void	mirroring_exp_and_env(t_all *all)
{
	t_env	*env;
	t_exp	*exp;
	t_env	*env_last;
	t_exp	*exp_last;

	env = all->env;
	exp = all->exp;
	while (env != NULL && exp != NULL)
	{
		if (match_word(env->variable, exp->variable) & !match_word(env->value,
				exp->value))
			env->value = exp->value;
		exp = exp->next;
		env = env->next;
		if (env == NULL && exp != NULL)
		{
			env_last = env_getlast(all->env);
			env_last->next = new_env_(exp); 
			env_last->next->prev = env_last;
		}
	}
}

