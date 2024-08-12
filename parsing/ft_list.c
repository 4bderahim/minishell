#include "minishell.h"

// void ft_init(t_shell *shell)
// {
//   shell = malloc(sizeof(t_shell));
//   shell->head = NULL;
//   shell->size = 0;
//   shell->state = NORMAL;
// }

static bool is_redirection(char *str)
{
	if(!ft_strcmp(str, ">"))
		return true;
	else if(!ft_strcmp(str, ">>"))
		return true;
	else if(!ft_strcmp(str, "<"))
		return true;
	else
		return false;
}

static size_t count_valid_args(char **args)
{
	size_t i;
	size_t counter;

	i = 0;
	counter = 0;
	while(args[i])
	{
		while(args[i] && is_redirection(args[i]))
			i += 2;
		if(!args[i])
			break;
		counter++;
		i++;
	}
	return counter;
}

// char *check_arg(char *arg)
// {
// 	size_t i;

// 	i = 0;
// 	while(arg[i])
// 	{
// 		if((arg[i] == BACK_SLASH && arg[i + 1] == VAR_SIGN) || arg[i] && arg[i] == VAR_SIGN)
// 		{

// 		}
// 	}
// }

char **ft_args_dup(char **args)
{
	int i;
	int j;
	char **result;
	char arg;

	i = 0;
	j = 0;
	// arg = NULL;
	result = malloc(sizeof(char *) * (count_valid_args(args) + 1));
	if(!result)
		return NULL;
	while (args[i])
	{
		while(args[i] && is_redirection(args[i]))
			i += 2;
		if(!args[i])
			break;
		// arg = check_arg(args[i]);
		result[j++] = ft_strdup(args[i]);
		i++;
	}
	result[j] = NULL;
	return result;
}

// bool check_args(char **args)
// {
// 	int i;

// 	i = 0;
// 	if(!args)
// 		return false;
// 	while(args[i])
// 	{
// 		if(!args[i + 1] && is_not_valid_symbol(args))
// 	}
// }

void free_cmd(t_cmd *cmd)
{
	ft_free(cmd->args);
	free(cmd->cmd);
	free(cmd->in_file);
	free(cmd->out_file);
	free(cmd->heredoc_content);
	free(cmd->heredoc_delimiter);
	free(cmd->full_path);
	free(cmd->append_file);
	free(cmd);
	cmd = NULL;
}

t_cmd	*ft_lstnew(char **args, int args_nbr, int pipe)
{
	t_cmd	*new_node;

	// if(!check_args(args))
		// throw_error("Something went wrong with arguments");
	new_node = malloc(sizeof(t_cmd));
	if (!new_node)
		return (NULL);
	new_node->arg_count = args_nbr;
	new_node->args = ft_args_dup(args);
	new_node->in_file = get_input_redirection_file(args);
	new_node->out_file = get_output_redirection_file(args);
	new_node->append_file = get_append_to_file(args);
	new_node->heredoc_delimiter = NULL;
	new_node->heredoc_content = NULL;
	new_node->pipe = pipe;
	new_node->next = NULL;
	new_node->cmd = ft_strdup(new_node->args[0]);
	new_node->full_path = get_path(new_node->cmd);
	if(!new_node->full_path)
	{
		free_cmd(new_node);
		return NULL;
	}
	// ft_free(args);
	return (new_node);
}

// t_node *ft_lstlast(t_node *head)
// {
//   if(!head)
//     return NULL;
//   while (head->next != NULL)
//     head = head->next;  
//   return head;
// }

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


void    ft_lstclear(t_cmd **lst)
{
	t_cmd  *current;

	if (!lst)
		return ;
	while (*lst != NULL)
	{
		current = (*lst)->next;
		ft_free((*lst)->args);
		free((*lst)->cmd);
		free((*lst)->in_file);
		free((*lst)->out_file);
		free((*lst)->heredoc_content);
		free((*lst)->heredoc_delimiter);
		free((*lst)->full_path);
		free((*lst)->append_file);
		free(*lst);
		*lst = NULL;
		*lst = current;
	}
}
