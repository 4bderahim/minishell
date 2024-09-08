/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executables.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:37:13 by mben-jad          #+#    #+#             */
/*   Updated: 2024/09/07 03:43:44 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (i);
	while (str[i])
		i++;
	return (i);
}

int	get_slash_index(char *cmd)
{
	int	i;
	int	index;

	i = 0;
	index = -1;
	if (!cmd)
		return (-1);
	while (cmd[i])
	{
		if (cmd[i] == '/')
			index = i;
		i++;
	}
	return (index);
}

char	*get_executable_path(char *cmd, char *filename)
{
	int		slash_index;
	char	*path;
	char	cwd[256];

	slash_index = get_slash_index(cmd);
	if (slash_index == -1)
		return (NULL);
	path = ft_strndup(cmd, slash_index + 1);
	chdir(path);
	free(path);
	path = NULL;
	getcwd(cwd, sizeof(cwd));
	path = ft_strjoin(ft_strjoin(ft_strdup(cwd), "/"), filename);
	free(filename);
	filename = NULL;
	return (path);
}

char	*get_executable(char *cmd)
{
	int		i;
	char	**arr;
	char	*filename;
	char	*path;

	i = 0;
	if (get_slash_index(cmd) == -1)
		return (NULL);
	arr = ft_split(cmd, '/');
	if (!arr)
		return (NULL);
	while (arr[i + 1] != NULL)
		i++;
	filename = ft_strdup(arr[i]);
	ft_free(arr, get_arr_len(arr));
	if (!filename)
		return (NULL);
	path = get_executable_path(cmd, filename);
	if (path != NULL)
		return (path);
	return (NULL);
}
