/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:25:22 by mben-jad          #+#    #+#             */
/*   Updated: 2024/09/07 03:43:44 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_str_inside_quote(char *cmd, int *i, char c)
{
	while (cmd[*i] && cmd[*i] != c)
		*i += 1;
}

bool	is_symbol(char c)
{
	if (c == '\0')
		return (false);
	if (c == '|' || c == '<' || c == '>')
		return (true);
	return (false);
}

void	is_pipe(char c, int *counter)
{
	if (c == '|')
		*counter += 1;
}

int	reds_counter(char *cmd, t_all *all)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	if (!cmd)
		return (counter);
	while (i < ft_strlen(cmd))
	{
		if (i < ft_strlen(cmd) && (cmd[i] == '\'' || cmd[i] == '\"'))
		{
			i++;
			counter += 2;
			skip_str_inside_quote(cmd, &i, cmd[i - 1]);
		}
		else if (i < ft_strlen(cmd) && (cmd[i] == '<' || cmd[i] == '>'))
		{
			counter++;
			if (!skip_reds(cmd, &i, cmd[i], all))
				return (0);
		}
		is_pipe(cmd[i], &counter);
		i++;
	}
	return ((counter * 2));
}

bool	full_of_spaces(char *buffer)
{
	int	i;

	i = 0;
	while (buffer[i] && ft_isspace(buffer[i]))
		i++;
	if (i == ft_strlen(buffer))
		return (true);
	return (false);
}
