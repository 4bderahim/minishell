
#include "minishell.h"

void	heredoc_child(int *_pipe, t_cmd *cmd, t_all *all)
{
	signal(SIGINT, SIG_DFL);// defau
	close(_pipe[0]);
	heredoc_(cmd, all);
	write(_pipe[1], cmd->heredoc_content, ft_strlen(cmd->heredoc_content));
	close(_pipe[1]);
	exit(0);
}

void	read_heredoc_input(int *pipefd, t_cmd *cmd, t_all *all)
{
	char	*buffer;
	int		read_ret;

	read_ret = 1;
	while (read_ret > 0)
	{
		buffer = (char *)shell_calloc(sizeof(char), 1001);
		if (!buffer)
			ft_error(all, 1);
		buffer[1000] = 0;
		read_ret = read(pipefd[0], buffer, 1000);
		cmd->heredoc_content = ft_strjoin(cmd->heredoc_content, buffer);
		free(buffer);
	}
}

void	heredoc_ing(t_cmd *cmd, t_all *all)
{
	int		read_ret;
	pid_t	pid;
	int		pipefd[2];
	int		status;
	/*

	each heredoc command will be handled in a child process 
	then the content from stdin will be written to a pipe.
	*/
	read_ret = 1;
	if (pipe(pipefd) == -1)
		ft_error(all, 1);
	pid = fork();
	if (pid < 0)
		ft_error(all, 1);
	if (pid == 0)
		{
			heredoc_child(pipefd, cmd, all);
		}
	else
	{
		close(pipefd[1]);
		read_heredoc_input(pipefd, cmd, all);// read the content from the pipe in the parent process
		close(pipefd[0]);
	}
	waitpid(pid, &status, 0);
}

void	heredoc_check(t_all *all)
{
	t_cmd	*doc;

	doc = all->cmd;
	g_signaled = 0;
	/*
	if we have a command line with pipes the loop bellow checks every command for heredoc to store its upcoming content
	all this will break once a SIGINT signal arrived which will set the global variable (g_signaled) in the heredoc signal handler.
	*/
	while (doc != NULL && !g_signaled)
	{
		if (doc->heredoc_delimiter != NULL)
			heredoc_ing(doc, all);
		doc = doc->next;
	}
	all->exit_status = g_signaled;
}
