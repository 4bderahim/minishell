
#include "minishell.h"

char	*handle_exit_status(t_all *all, char *output, int *i)
{
	char	*var_value;

	var_value = ft_itoa(all->exit_status);
	output = ft_strjoin(output, var_value);
	free(var_value);
	var_value = NULL;
	*i += 1;
	return (output);
}

char	*handle_regular_variable(char *str, int *i, t_env *env, char *output)
{
	char	*var_name;
	char	*var_value;

	var_name = extract_var_name(str, i);
	var_value = get_var_value(var_name, env);
	output = ft_strjoin(output, var_value);
	if (!output)
	{
		free(var_value);
		var_value = NULL;
	}
	free(var_value);
	var_value = NULL;
	return (output);
}

char	*append_regular_char(char *output, char c)
{
	char	*result;
	char	*character;

	character = malloc(2);
	character[0] = c;
	character[1] = '\0';
	result = ft_strjoin(output, character);
	free(character);
	character = NULL;
	return (result);
}

int	no_single_quote(char *str, int i)
{
	int	counter;

	counter = 0;
	while (i >= 0)
	{
		if (str[i] == '\'')
			counter++;
		i--;
	}
	if (counter % 2 == 0)
		return (1);
	return (0);
}

char	*handle_variables(char *str, t_env *env, t_all *all)
{
	int		i;
	int		len;
	char	*output;

	i = 0;
	output = NULL;
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	while (i < len)
	{
		if (str[i] == '$' && no_single_quote(str, i - 1))
		{
			i++;
			if (i < len && str[i] == '?')
				output = handle_exit_status(all, output, &i);
			else
				output = handle_regular_variable(str, &i, env, output);
		}
		else
			output = append_regular_char(output, str[i++]);
	}
	return (ft_strtok(output));
}
