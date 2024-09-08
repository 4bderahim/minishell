
#include "minishell.h"

char	*ft_strncpy(char *str, int size)
{
	int		i;
	char	*result;

	i = 0;
	result = malloc(sizeof(char) * size + 1);
	if (!result)
		return (NULL);
	while (i < size)
	{
		result[i] = str[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

bool	search_for_2nd_quote(char *str, char c)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (str[i])
	{
		if (str[i] == c)
			counter++;
		i++;
	}
	if (counter % 2 == 1)
	{
		return (true);
	}
	return (false);
}

int	ft_strchr(char *str, char c)
{
	int	i;
	int	in_quotes;

	i = 0;
	in_quotes = 1;
	if (!search_for_2nd_quote(str, c))
		return (-1);
	while (str[i])
	{
		if (!in_quotes && (str[i] == ' ' || str[i] == '|'))
			return (i);
		if (str[i] == c)
		{
			if (in_quotes == 1)
				in_quotes = 0;
			else
				in_quotes = 1;
		}
		i++;
	}
	return (i);
}

int	ft_strchr_pro(char *str, char c1, char c2)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i] && str[i] != c1 && str[i] != c2)
	{
		i++;
		if (str[i] == '\0')
			return (-1);
	}
	return (i + 1);
}

char	*ft_strndup(char *str, int n)
{
	int		i;
	char	*result;

	if (!str)
		return (NULL);
	i = 0;
	result = (char *)malloc(n + 1);
	if (!result)
		return (NULL);
	while (i < n)
	{
		result[i] = str[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
