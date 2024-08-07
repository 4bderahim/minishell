#include "mini.h"

t_cmd *list_new() {
    t_cmd *new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
    if (new_cmd == NULL) {
        return NULL; // Allocation failed
    }
    
    // Initialize the fields with default values
    // new_cmd->cmd = NULL;
    // new_cmd->full_path = NULL;
    // new_cmd->args = NULL;
    // new_cmd->arg_count = 0;
    // new_cmd->in_file = NULL;
    // new_cmd->out_file = NULL;
    // new_cmd->append_file = NULL;
    // new_cmd->heredoc_delimiter = NULL;
    // new_cmd->heredoc_content = NULL;
    // new_cmd->pipe = 0;
    // new_cmd->next = NULL;
    
    return new_cmd;
}
t_cmd* make(t_all *all, char **envp) {
    t_cmd *cmd = list_new(); // For "echo hello > file"
    t_cmd *cmd1 = list_new(); // For "grep < file.c"
    t_cmd *cmd2 = list_new(); // For "grep < file.c"
    t_cmd *cmd3 = list_new(); // For "ls"
    t_env *f;
    // = create_env_list(envp);
    // t_env *qq = f;
    
    // while (qq != NULL)
    // {
    //     write(2, qq->line, ft_strlen(qq->line));
    //     write(2, "\n", 1);
    //     qq = qq->next;
    // }
    
    
    cmd->cmd = strdup("env");
    cmd->full_path = strdup("/bin/echo"); 
    cmd->args = (char **)malloc(sizeof(char *) * 3);
    cmd->arg_count = 1;
    //cmd->args[0] = strdup("echo");
    cmd->args[0] = "env";//strdup("aaaa");
   // cmd->args[1] = strdup("helol\nnoen");
    //cmd->args[1] = strdup("dd");
    cmd->args[1] = NULL;//strdup("__a=helloworld");

    // cmd->args[3] = strdup("me\"=ggg");
    // cmd->args[4] = strdup("mess=aag");
    cmd->in_file = NULL;
    cmd->out_file = NULL;//"rett";
    cmd->append_file = NULL;
    cmd->heredoc_delimiter = NULL;//"s";
    cmd->heredoc_content = NULL;
    cmd->pipe = 1; 
    all->env = f;

    // Command 1: echo hello > file
    cmd2->cmd = strdup("grep");
    cmd2->full_path = strdup("/usr/bin/grep"); 
    cmd2->args = (char **)malloc(sizeof(char *) * 3);
    cmd2->arg_count = 2;
    cmd2->args[0] = strdup("grep");
    cmd2->args[1] = strdup("Zone");// strdup("2");
    cmd2->args[2] = NULL;// strdup("2");

    //cmd2->args[1] = NULL;//strdup("\\b\\w{3,}\\b");
    cmd2->env = f;
    cmd2->in_file = NULL;
    cmd2->out_file = NULL;//strdup("doneee"); //strdup("file");
    cmd2->append_file = NULL;
    cmd2->heredoc_delimiter = NULL;
    cmd2->heredoc_content = NULL;
    cmd2->pipe = 0; // Pipes to next command

    // Command 2: grep < file.c
    cmd3->cmd = strdup("sort");
    cmd3->full_path = strdup("/usr/bin/sort"); 
    cmd3->args = (char **)malloc(sizeof(char *) * 2);
    cmd3->arg_count = 1;
    cmd3->args[0] = strdup("sort");
    cmd3->args[1] = NULL;//strdup("N");
    cmd3->in_file = NULL;//strdup("exec.c");
    cmd3->out_file = NULL;
    cmd3->append_file = NULL;
    cmd3->heredoc_delimiter = NULL;
    cmd3->heredoc_content = NULL;
    cmd3->pipe = 0; // Pipes to next command
    cmd3->env = f;

    cmd->next = cmd2;
    cmd2->next = cmd3;
    cmd3->next = NULL;
    all->cmd = cmd;
    

    return all->cmd;
}
void redirections_set(t_all *all) 
{
    int fd;
    if (all->cmd->in_file) {
        fd = open(all->cmd->in_file, O_RDONLY);
        if (fd == -1) 
            exit(1);
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    if (all->cmd->out_file) {
        if (all->cmd->append_file)
            fd = open(all->cmd->out_file,O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd = open(all->cmd->out_file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
            exit(1);
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
        //all->cmd->heredoc_content = heredoc(all->heredoc_delimiter, 1);
        write(p[1], all->cmd->heredoc_content, ft_strlen(all->cmd->heredoc_content));
        close(p[1]);
        exit(1);
    }
    close(p[1]);
    dup2(p[0], STDIN_FILENO);
    close(p[0]);
    //int status;
    //waitpid(getpid(), &status, 0);
}
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

    //exit(0);
}

void signal_handler(int signo) {
    
    
    if (signo == SIGINT)
        {
            printf("\n");
            rl_on_new_line();
           // rl_replace_line("", 0);// fix compiling 
            //rl_redisplay();
            }
    if (signo == SIGQUIT)
        {
            printf("\n");
            // rl_on_new_line();
        }
}
void setup_signal_handlers() {
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
    // signal(SIGQUIT, SIG_DFL);
    // signal(SIGTSTP, SIG_DFL);
};





int exec_built_ins(t_all *all)
{
    int i;
    int exec;

    exec = 0;
    i = 1;

    if (match_word(all->cmd->cmd, "export") && all->cmd->args[1] != NULL)
    {
        // print the exported vars,, (list to be made later)
        while (all->cmd->args[i])
        {
            parse_indetifier(all, all->cmd->args[i]);
            i++;
        }
        exec++;
    }
    if (match_word(all->cmd->cmd, "unset")) 
    {
        if (all->cmd->args[1] != NULL)
            unset_env(all);
        exec++;
    }  

    if (match_word(all->cmd->cmd, "exit") && i == 0)
        {
            exit(0);
            exec++;
        }
    if (match_word(all->cmd->cmd, "cd") )
        {
            if (!all->cmd->pipe)
                change_dir(all, all->cmd->args[1]);
            //cmd = all->cmd->next ;
            exec++;
        }
    if (exec)
        return (1);
    return (0);
}


void set_lists(t_all *all, char **env)
{
    all->env = create_env_list(env);
    all->exp = set_export_list(all, env);
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
int main(int argc, char **argv, char *envp[])
{
    int t;
    char *line;
    t_cmd *f;
    t_all *all;
    int i = 0;

    all = (t_all *) malloc(sizeof(t_all));
    if (!all)
        exit(1);
    all->exp = NULL;
    
//    all->cmd = 
    
    make(all, envp);
    set_lists(all, envp);
   // t_env *ff = create_env_list(envp);
    

    //usepipe();
    int x[2];
    int pr_fd;
   // char *envp[] = {NULL};
    int dd;
    int n_pipes = 2;
    int j = 1;
    int s = 0;
    setup_signal_handlers();
    heredoc_check(all);
   
    if (exec_built_ins(all))
        {
            n_pipes--;
            all->cmd = all->cmd->next;
        }
            
    //i++;
    pid_t pids[n_pipes];  

    while (i < n_pipes)
    {
        //char *s = readline(">>");
        pipe(x);
        pids[i] = fork();
        if (pids[i] == 0)
        {
            reset_signal_handlers();
            //char *envp[] = {NULL};
            redirect_in_out_to_pipe(n_pipes, i, x, &pr_fd);
            
            redirections_set(all);
            heredoc_pipe(all);
            ///char *ls_args[] = {all->cmd->cmd,all->cmd->args[1], NULL};
            exec_piped_built_ins(all, x); // not completed!
            
            if (execve(all->cmd->full_path, all->cmd->args, NULL) == -1)
                ft_write(strerror(errno), 2);
            //exit(1);
        }
        if (i !=0 )
            close(pr_fd);
        pr_fd = dup(x[0]);// pr_fd
        close(x[1]);
        close(x[0]);
       i++;
       all->cmd = all->cmd->next ;
       
    }
    close(pr_fd);
     
    for (i = 0; i < n_pipes; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
    }
    

    return (0);
}



