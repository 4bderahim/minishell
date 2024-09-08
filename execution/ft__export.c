
#include "minishell.h"

int	check_(char *s, int index)
{
	int		i;
	int		j;
	char	*sp;

	sp = "'`~#$&-*()\\|@[]{};<>\"/?!%%^+";
	i = 0;
	while (i < index)
	{
		j = 0;
		while (sp[j])
		{
			if (sp[j] == s[i])
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	is_empty(char *s, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		if (s[i] != '\t' && s[i] != '\n' && s[i] != ' ' && s[i] != '\r'
			&& s[i] != '\v' && s[i] != '\f')
			return (0);
		i++;
	}
	return (1);
}

int	check_before_env(char *s)
{
	int	i;

	i = 0;
	if ((s[i] >= '0' && s[i] <= '9') || s[i] == '=')
		return (0);
	while (s[i])
	{
		if (s[i] == '=')
		{
			if (is_empty(s, i) || !check_(s, i))
				return (0);
			return (1);
		}
		i++;
	}
	if (!check_(s, i))
		return (0);
	return (-1);
}

void	add_it_to_env(t_all *all, char *new, t_exp *new_exp)
{
	t_env	*new_env;

	new_env = env_new(new);
	if (new_env == NULL)
	{
		free(new);
		ft_error(all, 0);
	}
	free(new);
	if (all->env == NULL)
		all->env = new_env;
	else
		env_addback(all->env, new_env);
	if (all->exp == NULL)
		all->exp = new_exp;
	else
		exp_addback(all->exp, new_exp);
}
