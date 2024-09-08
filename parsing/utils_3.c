/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 09:48:46 by mben-jad          #+#    #+#             */
/*   Updated: 2024/09/07 03:43:44 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	throw_error(char *msg, t_all *all, int nbr)
{
	int	i;

	i = 0;
	write(2, "Error: ", 7);
	while (msg[i])
		write(2, &msg[i++], 1);
	write(2, "\n", 1);
	all->exit_status = nbr;
	all->error = true;
}

int	find_pipe_index(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	while (i < len && str[i])
	{
		if (str[i] == '|')
			return (i);
		else if (i < len && str[i] == '\"')
		{
			i++;
			while (i < len && str[i] && str[i] != '\"')
				i++;
		}
		else if (i < len && str[i] == '\'')
		{
			i++;
			while (i < len && str[i] && str[i] != '\'')
				i++;
		}
		i++;
	}
	return (-1);
}

int	args_counter(char *str, int len)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	if (len == -1)
		len = ft_strlen(str);
	while (i < len && str[i])
	{
		skip_spaces(str, &i);
		if (i < len && (str[i] == '\"' || str[i] == '\''))
		{
			i++;
			skip_str_inside_quote(str, &i, str[i - 1]);
		}
		if (i < len && str[i] != ' ')
			words++;
		while (i < len && str[i] && str[i] != ' ')
			i++;
	}
	return (words);
}

void	ft_free(char **args, int len)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (i < len)
	{
		if (args[i] != NULL)
			free(args[i]);
		i++;
	}
	free(args);
	args = NULL;
}

int	nums_of_chars(char *str, char c)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	if (!str)
		return (counter);
	while (str[i])
	{
		if (str[i] == c)
			counter++;
		i++;
	}
	return (counter);
}
