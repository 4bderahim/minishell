/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_export(t_all *all)
{
	int	i;

	i = 1;
	while (all->cmd->args[i])
	{
		parse_indetifier(all, all->cmd->args[i]);
		i++;
	}
}

void	free_and_exit(t_all *all, int exit_)
{
	env_exp_lists_clear(all);
	exit(exit_);
}

int	check_exit_args(int exit_num, t_all *all)
{
	if (exit_num == -1 && (all->cmd->args[1][0] != '-'
		|| all->cmd->args[1][1] != '1'))
	{
		ft_write("minishell: numeric argument required\n", 2);
		all->exit_status = 255;
	}
	else
		return (0);
	if (all->pipes_num)
		return (1);
	free_and_exit(all, 255);
	env_exp_lists_clear(all);
	return (0);
}

int	handle_exit(t_all *all)
{
	int	exit_num;
	int	exit_;

	if (all->cmd->args[1] != NULL)
	{
		exit_num = sh_atoi(all->cmd->args[1]);
		if (check_exit_args(exit_num, all))
			return (1);
		if (all->cmd->args[2])
		{
			ft_write("minishell: too many arguments\n", 2);
			all->exit_status = 255;
			return (1);
		}
		if (!all->pipes_num)
			free_and_exit(all, exit_num);
	}
	if (all->pipes_num)
		return (1);
	exit_ = all->exit_status;
	free_and_exit(all, exit_);
	return (0);
}

int	exec_built_ins(t_all *all)
{
	int	exec;

	exec = 0;
	if (match_word(all->cmd->cmd, "export") && all->cmd->args[1] != NULL)
	{
		handle_export(all);
		exec++;
	}
	if (match_word(all->cmd->cmd, "unset"))
	{
		if (!all->pipes_num && all->cmd->args[1] != NULL)
			unset_env(all);
		exec++;
	}
	if (match_word(all->cmd->cmd, "exit") && handle_exit(all))
		exec++;
	if (match_word(all->cmd->cmd, "cd"))
	{
		change_dir(all, all->cmd->args[1]);
		exec++;
	}
	if (exec)
		return (1);
	return (0);
}
