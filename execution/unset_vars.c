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
// unset SECURITYSESSIONID USER MallocNanoZone  COMMAND_MODE  PATH SSH_AUTH_SOCK LOGNAME ORIGINAL_XDG_CURRENT_DESKTOP SHLVL PWD OLDPWD LIBRARY_PATH HOMEBREW_CACHE TERM_PROGRAM TERM_PROGRAM_VERSION LANG COLORTERM GIT_ASKPASS VSCODE_GIT_ASKPASS_NODE VSCODE_GIT_ASKPASS_EXTRA_ARGS VSCODE_GIT_ASKPASS_MAIN VSCODE_GIT_IPC_HANDLE VSCODE_INJECTION VSCODE_INJECTION ZDOTDIR USER_ZDOTDIR TERM HOME SHELL LaunchInstanceID __CF_USER_TEXT_ENCODING XPC_SERVICE_NAME XPC_FLAGS TMPDIR HOMEBREW_TEMP
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
		ft_error(all);
	env_new->variable = NULL;
	env_new->value = NULL;
	env_new->prev = NULL;
	env_new->next = NULL;
	return (env_new);
}

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

void	mirroring_exp_and_env(t_all *all)
{
	t_env	*env;
	t_exp	*exp;
	t_env	*env_last;
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
		{
			env_last = env_getlast(all->env);
			env_last->next = new_env_(exp);
			env_last->next->prev = env_last;
		}
	}
}
