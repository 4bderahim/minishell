/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_piped_ins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	print_exp_list(t_all *all, int pipe[2])
{
	t_exp	*tmp;
	int		i;
	char	*str;

	tmp = all->exp;
	i = 0;
	while (tmp != NULL)
	{
		ft_write("declare -x ", STDOUT_FILENO);
		ft_write(tmp->variable, STDOUT_FILENO);
		ft_write("=", STDOUT_FILENO);
		ft_write("\"", STDOUT_FILENO);
		ft_write(tmp->value, STDOUT_FILENO);
		ft_write("\"", STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", STDOUT_FILENO);
		tmp = tmp->next;
		i++;
	}
}

void	print_env_list(t_all *all)
{
	t_env	*tmp;
	int		i;

	tmp = all->env;
	while (tmp != NULL)
	{
		ft_write(tmp->variable, STDOUT_FILENO);
		ft_write("=", STDOUT_FILENO);
		ft_write(tmp->value, STDOUT_FILENO);
		ft_write("\n", STDOUT_FILENO);
		tmp = tmp->next;
	}
}

void	exec_piped_built_ins(t_all *all, int pipes[2])
{
	int		i;
	char	*str;

	i = 0;
	if (match_word(all->cmd->cmd, "echo"))
		ft_echo(all->cmd->args + 1, STDOUT_FILENO);
	else if (match_word(all->cmd->cmd, "env"))
		print_env_list(all);
	else if (match_word(all->cmd->cmd, "export") && all->cmd->args[1] == NULL)
		print_exp_list(all, pipes);
	else if (match_word(all->cmd->cmd, "pwd"))
		ft_pwd(all);
	else
		return ;
	exit(0);
}
