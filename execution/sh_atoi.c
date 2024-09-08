
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
