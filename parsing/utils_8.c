/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 22:30:45 by mben-jad          #+#    #+#             */
/*   Updated: 2024/09/07 01:05:57 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(int *i, t_all *all, char *cmd)
{
	int	len;

	*i += 1;
	len = ft_strchr(cmd + *i, cmd[*i - 1]);
	if (len == -1)
		return (throw_error("syntax error", all, 2), 0);
	skip_str_inside_quote(cmd, i, cmd[*i - 1]);
	return (1);
}

bool	is_symbol_in_cmd(char c)
{
	if (c == '/' || c == '\\')
		return (true);
	return (false);
}

int	count_commands(t_cmd *cmd)
{
	int	counter;

	counter = 0;
	while (cmd)
	{
		cmd = cmd->next;
		counter++;
	}
	return (counter);
}

bool	is_correct_cmd(char *cmd, t_all *all)
{
	int	i;

	i = 0;
	skip_spaces(cmd, &i);
	if (cmd[i] == '|')
		return (throw_error("syntax error", all, 258), false);
	while (i < ft_strlen(cmd))
	{
		skip_spaces(cmd, &i);
		if (cmd[i] == '\'' || cmd[i] == '\"')
		{
			if (!check_quotes(&i, all, cmd))
				return (false);
		}
		else if (cmd[i] == '|')
		{
			if (cmd[++i] && cmd[i] == '|')
				i++;
			skip_spaces(cmd, &i);
		}
		i++;
	}
	return (true);
}
