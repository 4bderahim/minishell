#include "minishell.h"

// void ft_init(t_shell *shell)
// {
//   shell = malloc(sizeof(t_shell));
//   shell->head = NULL;
//   shell->size = 0;
//   shell->state = NORMAL;
// }

static int is_redirection(char *str, char *next)
{
	if(!ft_strcmp(str, ">") && ft_strcmp(next, ">"))
		return 2;
	else if(!ft_strcmp(str, ">") && !ft_strcmp(next, ">"))
		return 3;
	else if(!ft_strcmp(str, "<") && ft_strcmp(next, "<"))
		return 2;
	else if(!ft_strcmp(str, "<") && !ft_strcmp(next, "<"))
		return 3;
	else
		return 0;
}

static size_t count_valid_args(char **args)
{
	size_t i;
	size_t counter;

	i = 0;
	counter = 0;
	while(args[i])
	{
		while(args[i] && is_redirection(args[i], args[i + 1]))
			i += is_redirection(args[i], args[i + 1]);
		if(!args[i])
			break;
		counter++;
		i++;
	}
	return counter;
}

char **ft_args_dup(char **args)
{
	int i;
	int j;
	char **result;
	char *arg;

	i = 0;
	j = 0;
	arg = NULL;
	if(!count_valid_args(args))
		return NULL;
	result = malloc(sizeof(char *) * (count_valid_args(args) + 1));
	if(!result)
		return NULL;
	while (args[i])
	{
		while(args[i] && is_redirection(args[i], args[i + 1]))
			i += is_redirection(args[i], args[i + 1]);
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
	if(cmd->args)
		ft_free(cmd->args);
	if(cmd->cmd)
		free(cmd->cmd);
	if(cmd->in_file)
		free(cmd->in_file);
	if(cmd->out_file)
		free(cmd->out_file);
	if(cmd->heredoc_content)
		free(cmd->heredoc_content);
	if(cmd->heredoc_delimiter)
		free(cmd->heredoc_delimiter);
	if(cmd->full_path)
		free(cmd->full_path);
	if(cmd->append_file)
		free(cmd->append_file);
	// if(cmd)
	// free(cmd);
	// cmd = NULL;
}

bool is_builtin(char *cmd)
{
	if(!ft_strcmp(cmd, "export"))
		return true;
	else if(!ft_strcmp(cmd, "unset"))
		return true;
	// else if(!ft_strcmp(cmd, "unset"))
	// 	return true;
	// else if(!ft_strcmp(cmd, "unset"))
	// 	return true;
	return false;
}

// bool is_executable(char *cmd)
// {

// }

t_cmd	*ft_lstnew(t_all **all, char **args, int args_nbr, int pipe)
{
	t_cmd	*new_node;

	// if(!check_args(args))
		// throw_error("Something went wrong with arguments");
	new_node = malloc(sizeof(t_cmd));
	if (!new_node)
		return (NULL);
	new_node->arg_count = args_nbr;
	new_node->args = ft_args_dup(args);
	// get_executable(new_node->cmd, new_node->args);
	new_node->in_file = get_input_redirection_file(args, *all);
	new_node->out_file = get_output_redirection_file(args, *all);
	new_node->append_file = get_append_to_file(args, *all);
	new_node->heredoc_delimiter = get_herdoc_delimiter(args, *all);
	new_node->heredoc_content = NULL;
	new_node->pipe = pipe;
	new_node->next = NULL;
	new_node->cmd = NULL;
	new_node->cmd_not_found = false;
	if(!new_node->args)
		ft_free(new_node->args);
	else
		new_node->cmd = ft_strdup(new_node->args[0]);
	new_node->full_path = get_path(new_node->cmd);
	if(!new_node->full_path)
	{
		new_node->full_path = get_executable(new_node->cmd);
		if(!new_node->full_path)
			new_node->cmd_not_found = 1;
	}
	if((*all)->error)
		return NULL;
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
		if((*lst)->args)
			ft_free((*lst)->args);
		if((*lst)->cmd != NULL)
			free((*lst)->cmd);
		free((*lst)->in_file);
		free((*lst)->out_file);
		if((*lst)->heredoc_delimiter)
			ft_free((*lst)->heredoc_delimiter);
		free((*lst)->heredoc_content);
		free((*lst)->full_path);
		free((*lst)->append_file);		
		free(*lst);
		*lst = NULL;
		*lst = current;
	}
}
