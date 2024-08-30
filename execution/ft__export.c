/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft__export.c                                        :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_(char *s, int index)
{
	int		i;
	int		j;
	char	*sp;

	sp = "'`~#$&-*()\\|[]{};<>\"/?!%%^";
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
int is_empty(char *s, int index)
{
	int i;

	i = 0;
	while (i < index)
	{
		if (s[i] != '\t' && s[i] != '\n' 
			&& s[i] != ' ' && s[i] != '\r' && s[i] != '\v' && s[i] != '\f')
			return (0);
		i++;
	}
	return (1);
	
}
int	check_before_env(char *s)
{
	int	i;

	i = 0;
	if ((s[i] >= '0' && s[i] <= '9') || s[i] == '=') //  also check if empty
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
		ft_error(all);
	}
	free(new);
	env_addback(all->env, new_env);
	exp_addback(all->exp, new_exp);
	// free(new_exp->value);
	// free(new_exp->variable);
	// free(new_exp);

}
void	identifier_error(char *indentifer)
{
	// ft_write("export: ", 2);
	// ft_write(indentifer, 2);
	
	
}

void	parse_indetifier(t_all *all, char *str)
{
	int i;
	int ret;
	t_exp *last;
	char *tmp_str;

	i = 0;
	tmp_str = ft_strdup(str);
	ret = check_before_env(str);
	if (ret == 0)
	{
		// free(tmp_str);
		ft_write("minishell: not a valid identifier\n", 2);
		all->exit_status = 1;
		return ;
	}
	last = exp_new(str);
	if (ret == -1)
	{
		if (unset_exp(all, last, ret))
		{
			free(last);
			free(tmp_str); // check ...
			return ;
		}
		//free(tmp_str);
		exp_addback(all->exp, last); 
		return ;
	}
	if (unset_exp(all, last, ret))
	{
		free(tmp_str); // check ...
		return ;
	}
	add_it_to_env(all, tmp_str, last);
}