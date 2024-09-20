
#include "minishell.h"

void	redirections_set(t_all *all)
{
	int	fd;

	if (all->cmd->in_file)
	{
		fd = open(all->cmd->in_file, O_RDONLY);
		if (fd == -1)
			ft_error(all, 1);
		if (dup2(fd, STDIN_FILENO) < 0)
			ft_error(all, 1);
		close(fd);
	}
	if (all->cmd->out_file || all->cmd->append_file)
	{
		if (all->cmd->append_file)
			fd = open(all->cmd->append_file, O_CREAT | O_RDWR | O_APPEND);
		else
			fd = open(all->cmd->out_file, O_CREAT | O_WRONLY | O_TRUNC);
		if (fd == -1)
			ft_error(all, 1);
		if (dup2(fd, STDOUT_FILENO) < 0)
			ft_error(all, 1);
		close(fd);
	}
}

void	exit__(void)
{
	if (g_signaled)
		exit(g_signaled);
	exit(0);
}

void	heredoc_pipe(t_all *all)
{
	int	fd;
	int	p[2];
	/*
	if there's a heredoc in command line 
	i write the content already read to a pipe an duplicate its input end to stdin
	*/
	if (all->cmd->heredoc_delimiter == NULL)
		return ;
	if (pipe(p) > 0)
		ft_error(all, 1);
	fd = fork();
	if (fd == -1)
		ft_error(all, 1);
	if (fd == 0)
	{
		close(p[0]);
		redirections_set(all);
		ft_write(all->cmd->heredoc_content, p[1]);
		close(p[1]);
		exit(1);
	}
	close(p[1]);
	if (dup2(p[0], STDIN_FILENO) < 0)
		ft_error(all, 1);
	close(p[0]);
	if (all->cmd->cmd == NULL)
		exit__();
}

void	redirect_in_out_to_pipe(int index, int pipe[], int *pr_fd, t_all *all)
{
	/*
	using dup2 to duplicat the STDIN_FILENO/STDIN_FILENO to the pipe file descriptors
	only if there is a pipe in the command line!
	*/

	if (index != 0)
	{
		if (dup2(*pr_fd, STDIN_FILENO) < 0)
			ft_error(all, 1);
		close(*pr_fd);
	}
	if (index < all->nums_of_cmds - 1)
	{
		if (dup2(pipe[1], STDOUT_FILENO) < 0)
			ft_error(all, 1);
		close(pipe[1]);
	}
}
