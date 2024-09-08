/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:42:35 by mben-jad          #+#    #+#             */
/*   Updated: 2024/09/07 01:02:32 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_args_dup(char **args, t_all *all)
{
	int		i;
	int		j;
	char	**result;

	i = -1;
	j = 0;
	if (!count_valid_args(args))
		return (NULL);
	result = malloc(sizeof(char *) * (count_valid_args(args) + 2));
	if (!result)
		return (NULL);
	while (++i < get_arr_len(args))
	{
		while (args[i] && is_redirection(args[i], args[i + 1]))
			i += is_redirection(args[i], args[i + 1]);
		if (!args[i])
			break ;
		if (j == 0 && get_vars_length(args[i]) > 0
			&& args[i][0] != '\"')
			handle_var_as_cmd(result, args[i], &j, all);
		else
			result[j++] = catch_arg(args[i], all);
	}
	result[j] = NULL;
	return (result);
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd->args)
		ft_free(cmd->args, get_arr_len(cmd->args));
	if (cmd->cmd)
		free(cmd->cmd);
	if (cmd->in_file)
		free(cmd->in_file);
	if (cmd->out_file)
		free(cmd->out_file);
	if (cmd->heredoc_content)
		free(cmd->heredoc_content);
	if (cmd->heredoc_delimiter)
		free(cmd->heredoc_delimiter);
	if (cmd->full_path)
		free(cmd->full_path);
	if (cmd->append_file)
		free(cmd->append_file);
}

void	ft_lstadd_back(t_cmd **cmd, t_cmd *new)
{
	t_cmd	*current;

	current = *cmd;
	if (!new)
		return ;
	if (!current)
	{
		*cmd = new;
		return ;
	}
	while (current->next != NULL)
		current = current->next;
	current->next = new;
}

int	is_redirection(char *str, char *next)
{
	if (!ft_strcmp(str, ">") && ft_strcmp(next, ">"))
		return (2);
	else if (!ft_strcmp(str, ">") && !ft_strcmp(next, ">"))
		return (3);
	else if (!ft_strcmp(str, "<") && ft_strcmp(next, "<"))
		return (2);
	else if (!ft_strcmp(str, "<") && !ft_strcmp(next, "<"))
		return (3);
	else
		return (0);
}
