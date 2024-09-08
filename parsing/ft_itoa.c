/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:37:19 by mben-jad          #+#    #+#             */
/*   Updated: 2024/09/02 19:37:40 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	numlen(long int num)
{
	int	i;

	i = 0;
	if (num == 0)
		return (1);
	else
	{
		while (num != 0)
		{
			num /= 10;
			i++;
		}
	}
	return (i);
}

static char	*ft_strrev(char *res)
{
	size_t	i;
	size_t	j;
	char	temp;

	i = 0;
	j = ft_strlen(res) - 1;
	while (i < j)
	{
		temp = res[i];
		res[i] = res[j];
		res[j] = temp;
		i++;
		j--;
	}
	return (res);
}

static char	*alloc_str(long int *num)
{
	char	*res;
	int		len;

	len = numlen(*num);
	if (*num < 0)
	{
		*num *= (-1);
		len++;
	}
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	return (res);
}

char	*ft_itoa(int n)
{
	char		*res;
	int			i;
	long int	num;

	i = 0;
	num = n;
	res = alloc_str(&num);
	if (!res)
		return (NULL);
	if (num == 0)
		res[i++] = '0';
	while (num != 0)
	{
		res[i] = (num % 10) + 48;
		num /= 10;
		i++;
	}
	if (n < 0)
		res[i++] = '-';
	res[i] = '\0';
	return (ft_strrev(res));
}
