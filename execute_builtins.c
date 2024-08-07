

#include "mini.h"

void print_exp_list(t_all *all, int pipe[2])
{
    t_exp *tmp;
    tmp = all->exp;
    int i;
    i = 0;
    char *str;
    while (tmp != NULL)
    {
        ft_write(tmp->variable, STDOUT_FILENO);
        ft_write("=", STDOUT_FILENO);
        ft_write("\"", STDOUT_FILENO);
        ft_write(tmp->value, STDOUT_FILENO);
        ft_write("\"", STDOUT_FILENO);
        write(STDOUT_FILENO, "\n",STDOUT_FILENO);
        tmp = tmp->next;
        i++;
    }
}
void print_env_list(t_all *all)
{
    t_env *tmp;
    tmp = all->env;
    int i;
    i = 0;
    
    while (tmp != NULL) 
    {
        ft_write(tmp->variable, STDOUT_FILENO);
        write(STDOUT_FILENO, "=", 1);
        ft_write(tmp->value, STDOUT_FILENO);
        ft_write("\n", STDOUT_FILENO);
        tmp = tmp->next;
        i++;
    }
    
}



void exec_piped_built_ins(t_all *all, int pipes[2])
{
    int i;
    char *str;

    i = 0;
    if (match_word(all->cmd->cmd, "echo"))
        {
            // need to loop and echo all args
            if (all->cmd->pipe == 1)
                ft_echo(all->cmd->args+1, STDOUT_FILENO);// need to loop and echo all args
            
            exit(0);
        }
    if (match_word(all->cmd->cmd, "env"))
        print_env_list(all);
    if (match_word(all->cmd->cmd, "export") && all->cmd->args[1] == NULL)
        print_exp_list(all, pipes);
    else if (match_word(all->cmd->cmd, "pwd"))
        ft_pwd(all);
    else
        return ;
    //char *ls_args[] = {all->cmd->cmd,all->cmd->args[1], NULL};

    //execve("/bin/ls",ls_args , NULL);

    exit(0);
}