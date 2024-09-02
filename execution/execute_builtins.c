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

int	sh_atoi(char *s)
{
	int	res;
	int	i;
	int	sign;

	sign = 1;
	i = 0;
	res = 0;
	while (s[i] != 0)
	{
		if (s[i] == '+' || s[i] == '-')
		{
			if (!(s[i + 1] >= '0' && s[i + 1] <= '9'))
				return (-1);
			sign *= -1;
			i++;
		}
		if (!(s[i] >= '0' && s[i] <= '9'))
			return (-1);
		res = (res * 10) + (s[i] - 48);
		i++;
	}
	return (res * sign);
}

void	free_and_exit(t_all *all, int exit_)
{
	env_exp_lists_clear(all);
	exit(exit_);
}

void	handle_exit(t_all *all)
{
	int	exit_num;
	int	exit_;

	ft_write("exit\n", 2);
	if (all->cmd->args[1] != NULL)
	{
		exit_num = sh_atoi(all->cmd->args[1]);
		if (exit_num == -1 && (all->cmd->args[1][0] != '-'
			&& all->cmd->args[1][0] != '1'))
		{
			ft_write("minishell: numeric argument required\n", 2);
			free_and_exit(all, 255);
			env_exp_lists_clear(all);
		}
		if (all->cmd->args[2])
		{
			ft_write("minishell: too many arguments\n", 2);
			return ;
		}
		free_and_exit(all, exit_num);
	}
	if (all->cmd->pipe)
		return ;
	exit_ = all->exit_status;
	free_and_exit(all, exit_);
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
		if (all->cmd->args[1] != NULL)
			unset_env(all);
		exec++;
	}
	if (match_word(all->cmd->cmd, "exit"))
		handle_exit(all);
	if (match_word(all->cmd->cmd, "cd"))
	{
		change_dir(all, all->cmd->args[1]);
		exec++;
	}
	if (exec)
		return (1);
	return (0);
}
