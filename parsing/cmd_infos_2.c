
#include "minishell.h"

bool	correct_path(char *path, char *cmd, bool is_path)
{
	if (is_path)
	{
		if (!ft_strcmp(path, cmd))
			return (true);
		return (false);
	}
	return (true);
}

int	get_arr_len(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (i);
	while (arr[i])
		i++;
	return (i);
}

static char	**get_delimiters_len(char **args)
{
	int		i;
	int		len;
	char	**delimiters;

	i = 0;
	len = 0;
	while (args[i])
	{
		if (!ft_strcmp(args[i], "<") && !ft_strcmp(args[i + 1], "<"))
			len++;
		i++;
	}
	if (!len)
		return (NULL);
	delimiters = malloc(sizeof(char *) * (len + 1));
	if (!delimiters)
		return (NULL);
	return (delimiters);
}

char	*get_delimiter(char *arg)
{
	char	*buffer;

	buffer = find_and_remove(ft_strdup(arg), '\"');
	buffer = find_and_remove(buffer, '\'');
	if (buffer[0] == '\0')
	{
		free(buffer);
		buffer = ft_strdup("\0");
	}
	return (buffer);
}

char	**get_herdoc_delimiter(char **args, t_all *all)
{
	int		i;
	int		j;
	char	**delimiters;

	i = 0;
	delimiters = get_delimiters_len(args);
	if (!delimiters)
		return (NULL);
	i = 0;
	j = 0;
	while (args[i])
	{
		if (!ft_strcmp(args[i], "<") && !ft_strcmp(args[i + 1], "<"))
		{
			i++;
			if (!args[i + 1])
				return (throw_error("syntax error", all, 258), NULL);
			else
				delimiters[j++] = get_delimiter(args[i + 1]);
		}
		i++;
	}
	delimiters[j] = NULL;
	return (delimiters);
}
