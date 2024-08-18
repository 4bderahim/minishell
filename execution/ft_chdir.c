/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_chdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mirroring_env_and_exp(t_all *all)
{
	t_env	*env;
	t_exp	*exp;

	env = all->env;
	exp = all->exp;
			

	while (env != NULL && exp != NULL)
	{
		if (match_word(env->variable, exp->variable) & !match_word(env->value, exp->value))
			{
				exp->value = env->value;
			}
		exp = exp->next;
		env = env->next;
	}
}
void	set_old_pwd(t_all *all, char *old_dir)
{
	t_env	*a;

	a = all->env;
	while (a != NULL)
	{
		if (match_word("OLDPWD", a->variable))
			a->value = ft_strdup(old_dir);
		a = a->next;
	}
}
void	add_to_env(t_all *all, char *new_dir)
{
	t_env	*tmp;
	t_env	*tmp2;
	t_all	*al;

	tmp = all->env;
	//more checks here for SEGV
	while (tmp != NULL)
	{
		if (ft_strlen(tmp->variable) > 2 &&
			tmp->variable[0] == 'P' && tmp->variable[1] == 'W'
				&& tmp->variable[2] == 'D')
		{
			if (tmp->next != NULL)
			{
				set_old_pwd(all, tmp->value);
				tmp->value = ft_strdup(new_dir);
				break ;
			}
			al->env->prev->next = NULL;
			env_addback(al->env, env_new(new_dir));
			break ;
		}
		tmp = tmp->next;
	}
	mirroring_env_and_exp(all);
}
char	*get_home_wd(t_all *all)
{
	t_env	*tmp;

	tmp = all->env;
	while (tmp)
	{
		if (match_word("HOME", tmp->variable))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
void	change_dir(t_all *all, char *new_dir)
{
	char buff[1024];
	if (new_dir == NULL)
		new_dir = get_home_wd(all);
	if (chdir(new_dir) < 0)
	{
		ft_write("cd: ", 2);
		ft_write(new_dir, 2);
		ft_write(": ", 2);
		ft_write(strerror(errno), 2);
		ft_write("\n", 1);

		return ;
	}
	add_to_env(all, getcwd(buff, 1024));
}