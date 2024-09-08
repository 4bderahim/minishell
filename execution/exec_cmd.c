/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/09/06 21:44:11 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	executing_commands(t_all *all, char **envpp)
{
	// redirect stdin/stdout to files if existed!
	redirections_set(all);
	// read from heredoc
	heredoc_pipe(all);
	if (exec_built_ins(all))
		exit(0);
	// execute built in commands
	exec_piped_built_ins(all);
	if (all->cmd->cmd_not_found)
	{
		// throw error msg and continue executing all command in a piped command line !
		ft_write("minishell: command not found\n", 2);
		ft_error(all, 1);
	}
	/*
	call execev() func 
	end throw error from the errno seted by execve() in a failure
	otherwise execve() does'nt return!
	*/
	if (execve(all->cmd->full_path, all->cmd->args, envpp) == -1)
	{
		ft_write(strerror(errno), 2);
		write(2, "\n", 1);
	}
	if (errno == 13 || errno == 2)
		exit(127);
	exit(1);
}
