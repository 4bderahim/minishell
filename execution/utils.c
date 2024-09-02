/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-krid <ael-krid@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:54:34 by ael-krid          #+#    #+#             */
/*   Updated: 2024/08/16 14:54:50 by ael-krid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_write(char *str, int fd)
{
	if (str == NULL)
		return ;
	if (write(fd, str, ft_strlen(str)) == -1)
	{
		write(2, "error\n", 7);
		exit(1);
	}
}

int	n_flaged(char *str)
{
	if (str == NULL)
		return (0);
	if ((*str != '-' && *(str + 1) != 0) || *str == 0)
		return (0);
	if (*str == '-' && *(str + 1) != 'n')
		return (0);
	str++;
	while (*str)
	{
		if (*str != 'n')
			return (0);
		str++;
	}
	return (1);
}

void	echo_to_file(char **str, int i, int fd)
{
	printf("[%s]", str[i]);
	while (str[i])
	{
		ft_write(str[i], fd);
		if (str[i + 1] != NULL)
			ft_write(" ", fd);
		i++;
	}
}

void	ft_echo(char **str, int fd)
{
	int	i;
	int	flag;
	int	index_to_txt;

	flag = 0;
	i = 0;
	index_to_txt = 0;
	while (n_flaged(str[index_to_txt]))
		index_to_txt++;
	if (index_to_txt > 0)
	{
		if (str[1] == NULL)
		{
			return ;
		}
		flag = 1;
		i++;
	}
	echo_to_file(str, index_to_txt, fd);
	if (flag != 1)
		ft_write("\n", fd);
}

void	ft_pwd(t_all *all)
{
	char	buff[1024];
	char	*ret;

	ret = getcwd(buff, 1024);
	if (ret == NULL)
		ft_error(all);
	ft_write(ret, STDOUT_FILENO);
	ft_write("\n", STDOUT_FILENO);
}
