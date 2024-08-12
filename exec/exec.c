#include "minishell.h"

void file_error()
{
    ft_write(strerror(errno), 2);
    write(2, "\n", 1);
    exit(1);
}
void redirections_set(t_all *all) 
{
    int fd;
    if (all->cmd->in_file)
    {
        fd = open(all->cmd->in_file, O_RDONLY);
        if (fd == -1) 
            exit(1);
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    if (all->cmd->out_file || all->cmd->append_file) 
    {
        if (all->cmd->append_file)
            fd = open(all->cmd->out_file, O_APPEND, 0644);
        else
            fd = open(all->cmd->out_file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
            file_error();
        dup2(fd, STDOUT_FILENO);
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
    dup2(p[0], STDIN_FILENO);
    close(p[0]);
}

void signal_handler(int signo) {
    if (signo == SIGINT)
        {
             printf("\n");
            rl_on_new_line();
             rl_replace_line("", 0);// fix compiling 
            rl_redisplay();
            }
    if (signo == SIGQUIT)
        {
            rl_redisplay();

            //rl_on_new_line();
            // rl_on_new_line();
        }
    if (signo == SIGTSTP)
        {
        
            rl_on_new_line();
            //exit(1);
            // rl_on_new_line();
        }
}

void setup_signal_handlers() 
{
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);

     sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}



void reset_signal_handlers() {
    signal(SIGINT, SIG_DFL);
     signal(SIGQUIT, SIG_DFL);
    // signal(SIGTSTP, SIG_DFL);
};


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
    envp = (char **) malloc(sizeof(char *) * i);
    if (!envp)
        exit(1);
    envp[i]  = NULL;
    
    i = 0;
    while (env[i])
    {
        envp[i] = strdup(env[i]);
        i++;
    }
    all->env = create_env_list(envp);
    all->exp = set_export_list(all, envp);
}
void redirect_in_out_to_pipe(int n_pipes, int index, int pipe[],int *pr_fd)
{
    if (index != 0)
    {
        dup2(*pr_fd, STDIN_FILENO);  
        close(*pr_fd);
    }
    if (index < n_pipes -1)
    {
        dup2(pipe[1],STDOUT_FILENO);
        close(pipe[1]);
    }
}

void execution(t_all **alll, char *envpp[])
{
    // int t;
    char *line;
    
    // t_cmd *f;
    t_all *all;
    t_cmd *cmd_;
    char *full;
    int i = 0;
    all = *alll;
    full = all->cmd->full_path;
    cmd_ = all->cmd;
    
   
//    all->cmd = 
   // make(all, envp);
    
   // t_env *ff = create_env_list(envp);
    //usepipe();
    int x[2];
    int pr_fd;
   // char *envp[] = {NULL};
    int dd;
    int n_pipes = all->nums_of_cmds;
    int j = 1;
    int s = 0;
   // setup_signal_handlers();

    heredoc_check(all);
    if (exec_built_ins(all))
        {
            n_pipes--;
            all->cmd = all->cmd->next;
        }
    pid_t pids[n_pipes];
    while (i < n_pipes)
    {
        pipe(x);
        pids[i] = fork();
        if (pids[i] == 0)
        {
            reset_signal_handlers();
            redirect_in_out_to_pipe(n_pipes, i, x, &pr_fd);
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
        close(x[1]);
        close(x[0]);
        i++;
        all->cmd = all->cmd->next;
    }
    close(pr_fd);
    for (i = 0; i < n_pipes; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
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