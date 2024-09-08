/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-jad <mben-jad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 22:25:32 by mben-jad          #+#    #+#             */
/*   Updated: 2024/09/07 01:04:43 by mben-jad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_s_d_quotes(t_fix_cmd *vars, char *cmd)
{
	vars->quote = cmd[vars->i];
	vars->k = vars->i;
	vars->line[vars->j++] = cmd[vars->i++];
	while (vars->i < ft_strlen(cmd) && cmd[vars->i] != vars->quote)
		vars->line[vars->j++] = cmd[vars->i++];
	if (vars->i < ft_strlen(cmd) && vars->i - vars->k == 1 && cmd[vars->i
			+ 1] != '\0' && cmd[vars->i + 1] == ' ' && vars->k - 1 >= 0
		&& cmd[vars->k - 1] == ' ')
		vars->line[vars->j++] = ' ';
}

void	vars_init(t_fix_cmd *vars, char *cmd, t_all *all)
{
	vars->i = 0;
	vars->j = 0;
	vars->k = 0;
	vars->quote = 0;
	vars->reds_nums = reds_counter(cmd, all);
}

char	*fix_cmd(char *cmd, t_all *all)
{
	t_fix_cmd	vars;

	vars_init(&vars, cmd, all);
	if (!vars.reds_nums)
		return (cmd);
	vars.line = malloc(ft_strlen(cmd) + vars.reds_nums + 10);
	if (!vars.line)
		return (NULL);
	while (vars.i < ft_strlen(cmd))
	{
		if (cmd[vars.i] == '\'' || cmd[vars.i] == '\"')
			handle_s_d_quotes(&vars, cmd);
		else if (((cmd[vars.i] == '<' || cmd[vars.i] == '>'
					|| cmd[vars.i] == '|') && (vars.i > 0 && (cmd[vars.i
							- 1] != '<' || cmd[vars.i - 1] != '>'))))
			vars.line[vars.j++] = ' ';
		else if (vars.i > 0 && (cmd[vars.i - 1] == '<' || cmd[vars.i
					- 1] == '>') && cmd[vars.i] != ' ')
			vars.line[vars.j++] = ' ';
		vars.line[vars.j++] = cmd[vars.i++];
	}
	vars.line[vars.j] = '\0';
	return (free(cmd), vars.line);
}
