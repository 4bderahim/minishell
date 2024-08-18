/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// size_t	ft_strlen(char *s)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i])
// 	{
// 		i++;
// 	}
// 	return (i);
// }
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
static void	join_the_two_strings(char *all, char const *s1, char const *s2)
{
	size_t	i;
	int		j;

	i = 0;
	if (s1 != 0)
	{
		while (s1[i])
		{
			all[i] = s1[i];
			i++;
		}
	}
	j = 0;
	while (s2[j])
	{
		all[i] = s2[j];
		i++;
		j++;
	}
}

// char	*ft_strjoin(char *s1, char *s2)
// {
// 	char	*allocated;
// 	size_t	string_len;
// 	size_t	x;

// 	x = 0;
// 	if (s1 != NULL)
// 	{
// 		x = ft_strlen(s1);
// 	}
// 	string_len = x + ft_strlen(s2);
// 	allocated = (char *)malloc(string_len + 1);
// 	if (!allocated)
// 	{
// 		return (NULL);
// 	}
// 	join_the_two_strings(allocated, s1, s2);
// 	allocated[string_len] = '\0';
// 	free(s1);
// 	return (allocated);
// }
// int write_fd(char *str, int fd)
// {
//     while (*str)
//     {
//         if (write(fd,  &(*str), 1) == -1)
//             return (0);
//         str++;
//     }
//     return (1);
// }

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
		if (!match_word(input, heredoc_str))
		{
			full_str = ft_strjoin(full_str, input);
			full_str = ft_strjoin(full_str, "\n");
		}
		else
			break ;
	}
	return (full_str);
}


void	heredoc_check(t_all *all)
{
	t_cmd	*doc;
	char *here_tmp;
	int i;


	doc = all->cmd;
	while (doc != NULL)
	{
		if (doc->heredoc_delimiter != NULL)
		{
			doc->heredoc_content = ft_strdup("");
			i = 0;
			while (doc->heredoc_delimiter[i])
			{
				here_tmp = ft_strdup("");
				here_tmp = heredoc(doc->heredoc_delimiter[i], 1, all);
				doc->heredoc_content = ft_strjoin(doc->heredoc_content+i, here_tmp);
				free(here_tmp);
				i++;
			}
			printf("[%s]\n\n\n", doc->heredoc_content);
		}
		doc = doc->next;
	}
}
