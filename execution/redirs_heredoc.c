/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int iss_empty(char *s)
{
	int i;
	i =0;
	while (s[i] != 0 && s[i] == ' ')
		i++
		;
	if (s[i] != '\0')
		return (0);
	return (1);
}
int get_line_index(char *p_file, int i)
{

	while (p_file[i] && p_file[i] != ' ')
			i++
			;
	return (i);
}
char *fix_file_name(char *p_file)
{
	int i;
	int j;

	i = 0;
	if (iss_empty(p_file) || !ft_strlen(p_file))
		return (NULL);
	while (*p_file)
	{
		if (*p_file != ' ')
		{
		i = get_line_index(p_file, i);
		if (p_file[i] == 0)
			break;
		j = get_line_index(p_file, i);
		if (p_file[j] == '\0')
			break;
		else
			return (NULL);
		}
		p_file++;
	}
	p_file[i] = 0;
	return (p_file);
}
             
int check_amb(char *file)
{
	int i;
	int found;

	i = 0;
	found = 0;
	while (file[i])
	{
		if (file[i] != ' ')
			{
				if (found > 1)
					return (1);
				found++;
			}
		if (found)
			found++;
		i++;
	}
	if (!found)
		return (1);
	return (0);
}
int ambig_outed(t_all *all)
{
	if (all->cmd->out_file || all->cmd->append_file)
	{
		if (all->cmd->out_file)
			return (check_amb(all->cmd->out_file));
		return (check_amb(all->cmd->append_file));
	}		
	return (0);

}

void	redirections_set(t_all *all)
{
	int	fd;

	if (all->cmd->in_file)
	{
		fd = open(all->cmd->in_file, O_RDONLY);
		if (fd == -1)
			ft_error(all);
		if (dup2(fd, STDIN_FILENO) < 0)
			ft_error(all);
		close(fd);
	}
	if (all->cmd->out_file || all->cmd->append_file)
	{
		printf("%zu\n\n", ft_strlen(all->cmd->out_file));
		// if (ambig_outed(all))
		// {
		// 	ft_write("minishell: ambiguous redirect\n", 2);
		// 	exit(1);
		// }
		printf("\t{%s}\n", fix_file_name(all->cmd->out_file));

		if (all->cmd->append_file)
			fd = open(all->cmd->append_file, O_CREAT | O_RDWR | O_APPEND);
		else
			fd = open(all->cmd->out_file, O_CREAT | O_WRONLY | O_TRUNC);
		if (fd == -1)
			ft_error(all);
		if (dup2(fd, STDOUT_FILENO) < 0)
			ft_error(all);
		close(fd);
	}
}

void	heredoc_pipe(t_all *all)
{
	int	fd;
	int	p[2];

	if (all->cmd->heredoc_delimiter == NULL)
		return ;
	if (pipe(p) < 0)
		ft_error(all);
	fd = fork();
	if (fd == -1)
		ft_error(all);
	if (fd == 0)
	{
		close(p[0]);
		redirections_set(all);
		ft_write(all->cmd->heredoc_content, p[1]);
		close(p[1]);
		write(1, "\n", 1);
		exit(1);
	}
	close(p[1]);
	if (dup2(p[0], STDIN_FILENO) < 0)
		ft_error(all);
	close(p[0]);
	if (all->cmd->cmd == NULL)
		exit(0);
}

void	redirect_in_out_to_pipe(int index, int pipe[], int *pr_fd, t_all *all)
{
	if (index != 0)
	{
		if (dup2(*pr_fd, STDIN_FILENO) < 0)
			ft_error(all);
		close(*pr_fd);
	}
	if (index < all->nums_of_cmds - 1)
	{
		if (dup2(pipe[1], STDOUT_FILENO) < 0)
			ft_error(all);
		close(pipe[1]);
	}
}