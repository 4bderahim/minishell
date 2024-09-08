#include "minishell.h"

char	*ft_strtok(char *str)
{
	char	*result;
	char	*dest;
	int		i;
	int		end;

	end = ft_strlen(str) - 1;
	i = 0;
	skip_spaces(str, &i);
	if (str[i] == '\0')
		return (free(str), ft_strdup("\0"));
	while (end > i && ft_isspace(str[end]))
		end--;
	result = (char *)malloc((end - i + 1) + 1);
	if (result == NULL)
		return (NULL);
	dest = result;
	while (i <= end)
	{
		if (!ft_isspace(str[i]) || (dest != result && !ft_isspace(*(dest - 1))))
			*dest++ = str[i];
		i++;
	}
	*dest = '\0';
	free(str);
	return (result);
}

char	*get_only_var(char *str)
{
	int		len;
	char	*result;

	len = 0;
	while (str[len] && !is_symbol(str[len]) && str[len] != ' '
		&& str[len] != '\'')
		len++;
	result = ft_substr(str, 0, len);
	return (result);
}

char	*get_var_value(char *str, t_env *env)
{
	char	*var;
	char	*rest;
	char	*result;

	result = NULL;
	var = get_only_var(str);
	rest = ft_strdup(str + ft_strlen(var));
	while (env)
	{
		if (!ft_strcmp(var, env->variable))
		{
			result = ft_strjoin(ft_strdup(env->value), rest);
			break ;
		}
		env = env->next;
	}
	free(str);
	str = NULL;
	free(var);
	if (!result)
		return (rest);
	free(rest);
	return (result);
}

int	ft_isalnum(int c)
{
	if ((c >= 48 && c <= 57) || (c >= 97 && c <= 122) || (c >= 65 && c <= 90))
		return (1);
	return (0);
}

char	*extract_var_name(char *str, int *i)
{
	int		var_len;
	char	var_name[1024];

	var_len = 0;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
	{
		var_name[var_len++] = str[*i];
		*i += 1;
	}
	var_name[var_len] = '\0';
	return (ft_strdup(var_name));
}
