#include "minishell.h"

void unset_exp_list(t_all *all, char *var)
{
    t_exp *exp;
    exp = all->exp;
    while (exp != NULL)
    {
        if (match_word(var, exp->variable))
            {
                if (exp->prev == NULL)
                {
                    if (exp->next != NULL)
                        all->exp = exp;
                }
                else if (exp->next == NULL)
                    exp->prev->next = NULL;
                else
                {
                    exp->prev->next = exp->next;
                    exp->next->prev = exp->prev;
                }
                free(exp);
                break;
            }
        exp = exp->next;
    }
}
void unset_exp(t_all *all)
{
    t_exp *exp;
    int i;
    
    i = 1;
    while (all->cmd->args[i])
    {
       exp = all->exp;
       while (exp != NULL)
       {
            if (match_word(all->cmd->args[i], exp->variable))
                {
                    if (exp->value != NULL)
                        unset_exp_list(all , exp->variable);
                    break;
                }
            exp = exp->next;
       }
       i++;
    }
}

void unset_env_list(t_all *all, char *var)
{
    t_env *env;
    env = all->env;
    while (env != NULL)
    {
        if (match_word(var, env->variable))
            {
                if (env->prev == NULL)
                {
                    if (env->next != NULL)
                        all->env = env;
                }
                else if (env->next == NULL)
                    env->prev->next = NULL;
                else
                {
                    env->prev->next = env->next;
                    env->next->prev = env->prev;
                }
                free(env);
                break;
            }
        env = env->next;
    }
}
void unset_env(t_all *all)
{
    t_env *env;
    int i;
    
    i = 1;
    while (all->cmd->args[i])
    {
       env = all->env;
       while (env != NULL)
       {
            if (match_word(all->cmd->args[i], env->variable))
                {
                    unset_env_list(all , env->variable);
                    break;
                }
            env = env->next;
       }
       i++;
    }
}

