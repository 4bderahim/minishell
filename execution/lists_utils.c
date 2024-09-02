/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/30 11:09:54 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exp	*new_exp_(t_env *env)
{
	t_exp	*new;

	new = (t_exp *)malloc(sizeof(t_exp));
	if (!new)
		return (NULL);
	new->value = ft_strdup(env->value);
	new->variable = ft_strdup(env->variable);
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_exp	*set_export_list(t_all *all)
{
	t_env	*env;
	t_exp	*exp;
	t_exp	*new;

	env = all->env;
	exp = new_exp_(env);
	env = env->next;
	while (env != NULL)
	{
		new = new_exp_(env);
		if (new == NULL)
			return (NULL);
		exp_addback(exp, new);
		env = env->next;
	}
	return (exp);
}
