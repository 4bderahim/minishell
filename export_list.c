#include "mini.h"

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

t_exp *exp_new(char *new_line)
{
    t_exp *new;
    int i;
    i = 0;
    while(new_line[i])
    {
        if (new_line[i] == '=')
            {
                new_line[i] = '\0';
                i++;
                break;
            }
        i++;
    }
    new = (t_exp *) malloc(sizeof(t_exp));
    if (!new)
        exit(1);
    
    new->variable = strdup(new_line);
    if (*(new_line+i))
        new->value = strdup(new_line+i);
    new->next = NULL;
    return (new);
}

void exp_addback(t_exp    *head, t_exp    *new)
{
    t_exp *tmp;
    if (head == NULL)
        {
            if (new == NULL)
                return ;
            head = new;
            return ;
        }
    tmp = env_getlast(head);
    tmp->next = new;
    //new->prev = tmp;
}

void set_export_list(t_all *all, char **env)
{
    while (env)
    {
        exp_addback(all->exp, exp_new(*env) );
        env++;
    }
}