/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_piped_ins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/09/04 19:19:46 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_exp_list(t_all *all)
{
	t_exp	*tmp;
	int		i;

	tmp = all->exp;
	i = 0;
	while (tmp != NULL)
	{
		ft_write("declare -x ", STDOUT_FILENO);
		ft_write(tmp->variable, STDOUT_FILENO);
		if (tmp->value != NULL)
			ft_write("=", STDOUT_FILENO);
		if (tmp->value != NULL)
			ft_write("\"", STDOUT_FILENO);
		ft_write(tmp->value, STDOUT_FILENO);
		if (tmp->value != NULL)
			ft_write("\"", STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", STDOUT_FILENO);
		tmp = tmp->next;
		i++;
	}
}

char	**n_env(t_all *all, char **env)
{
	int		i;
	char	**envp;

	i = 0;
	while (env[i])
		i++;
	envp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!envp)
	{
		ft_lstclear(&all->cmd);
		exit(1);
	}
	envp[i] = NULL;
	return (envp);
}

void	print_env_list(t_all *all)
{
	t_env	*tmp;

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

void	exec_piped_built_ins(t_all *all)
{
	if (match_word(all->cmd->cmd, "echo") || match_word(all->cmd->cmd, "ECHO"))
		ft_echo(all->cmd->args + 1, STDOUT_FILENO);
	else if (match_word(all->cmd->cmd, "env") || match_word(all->cmd->cmd,
			"ENV"))
		print_env_list(all);
	else if (match_word(all->cmd->cmd, "export") && all->cmd->args[1] == NULL)
		print_exp_list(all);
	else if (match_word(all->cmd->cmd, "pwd") || match_word(all->cmd->cmd,
			"PWD"))
		ft_pwd(all);
	else
		return ;
	exit(0);
}
