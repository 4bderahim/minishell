#include "minishell.h"

char ** n_env(t_all *all, char **env)
{
    int i;
    char **envp;
    
    i = 0;
    while (env[i])
        i++
        ;
    envp = (char **) malloc(sizeof(char *) * (i+1));
    if (!envp)
       {
        ft_lstclear(&all->cmd);
        exit(1);
       }
    envp[i]  = NULL;
    return (envp);
}
void set_lists(t_all *all, char **env)
{
    int i;
    char **envp;

    i = 0;
    envp = n_env(all, env);
    while (env[i])
    {
        envp[i] = ft_strdup(env[i]);
        i++;
    }
    all->env = create_env_list(envp);
    if (all->env == NULL)
        {
            ft_lstclear(&all->cmd);
            exit(1);
        }
    all->exp = set_export_list(all);
    if (all->exp == NULL)
        {
            free_env_list(all);
            ft_lstclear(&all->cmd);
            exit(1);
        }
    i = 0 ;
    while(envp[i])
        free(envp[i++])
        ;
}

void wait_ps(pid_t *pids, t_all *all)
{
    int i;

    i = 0;
    while (i < all->nums_of_cmds)
    {
        int status;
        waitpid(pids[i], &status, 0);
        i++;
    }

}
void execution(t_all **alll, char *envpp[])
{
    t_all *all;
    all = *alll;
    pid_t pids[all->nums_of_cmds];
    t_cmd *cmd_;
    int i;
    
    i = 0;

    cmd_ = all->cmd;
    int x[2];
    int pr_fd;
    heredoc_check(all);
    if (exec_built_ins(all))
        {
            all->nums_of_cmds--;
            all->cmd = all->cmd->next;
        }
    while (i < all->nums_of_cmds)
    {
        if (pipe(x) < 0)
            ft_error(all);
        pids[i] = fork();
        if (pids[i] < 0)
            ft_error(all);
        if (pids[i] == 0)
        {
            reset_signal_handlers();
            redirect_in_out_to_pipe(all->nums_of_cmds, i, x, &pr_fd, all);
            redirections_set(all);
            heredoc_pipe(all);
            exec_piped_built_ins(all, x);
            if (execve(all->cmd->full_path, all->cmd->args, envpp) == -1)
                ft_write(strerror(errno), 2);
            exit(1);
        }
        if (i != 0)
            close(pr_fd);
        pr_fd = dup(x[0]);
        if (pr_fd < 0)
            ft_error(all);
        close(x[1]);
        close(x[0]);
        i++;
        all->cmd = all->cmd->next;
    }
    close(pr_fd);
    wait_ps(pids, all);
    all = *alll;
    all->cmd = cmd_;

}



// pwd chnage pwd in list, change OLDPWD
// add last command executed
// signals checks,
// signals func lib set.
// 