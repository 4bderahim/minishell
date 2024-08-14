#include "minishell.h"

void redirections_set(t_all *all) 
{
    int fd;
    if (all->cmd->in_file)
    {
        fd = open(all->cmd->in_file, O_RDONLY);
        if (fd == -1) 
            ft_error(all);
        if (dup2(fd, STDIN_FILENO) < 0)
            ft_error(all);
        close(fd);
    }
    if (all->cmd->out_file || all->cmd->append_file) 
    {
        if (all->cmd->append_file)
            fd = open(all->cmd->out_file, O_APPEND, 0644);
        else
            fd = open(all->cmd->out_file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
            ft_error(all);
        if (dup2(fd, STDOUT_FILENO))
            ft_error(all);
        close(fd);
    }
}

void heredoc_pipe(t_all *all)
{
    int fd;
    int p[2];
    if (all->cmd->heredoc_delimiter == NULL)
        return ;
    if (pipe(p)<0)
        exit(1);
    fd = fork();
    if (fd == -1)
        exit(1);
    if (fd == 0)
    {
        close(p[0]);
        redirections_set(all);
        //all->cmd->heredoc_content = heredoc(all->cmd->heredoc_delimiter, 1);
        write(p[1], all->cmd->heredoc_content, ft_strlen(all->cmd->heredoc_content));
        close(p[1]);
        exit(1);
    }
    close(p[1]);
    if (dup2(p[0], STDIN_FILENO) < 0)
        ft_error(all);
    close(p[0]);
}




// int exec_built_ins(t_all *all)
// {
//     int i;
//     int exec;

//     exec = 0;
//     i = 1;
    
//     if (match_word(all->cmd->cmd, "export") && all->cmd->args[1] != NULL)
//     {
//         while (all->cmd->args[i])
//         {
//             parse_indetifier(all, all->cmd->args[i]);
//             i++;
//         }
//         exec++;
//     }
//     if (match_word(all->cmd->cmd, "unset")) 
//     {
//         if (all->cmd->args[1] != NULL)
//             unset_env(all);
//         exec++;
//     }  

//     if (match_word(all->cmd->cmd, "exit") && i == 0)
//         {
//             exit(0);
//             exec++;
//         }
//     if (match_word(all->cmd->cmd, "cd") )
//         {
//             if (!all->cmd->pipe)
//                 change_dir(all, all->cmd->args[1]);
//             exec++;
//         }
//     if (exec)
//         return (1);
//     return (0);
// }

void set_lists(t_all *all, char **env)
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
    i = 0;
    while (env[i])
    {
        envp[i] = strdup(env[i]);
        i++;
    }
    all->env = create_env_list(envp);
    if (all->env == NULL)
        {
            ft_lstclear(&all->cmd);
            exit(1);
        }
    all->exp = set_export_list(all, envp);
    if (all->exp == NULL)
        {
            free_env_list(all);
            ft_lstclear(&all->cmd);
            exit(1);
        }
}
void redirect_in_out_to_pipe(int n_pipes, int index, int pipe[],int *pr_fd, t_all *all)
{
    if (index != 0)
    {
        if (dup2(*pr_fd, STDIN_FILENO) < 0)
            ft_error(all);
        close(*pr_fd);
    }
    if (index < n_pipes -1)
    {
        if (dup2(pipe[1],STDOUT_FILENO) < 0)
            ft_error(all);
        close(pipe[1]);
    }
}

void execution(t_all **alll, char *envpp[])
{
    // int t;
    // char *line;
    
    // t_cmd *f;
    t_all *all;
    t_cmd *cmd_;
    // char *full;
    int i;
    
    i = 0;
    all = *alll;

    cmd_ = all->cmd;
    int x[2];
    int pr_fd;
    // int n_pipes = ;
   // setup_signal_handlers();
    
    heredoc_check(all);
    if (exec_built_ins(all))
        {
            all->nums_of_cmds--;
            all->cmd = all->cmd->next;
        }
    pid_t pids[all->nums_of_cmds];
    while (i < all->nums_of_cmds)
    {
        if (pipe(x) < 0)
            ft_error(all);
        pids[i] = fork();
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
    i = 0;
    while (i < all->nums_of_cmds)
    {
        int status;
        waitpid(pids[i], &status, 0);
        i++;
    }
    all = *alll;
    all->cmd = cmd_;
    
    
    //all->cmd->full_path = full;
    
    // return (0);
}



// pwd chnage pwd in list, change OLDPWD
// add last command executed
// signals checks,
// signals func lib set.
// 