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

int	unset_exp_from_beg(t_exp *exp, t_all *all)
{
	t_exp	*tmp;

	if (exp->prev == NULL)
	{
		if (exp->next != NULL)
		{
			tmp = all->exp;
			all->exp = exp->next;
			all->exp->prev = NULL;
			free(tmp->value);
			free(tmp->variable);
			free(tmp);
			return (1);
		}
	}
	return (0);
}

void	free_last_var_exp(t_exp *exp)
{
	free(exp->value);
	free(exp->variable);
	free(exp);
}

void	unset_exp_list(t_all *all, char *var)
{
	t_exp	*exp;

	exp = all->exp;
	while (exp != NULL)
	{
		if (match_word(var, exp->variable))
		{
			if (unset_exp_from_beg(exp, all))
				return ;
			else if (exp->next == NULL)
			{
				if (exp->prev != NULL)
					exp->prev->next = NULL;
				else
					all->exp = NULL;
				free_last_var_exp(exp);
				break ;
			}
			exp->prev->next = exp->next;
			exp->next->prev = exp->prev;
			free_last_var_exp(exp);
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

t_env	*new_empty_env(t_all *all)
{
	t_env	*env_new;

	env_new = (t_env *)malloc(sizeof(t_env));
	if (!env_new)
		ft_error(all, 0);
	env_new->variable = NULL;
	env_new->value = NULL;
	env_new->prev = NULL;
	env_new->next = NULL;
	return (env_new);
}
