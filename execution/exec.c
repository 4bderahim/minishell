/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/30 11:09:54 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_ps(pid_t *pids, t_all *all)
{
	int	i;
	int	status;

	i = 0;
	while (i < all->nums_of_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			all->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			all->exit_status = WTERMSIG(status) + 128;
		i++;
	}
}

void	close_pipe_sides(int pipe_sides[2])
{
	close(pipe_sides[1]);
	close(pipe_sides[0]);
}

void	execution_loop(t_vars *vars, int i, t_all *all, t_cmd *cmd)
{
	int	pipe_sides[2];
	int	pr_fd;

	if (pipe(pipe_sides) < 0)
		ft_error(all);
	vars->pids[i] = fork();
	if (vars->pids[i] < 0)
		ft_error(all);
	signal(SIGINT, handle_sigs);
	signal(SIGQUIT, handle_sigs);
	if (vars->pids[i] == 0)
	{
		close(pipe_sides[0]);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		redirect_in_out_to_pipe(i, pipe_sides, &pr_fd, all);
		executing_commands(all, pipe_sides, vars->envpp);
	}
	if (i != 0)
		close(pr_fd);
	pr_fd = dup(pipe_sides[0]);
	if (pr_fd < 0)
		ft_error(all);
	vars->pr_fd = pr_fd;
	close_pipe_sides(pipe_sides);
}

t_vars	*set_envp_pids(t_all *all, char **env)
{
	t_vars	*vars;

	vars = (t_vars *)malloc(sizeof(t_vars));
	if (!vars)
		ft_error(all);
	vars->envpp = env;
	vars->pids = (pid_t *)malloc(sizeof(pid_t) * all->nums_of_cmds);
	if (!vars->pids)
		ft_error(all);
	all->_vars = vars;
	return (vars);
}

void	execution(t_all **alll, char *envpp[])
{
	t_all	*all;
	t_cmd	*cmd_;
	int		i;
	t_vars	*vars;

	all = *alll;
	i = 0;
	cmd_ = all->cmd;
	vars = set_envp_pids(all, envpp);
	ignore_sigs();
	heredoc_check(all);
	if (!all->pipes_num)
		exec_built_ins(all);
	while (i < all->nums_of_cmds)
	{
		execution_loop(vars, i, all, all->cmd);
		i++;
		all->cmd = all->cmd->next;
	}
	exiting_execution_loop(vars, all);
	close(all->_vars->pr_fd);
	(*alll)->cmd = cmd_;
}
