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

int	match_word(char *neadle, char *str)
{
	int	i;

	i = 0;
	if (!str || !neadle)
		return (0);
	if (!(*str) || !(*neadle))
		return (0);
	while (str[i])
	{
		if (str[i] != neadle[i])
			return (0);
		i++;
	}
	if (neadle[i] != 0)
		return (0);
	return (1);
}
int no_expand(char *s)
{
	int i;
	i = 0 ;

	while (s[i])
	{
		if (s[i] == '$' && (s[i+1] != 0 || s[i+1] != ' '))
			return (0);
		i++;
	}
	return (1);
	
}

char *fill_full_str(char *full_str, char *input, t_all *all)
{
	full_str = ft_strjoin(full_str, input);
	if (full_str == NULL)
		ft_error(all);
	//if (!ft_strlen(full_str))//|| no_expand(full_str))
	full_str = ft_strjoin(full_str, "\n");
	// else
	// 	full_str = ft_strjoin(
	// 	handle_variables(full_str, all->env, get_vars_length(full_str), all), "\n");
	free(input);
	return (full_str);
}
char	*heredoc(char *heredoc_str, int fd, t_all *all)
{
	char	*full_str;
	char	*input;

	// dont expand the herdoc delimeter
	full_str = (char *)malloc(1);
	if (!full_str)
		ft_error(all);
	full_str[0] = 0;
	while (1)
	{
		input = readline(">>");
		if (!input)
			exit(0);
		if (!match_word(input, heredoc_str))
			full_str = fill_full_str(full_str, input, all);
		else
			{
				free(input);
				break ;
			}
	}
	return (full_str);
}

void	heredoc_(t_cmd *doc, t_all *all)
{
	char	*here_tmp;
	int		i;
	int		last_hrdoc;
	char	*here_tmp2;


	doc->heredoc_content = ft_strdup("");
	i = 0;
	while (doc->heredoc_delimiter[i])
	{
		here_tmp = ft_strdup("");
		here_tmp = heredoc(doc->heredoc_delimiter[i], 1, all);
		last_hrdoc = ft_strlen(here_tmp);
		doc->heredoc_content = ft_strjoin(doc->heredoc_content, here_tmp);
		free(here_tmp);
		i++;
	}
	if (doc->out_file || doc->in_file )
		{
			here_tmp2 = doc->heredoc_content;	
			doc->heredoc_content = ft_strdup( here_tmp2+(ft_strlen(here_tmp2)-last_hrdoc));
			free(here_tmp2);
		}
}
void	heredoc_child(int *_pipe, t_cmd *cmd, t_all *all)
{
	signal(SIGINT, SIG_DFL);
	close(_pipe[0]);
	heredoc_(cmd, all);
	write(_pipe[1], cmd->heredoc_content, ft_strlen(cmd->heredoc_content));
	close(_pipe[1]);
	exit(0);
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
