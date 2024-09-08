
#include "minishell.h"

void	free_e(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

char	**alloc_if_empty(t_all *all)
{
	char	**envp;
	char	cwd[1024];

	envp = (char **)malloc(sizeof(char *) * 4);
	if (!envp)
	{
		ft_lstclear(&all->cmd);
		exit(1);
	}
	getcwd(cwd, sizeof(cwd));
	envp[0] = ft_strjoin(ft_strdup("PWD="), cwd);
	envp[1] = ft_strjoin(ft_strdup("SHLV="), "1");
	envp[2] = ft_strjoin(ft_strdup("_="), "/usr/bin/env");
	envp[3] = NULL;
	return (envp);
}

char	**alloc_normal(t_all *all, char **env)
{
	char	**envp;
	int		i;

	i = 0;
	envp = n_env(all, env);
	while (env[i])
	{
		envp[i] = ft_strdup(env[i]);
		i++;
	}
	return (envp);
}

void	set_lists(t_all *all, char **env)
{
	char	**envp;
 /*
	set the inhereted envirement variables in two lists  (env/export)
	to manage it in out new shell process!
 */
	if (*env == NULL)
		{
			// set them manualy if no variables inhereted!
			envp = alloc_if_empty(all);
		}
	else
		envp = alloc_normal(all, env);
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
