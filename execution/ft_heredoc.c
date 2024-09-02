/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_child(int *_pipe, t_cmd *cmd, t_all *all)
{
	signal(SIGINT, SIG_DFL);
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
			ft_error(all);
		buffer[1000] = 0;
		read_ret = read(pipefd[0], buffer, 1000);
		cmd->heredoc_content = ft_strjoin(cmd->heredoc_content, buffer);
		free(buffer);
	}
}

void	heredoc_ing(t_cmd *cmd, t_all *all)
{
	char	*buffer;
	int		read_ret;
	pid_t	pid;
	int		pipefd[2];

	read_ret = 1;
	if (pipe(pipefd) == -1)
		ft_error(all);
	pid = fork();
	if (pid < 0)
		ft_error(all);
	if (pid == 0)
		heredoc_child(pipefd, cmd, all);
	else
	{
		close(pipefd[1]);
		read_heredoc_input(pipefd, cmd, all);
		close(pipefd[0]);
	}
	waitpid(pid, NULL, 0);
}

void	heredoc_check(t_all *all)
{
	t_cmd	*doc;

	doc = all->cmd;
	while (doc != NULL)
	{
		if (doc->heredoc_delimiter != NULL)
			heredoc_ing(doc, all);
		doc = doc->next;
	}
}
