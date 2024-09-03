/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/09/03 22:11:38 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sh_atoi(char *s)
{
	int	res;
	int	i;
	int	sign;

	sign = 1;
	i = 0;
	res = 0;
	while (s[i] != 0)
	{
		if (s[i] == '+' || s[i] == '-')
		{
			if (!(s[i + 1] >= '0' && s[i + 1] <= '9'))
				return (-1);
			sign *= -1;
			i++;
		}
		if (!(s[i] >= '0' && s[i] <= '9'))
			return (-1);
		res = (res * 10) + (s[i] - 48);
		i++;
	}
	return (res * sign);
}
