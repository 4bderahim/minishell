#include <minishell.h>

int iss_empty(char *s)
{
	int i;
	i =0;
	while (s[i] != 0 && s[i] == ' ')
		i++
		;
	if (s[i] != '\0')
		return (0);
	return (1);
}

int get_line_index(char *p_file, int i)
{
	while (p_file[i] && p_file[i] != ' ')
		i++;
	return (i);
}

char *fix_file_name(char *p_file)
{
	int i;
	int j;

	i = 0;
	if (iss_empty(p_file) || !ft_strlen(p_file))
		return (NULL);
	while (*p_file)
	{
		if (*p_file != ' ')
		{
		i = get_line_index(p_file, i);
		if (p_file[i] == 0)
			break;
		j = get_line_index(p_file, i);
		if (p_file[j] == '\0')
			break;
		else
			return (NULL);
		}
		p_file++;
	}
	p_file[i] = 0;
	return (p_file);
}
