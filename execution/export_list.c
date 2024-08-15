#include "minishell.h"

t_exp *exp_getlast(t_exp *exp)
{
    t_exp *tmp;
    tmp = exp;

    if (exp == NULL)
        return NULL;
    if (exp->next == NULL)
        return (exp);
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    return (tmp);
}
int spliter_index(char *str)
{
    int i;
    i = 0;
    while(str[i])
    {
        if (str[i] == '=')
            {
                str[i] = '\0';
                i++;
                break;
            }
        i++;
    }
    return (i);
}
t_exp *exp_new(char *new_line)
{
    t_exp *new;
    int index;
    
    index = spliter_index(new_line);
    new = (t_exp *) malloc(sizeof(t_exp));
    if (!new)
        exit(1);
    new->variable = strdup(new_line);
    if (new_line[index] == '\0')
        new->value = NULL;
    else
        new->value = strdup(new_line+1);
    new->next = NULL;
    return (new);
}

void exp_addback(t_exp    *head, t_exp    *new)
{
    t_exp *tmp;
    // tmp = head;
    // if (head == NULL)
    //     {
    //         if (new == NULL)
    //             return ;
    //         head = new;
    //         return ;
    //     }
    tmp = exp_getlast(head);
    tmp->next = new;
    new->prev = tmp;
    //new->prev = tmp;
}
t_exp *new_exp_(t_env *env)
{
    t_exp *new;
    new = (t_exp *) malloc(sizeof(t_exp));
    if (!new)
        return (NULL);
    new->value = env->value;
    new->variable = env->variable;
    new->next = NULL;
    new->prev = NULL;
    // printf("%s=%s\n", new->value, new->variable);
    return (new);
}
t_exp *set_export_list(t_all *all)
{
    
    t_env *env;
    t_exp *exp;
    t_exp *new;

    
    env = all->env;
    exp = new_exp_(env);
    env = env->next;
    while (env != NULL)
    {
        new = new_exp_(env);
        if (new == NULL)
            return (NULL);
        exp_addback(exp , new);
        env = env->next;
    }
    return (exp);
}