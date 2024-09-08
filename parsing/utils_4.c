
#include "minishell.h"

int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	find_len(char *str)
{
	int	i;
	int	str_len;

	i = 0;
	str_len = ft_strlen(str);
	while (i < str_len && str[i] != ' ' && str[i] != '|')
	{
		if (i < str_len && (str[i] == '\'' || str[i] == '\"'))
		{
			i++;
			skip_str_inside_quote(str, &i, str[i - 1]);
		}
		i++;
		if (i < str_len && str[i] == '\0')
		{
			free(str);
			str = NULL;
			return (-1);
		}
	}
	return (i);
}

bool	str_has_quotes(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (true);
		i++;
	}
	return (false);
}

bool	is_pipe_after(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '|')
		return (true);
	return (false);
}

bool	no_herdoc_delemiter(char *cmd, int i)
{
	if (!cmd)
		return (false);
	while (i >= 0)
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
		{
			i--;
			while (i >= 0 && cmd[i] != cmd[i + 1])
				i--;
		}
		if (i >= 0 && cmd[i] == '|')
			return (true);
		else if (cmd[i] == '<')
		{
			i--;
			if (i >= 1 && cmd[i] == ' ' && cmd[i - 1] == '<')
				return (false);
		}
		i--;
	}
	return (true);
}
