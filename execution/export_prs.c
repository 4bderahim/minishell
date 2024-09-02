/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_prs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__unset_exp(t_all *all, t_exp *last, int ret, char *tmp_str)
{
	if (ret == -1)
	{
		if (unset_exp(all, last, ret))
		{
			free(last->variable);
			free(last->value);
			free(last);
			free(tmp_str);
			return (1);
		}
		free(tmp_str);
		if (all->exp == NULL)
			all->exp = last;
		else
			exp_addback(all->exp, last);
		return (1);
	}
	return (0);
}

void	parse_indetifier(t_all *all, char *str)
{
	int		i;
	int		ret;
	t_exp	*last;
	char	*tmp_str;

	i = 0;
	tmp_str = ft_strdup(str);
	ret = check_before_env(str);
	if (ret == 0)
	{
		free(tmp_str);
		ft_write("minishell: not a valid identifier\n", 2);
		all->exit_status = 1;
		return ;
	}
	last = exp_new(str);
	if (__unset_exp(all, last, ret, tmp_str))
		return ;
	if (unset_exp(all, last, ret))
	{
		free(tmp_str);
		return ;
	}
	add_it_to_env(all, tmp_str, last);
}
