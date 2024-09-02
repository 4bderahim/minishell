/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/30 11:09:54 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	executing_commands(t_all *all, int *pipe_sides, char **envpp)
{
	redirections_set(all);
	heredoc_pipe(all);
	exec_piped_built_ins(all, pipe_sides);
	if (all->cmd->cmd_not_found)
	{
		ft_write("minishell: command not found\n", 2);
		ft_error(all);
	}
	if (execve(all->cmd->full_path, all->cmd->args, envpp) == -1)
		ft_write(strerror(errno), 2);
	if (errno == 13 || errno == 2)
		exit(127);
	exit(1);
}
