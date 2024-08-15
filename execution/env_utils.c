#include "minishell.h"

t_env *env_new(char *new_line)
{
    t_env *new;
    int index;
    index = spliter_index(new_line);
    new = (t_env *) malloc(sizeof(t_env));
    if (!new)
        return (NULL);
    new->variable = ft_strdup(new_line);
    if (*(new_line+index))
        new->value = ft_strdup(new_line+index);
    else
        new->value = NULL;
    new->next = NULL;
    new->prev = NULL;
    
    return (new);
}

t_env *env_getlast(t_env *env)
{
    t_env *tmp;
    tmp = env;

    if (env == NULL)
        return NULL;
    if (env->next == NULL)
        return (env);
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    return (tmp);
}

void env_addback(t_env *head, t_env *new)
{
    t_env *tmp;
    if (head == NULL)
        head = new;
    tmp = env_getlast(head);
    tmp->next = new;
    new->prev = tmp;
}

t_env *create_env_list(char **envp)
{
    int i;
    t_env *head;
    t_env *new;
    i = 1;
    
    head = env_new(envp[0]);
    while (envp[i] != NULL)
    {
        new = env_new(envp[i]);
        if (new == NULL)
            {
                free(envp);
                return (NULL);
            }
        env_addback(head,new);
        i++;
    }
    
    return (head);
}
