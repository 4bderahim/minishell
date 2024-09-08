/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/09/07 19:34:43 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_ps(pid_t *pids, t_all *all)
{
	int	i;
	int	status;

	// wait child every process and check its exit status
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
	if (g_signaled)
		all->exit_status = 1;
}

void	close_pipe_sides(int pipe_sides[2])
{
	close(pipe_sides[1]);
	close(pipe_sides[0]);
}

int	execution_loop(t_vars *vars, int i, t_all *all)
{
	int	pipe_sides[2];
	/*
	because execve()  will replace the current process memory space
	with a copy of the executable file opened by it,   
	each command (other than built ins) will be executed using execve()
	inside a child process , this will happen after redirect input/output depending on:
	pipe ,(write/read) from regular files or heredoc
	*/
	if (pipe(pipe_sides) < 0)
		ft_error(all, 0);
	vars->pids[i] = fork();
	if (vars->pids[i] < 0)
		return (ft_error(all, 0), 0);
	set_sigs();
	if (vars->pids[i] == 0)
	{
		// set signals to default in child process 
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(pipe_sides[0]);
		redirect_in_out_to_pipe(i, pipe_sides, &vars->pr_fd, all);
		executing_commands(all, vars->envpp);
	}
	if (i != 0)
		close(vars->pr_fd);
	// store last pipe output fd to use it in next command as stdin
	vars->pr_fd = dup(pipe_sides[0]);
	if (vars->pr_fd < 0)
		ft_error(all, 0);
	close_pipe_sides(pipe_sides);
	return (1);
}

t_vars	*set_envp_pids(t_all *all, char **env)
{
	t_vars	*vars;

	vars = (t_vars *)malloc(sizeof(t_vars));
	if (!vars)
		ft_error(all, 0);
	vars->envpp = env;
	vars->pids = (pid_t *)malloc(sizeof(pid_t) * all->nums_of_cmds);
	if (!vars->pids)
		ft_error(all, 0);
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
	/*
 	set heredoc signale handler for SIGINT (exit when signaled)
  */
	signal(SIGINT, heredoc_sig_);
	heredoc_check(all);
	/* 
	 check if there's only one command and it is a built-in one !
	 exit executing id thats the case!
	*/
	if (!all->pipes_num && exec_built_ins(all))
		return ;
	/*
		ignore SIGINT in parent process 
		because otherwise both child and parent processes will receive the same signal 
		and handle it and both return new prompt!
		but we want to (exit in child process normally) by defaulting signal actions in child process
	*/
	signal(SIGINT, SIG_IGN);
	while (i < all->nums_of_cmds)
	{
		if (!execution_loop(vars, i, all))
			break ;
		i++;
		all->cmd = all->cmd->next;
	}
	exiting_execution_loop(vars, all);
	// reset signals handler in parent
	setup_signal_handlers();
	(*alll)->cmd = cmd_;
}
