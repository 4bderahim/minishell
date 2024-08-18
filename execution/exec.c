/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
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

void free_e(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
		free(envp[i++])
		;
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

void	wait_ps(pid_t *pids, t_all *all)
{
	int	i;
	int	status;

	i = 0;
	while (i < all->nums_of_cmds)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
}
void close_pipe_both_sides(int *sides)
{
	close(sides[1]);
	close(sides[0]);
}
void	execution(t_all **alll, char *envpp[])
{
	t_all	*all;
	t_cmd	*cmd_;
	int		i;
	int		pipe_sides[2];
	int		pr_fd;

	all = *alll;
	i = 0;
	cmd_ = all->cmd;
	pid_t	pids[all->nums_of_cmds];
	heredoc_check(all);
	if (exec_built_ins(all))
	{
		all->nums_of_cmds--;
		all->cmd = all->cmd->next;
	}
	while (i < all->nums_of_cmds)
	{
		if (pipe(pipe_sides) < 0)
			ft_error(all);
		pids[i] = fork();
		if (pids[i] < 0)
			ft_error(all);
		if (pids[i] == 0)
		{
			reset_signal_handlers();
			redirect_in_out_to_pipe(all->nums_of_cmds, i, pipe_sides, &pr_fd, all);
			redirections_set(all);
			heredoc_pipe(all);
			exec_piped_built_ins(all, pipe_sides);
			if (execve(all->cmd->full_path, all->cmd->args, envpp) == -1)
				ft_write(strerror(errno), 2);
			exit(1);
		}
		if (i != 0)
			close(pr_fd);
		pr_fd = dup(pipe_sides[0]);
		if (pr_fd < 0)
			ft_error(all);
		close_pipe_both_sides(pipe_sides);
		i++;
		all->cmd = all->cmd->next;
	}
	// if(valid == 1)
	// 	close(pr_fd);
	wait_ps(pids, all);
	all = *alll;
	all->cmd = cmd_;
	
}

