
#include "minishell.h"

void	mirroring_env_and_exp(t_all *all)
{
	t_env	*env;
	t_exp	*exp;
	char	*prv;

	env = all->env;
	exp = all->exp;
	while (env != NULL && exp != NULL)
	{
		if (match_word(env->variable, exp->variable) & !match_word(env->value,
				exp->value))
		{
			prv = exp->value;
			exp->value = ft_strdup(env->value);
			free(prv);
		}
		exp = exp->next;
		env = env->next;
	}
}

void	set_old_pwd(t_all *all, char *old_dir)
{
	t_env	*a;
	char	*tmp_free;

	a = all->env;
	while (a != NULL)
	{
		if (match_word("OLDPWD", a->variable))
		{
			tmp_free = a->value;
			a->value = ft_strdup(old_dir);
			free(tmp_free);
		}
		a = a->next;
	}
}

void	add_to_env(t_all *all, char *new_dir)
{
	t_env	*tmp;
	char	*tmp_free;

	tmp = all->env;
	while (tmp != NULL)
	{
		if (pwd_found(tmp))
		{
			if (tmp->next != NULL)
			{
				set_old_pwd(all, tmp->value);
				tmp_free = tmp->value;
				tmp->value = ft_strdup(new_dir);
				free(tmp_free);
				break ;
			}
			unset_env_list(all, "PWD");
			env_addback(all->env, env_new(ft_strjoin(ft_strdup("PWD="),
						new_dir)));
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
	char	buff[1024];
	DIR		*dir;

	if (new_dir == NULL)
	{
		new_dir = get_home_wd(all);
		if (!new_dir)
			return (ft_write("minishell: cd: HOME not set\n", 2));
	}
	dir = opendir(new_dir);
	if (dir == NULL)
	{
		if (errno == ENOENT || errno == ENOTDIR)
			cd_error_exit(all);
		return ;
	}
	close(dir->__dd_fd);
	free(dir->__dd_buf);
	free(dir);
	if (all->cmd->pipe)
		return ;
	if (chdir(new_dir) < 0)
		return (cd_error_exit(all));
	add_to_env(all, getcwd(buff, 1024));
}
