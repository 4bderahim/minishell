/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 20:17:09 by mben-jad          #+#    #+#             */
/*   Updated: 2024/09/07 19:44:33 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_spaces(char *cmd, int *i)
{
	while (cmd[*i] && ft_isspace(cmd[*i]))
		*i += 1;
}

void	shell_init(t_all *all, char **env, int ac, char **av)
{
	(void)ac;
	(void)av;
	using_history();
	set_lists(all, env);
	setup_signal_handlers();
	all->exit_status = g_signaled;
}

int	start_shell(char *read, t_all **all, char **env)
{
	add_history(read);
	if (!is_correct_cmd(read, *all))
	{
		free(read);
		read = NULL;
		return (0);
	}
	read = fix_cmd(read, *all);
	if ((*all)->error || !ft_lexer(read, all) || !(*all)->cmd)
	{
		free(read);
		read = NULL;
		return (0);
	}
	(*all)->nums_of_cmds = count_commands((*all)->cmd);
	(*all)->pipes_num = (*all)->nums_of_cmds - 1;
	if (!(*all)->error)
		execution(all, env);
	free((*all)->_vars->pids);
	free((*all)->_vars);
	free(read);
	read = NULL;
	ft_lstclear(&(*all)->cmd);
	return (1);
}

t_all	*alloc_all(void)
{
	t_all	*all;

	all = malloc(sizeof(t_all));
	if (!all)
		exit(1);
	return (all);
}

int	main(int ac, char **av, char **env)
{
	t_all	*all;
	char	*read;

	all = alloc_all();
	all->cmd = NULL;
	shell_init(all, env, ac, av);
	while (1)
	{
		all->error = false;
		read = readline("minishell > ");
		if (!read)
		{
			env_exp_lists_clear(all);
			exit(0);
		}
		if (ft_strlen(read))
		{
			if (!start_shell(read, &all, env))
				continue ;
		}
		else
			free(read);
	}
	return (all->exit_status);
}
