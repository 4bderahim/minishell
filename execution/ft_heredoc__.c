/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc__.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	match_word(char *neadle, char *str)
{
	int	i;

	i = 0;
	if (!str || !neadle)
		return (0);
	if (!(*str) || !(*neadle))
		return (0);
	while (str[i])
	{
		if (str[i] != neadle[i])
			return (0);
		i++;
	}
	if (neadle[i] != 0)
		return (0);
	return (1);
}

int	no_expand(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '$' && (s[i + 1] != 0 || s[i + 1] != ' '))
			return (0);
		i++;
	}
	return (1);
}

char	*fill_full_str(char *full_str, char *input, t_all *all)
{
	full_str = ft_strjoin(full_str, input);
	if (full_str == NULL)
		ft_error(all);
	full_str = ft_strjoin(full_str, "\n");
	free(input);
	return (full_str);
}

char	*heredoc(char *heredoc_str, int fd, t_all *all)
{
	char	*full_str;
	char	*input;

	full_str = (char *)malloc(1);
	if (!full_str)
		ft_error(all);
	full_str[0] = 0;
	while (1)
	{
		input = readline(">>");
		if (!input)
			exit(0);
		if (!match_word(input, heredoc_str))
			full_str = fill_full_str(full_str, input, all);
		else
		{
			free(input);
			break ;
		}
	}
	return (full_str);
}

void	heredoc_(t_cmd *doc, t_all *all)
{
	char	*here_tmp;
	int		i;
	int		last_hrdoc;
	char	*here_tmp2;

	doc->heredoc_content = ft_strdup("");
	i = 0;
	while (doc->heredoc_delimiter[i])
	{
		here_tmp = ft_strdup("");
		here_tmp = heredoc(doc->heredoc_delimiter[i], 1, all);
		last_hrdoc = ft_strlen(here_tmp);
		doc->heredoc_content = ft_strjoin(doc->heredoc_content, here_tmp);
		free(here_tmp);
		i++;
	}
	if (doc->out_file || doc->append_file)
	{
		here_tmp2 = doc->heredoc_content;
		doc->heredoc_content = ft_strdup(here_tmp2 + (ft_strlen(here_tmp2)
					- last_hrdoc));
		free(here_tmp2);
	}
}
