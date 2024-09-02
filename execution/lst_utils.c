/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**n_env(t_all *all, char **env)
{
	int		i;
	char	**envp;

	i = 0;
	while (env[i])
		i++;
	envp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!envp)
	{
		ft_lstclear(&all->cmd);
		exit(1);
	}
	envp[i] = NULL;
	return (envp);
}

void	free_e(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

void	set_lists(t_all *all, char **env)
{
	int		i;
	char	**envp;

	i = 0;
	envp = n_env(all, env);
	while (env[i])
	{
		envp[i] = ft_strdup(env[i]);
		i++;
	}
	all->env = create_env_list(envp);
	if (all->env == NULL)
	{
		ft_lstclear(&all->cmd);
		exit(1);
	}
	all->exp = set_export_list(all);
	if (all->exp == NULL)
	{
		free_env_list(all);
		ft_lstclear(&all->cmd);
		exit(1);
	}
	free_e(envp);
}

void	*shell_calloc(size_t size, int count)
{
	unsigned char	*ret;
	int				i;

	i = size * count;
	ret = (unsigned char *)malloc(i);
	if (!ret)
		return (NULL);
	i--;
	while (i >= 0)
	{
		ret[i] = 0;
		i--;
	}
	return ((void *)ret);
}
