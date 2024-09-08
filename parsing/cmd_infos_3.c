
#include "minishell.h"

static int	is_output_red(t_cmd *node, t_all *all, char **args, int i)
{
	int	fd;

	if (!args[i + 1])
		return (throw_error("syntax error", all, 258), 0);
	free(node->out_file);
	free(node->append_file);
	node->out_file = NULL;
	node->append_file = NULL;
	node->out_file = NULL;
	node->out_file = check_filename(args, i, all);
	fd = open(node->out_file, O_CREAT | O_RDWR, 0777);
	close(fd);
	return (1);
}

static int	is_output_append_red(t_cmd *node, t_all *all, char **args, int i)
{
	int	fd;

	if (!args[i + 1])
		return (throw_error("syntax error", all, 258), 0);
	free(node->append_file);
	free(node->out_file);
	node->out_file = NULL;
	node->append_file = NULL;
	node->append_file = check_filename(args, i, all);
	fd = open(node->append_file, O_CREAT | O_RDWR | O_APPEND, 0777);
	close(fd);
	return (1);
}

void	get_output_redirection_file(char **args, t_all *all, t_cmd *node)
{
	int	i;

	node->append_file = NULL;
	node->out_file = NULL;
	i = -1;
	while (args[++i])
	{
		if (!ft_strcmp(args[i], ">") && args[i + 1] && ft_strcmp(args[i + 1],
				">"))
		{
			if (!is_output_red(node, all, args, i))
				return ;
		}
		else if (!ft_strcmp(args[i], ">") && args[i + 1] && !ft_strcmp(args[i
					+ 1], ">"))
		{
			i++;
			if (!is_output_append_red(node, all, args, i))
				return ;
		}
	}
}

int	ft_isalpha(char c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}

bool	is_incorrect_cmd(char *cmd, int *pipe, t_all *all)
{
	int	i;

	i = 0;
	if (!cmd)
		return (free(cmd), false);
	if (cmd[i] == '|')
	{
		i++;
		skip_spaces(cmd, &i);
		if (cmd[i] == '|' || i == ft_strlen(cmd))
			return (throw_error("syntax error", all, 258), free(cmd), true);
	}
	if (i + 1 < ft_strlen(cmd) && cmd[i + 1] == '|' && i + 2 < ft_strlen(cmd)
		&& cmd[i + 2] != '|')
	{
		*pipe = -1;
		return (free(cmd), false);
	}
	return (free(cmd), false);
}
