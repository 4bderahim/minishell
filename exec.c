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
t_cmd* make(t_cmd *cmd, char **envp) {
    cmd = list_new(); // For "echo hello > file"
    t_cmd *cmd1 = list_new(); // For "grep < file.c"

    t_cmd *cmd2 = list_new(); // For "grep < file.c"
    t_cmd *cmd3 = list_new(); // For "ls"
    t_env *f = create_env_list(envp);
    // t_env *qq = f;
    
    // while (qq != NULL)
    // {
    //     write(2, qq->line, ft_strlen(qq->line));
    //     write(2, "\n", 1);
    //     qq = qq->next;
    // }
    
    
    cmd->cmd = strdup("export");
    cmd->full_path = strdup("/usr/bin/cd"); 
    cmd->args = (char **)malloc(sizeof(char *) * 3);
    cmd->arg_count = 1;
    cmd->args[0] = strdup("export");
    //cmd->args[1] = strdup("dd");
    cmd->args[1] = NULL;
    // cmd->args[3] = strdup("me\"=ggg");
    // cmd->args[4] = strdup("mess=aag");
    cmd->in_file = NULL;
    cmd->out_file = NULL;//"rett";
    cmd->append_file = NULL;
    cmd->heredoc_delimiter = NULL;//"s";
    cmd->heredoc_content = NULL;
    cmd->pipe = 0; 
    cmd->env = f;

    // Command 1: echo hello > file
    cmd1->cmd = strdup("ls");
    cmd1->full_path = strdup("/bin/ls"); 
    cmd1->args = (char **)malloc(sizeof(char *) * 3);
    cmd1->arg_count = 2;
    cmd1->args[0] = strdup("ls");
    cmd1->args[1] = strdup("-la");// strdup("2");
    cmd1->args[2] = NULL;//strdup("\\b\\w{3,}\\b");
    //cmd1->args[3] = NULL;//strdup("NUsdLL");
   // cmd1->args[4] = NULL;
    cmd1->env = f;
    cmd1->out_file = NULL;//strdup("doneee"); //strdup("file");
   // cmd1->in_file =  "aa";// "rett";
    cmd1->append_file = NULL;
    cmd1->heredoc_delimiter = NULL;
    cmd1->heredoc_content = NULL;
    cmd1->pipe = 0; // Pipes to next command

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
    cmd->next = cmd1;
    cmd1->next = cmd3;
    cmd3->next = NULL;

    // 
    
    return cmd;
}
void redirections_set(t_cmd *cmd) 
{
    int fd;
    if (cmd->in_file) {
        fd = open(cmd->in_file, O_RDONLY);
        if (fd == -1) 
            exit(1);
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    if (cmd->out_file) {
        if (cmd->append_file)
            fd = open(cmd->out_file,O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd = open(cmd->out_file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
            exit(1);
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}
void heredoc_pipe(t_cmd *cmd)
{
    int fd;
    int p[2];
    if (cmd->heredoc_delimiter == NULL)
        return ;
    if (pipe(p)<0)
        exit(1);
    fd = fork();
    if (fd == -1)
        exit(1);
    if (fd == 0)
    {
        close(p[0]);
        redirections_set(cmd);
        //cmd->heredoc_content = heredoc(cmd->heredoc_delimiter, 1);
        write(p[1], cmd->heredoc_content, ft_strlen(cmd->heredoc_content));
        close(p[1]);
        exit(1);
    }
    close(p[1]);
    dup2(p[0], STDIN_FILENO);
    close(p[0]);
    //int status;
    //waitpid(getpid(), &status, 0);
}

void print_env_list(t_cmd *cmd)
{
    t_env *tmp;
    tmp = cmd->env;
    int i;
    i = 0;
    while (tmp != NULL)
    {
        if (cmd->pipe)  
            {
                ft_write(tmp->variable, STDIN_FILENO);
                write(STDIN_FILENO, "=", 1);
                ft_write(tmp->value, STDIN_FILENO);
            }
        else
        {
            ft_write(tmp->variable, STDOUT_FILENO);
            write(STDOUT_FILENO, "=", 1);
            ft_write(tmp->value, STDOUT_FILENO);
        }
        write(1, "\n", 1);
        tmp = tmp->next;
        i++;
    }
}
void exec_piped_built_ins(t_cmd *cmd)
{
    int i;
    char *str;

    i = 0;
    if (match_word(cmd->cmd, "echo"))
        {
            if (cmd->pipe == 1)
                ft_echo(cmd->args+1, STDOUT_FILENO);
            else
                ft_echo(cmd->args+1, STDOUT_FILENO);
            exit(0);
        }
    if (match_word(cmd->cmd, "env"))
        print_env_list(cmd);
    if (match_word(cmd->cmd, "export") && cmd->args[1] == NULL)
        print_env_list(cmd);
    else if (match_word(cmd->cmd, "pwd"))
        ft_pwd(cmd);
    else
        return ;
    //char *ls_args[] = {cmd->cmd,cmd->args[1], NULL};

    //execve("/bin/ls",ls_args , NULL);

    exit(0);
}

void signal_handler(int signo) {
    
    
    if (signo == SIGINT)
        {
            printf("\n");
            rl_on_new_line();
           // rl_replace_line("", 0);// fix compiling 
            rl_redisplay();
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
}
extern char **environ;


void unset_it(t_cmd *cmd, char *var)
{
    t_env *env;
    env = cmd->env;
    while (env != NULL)
    {
        if (match_word(var, env->variable))
            {
                if (env->prev == NULL)
                {
                    if (env->next != NULL)
                        cmd->env = env;
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
void unset_env(t_cmd *cmd)
{
    t_env *env;
    int i;
    
    i = 1;
    while (cmd->args[i])
    {
       env = cmd->env;
       while (env != NULL)
       {
            if (match_word(cmd->args[i], env->variable))
                {
                    unset_it(cmd , env->variable);
                    break;
                }
            env = env->next;
       }
       if (env == NULL)
            identifier_error(cmd->args[i]);
       i++;
    }
}
int exec_built_ins(t_cmd *cmd)
{
    int i;
    int exec;

    exec = 0;
    i = 0;

    if (match_word(cmd->cmd, "export") && cmd->args[1] != NULL)
    {
        // print the exported vars,, (list to be made later)
        while (cmd->args[i])
        {
            parse_indetifier(cmd, cmd->args[i]);
            i++;
        }
        exec++;
    }
    if (match_word(cmd->cmd, "unset")) 
    {
        if (cmd->args[1] != NULL)
            unset_env(cmd);
        exec++;
    }  

    if (match_word(cmd->cmd, "exit") && i == 0)
        {
            exit(0);
            exec++;
        }
    if (match_word(cmd->cmd, "cd") )
        {
            if (!cmd->pipe)
                change_dir(cmd, cmd->args[1]);
            cmd = cmd->next ;
            exec++;
        }
    // should we skip all these command by increasing i
    if (exec)
        return (1);
    return (0);
}
void heredoc_check(t_cmd *cmd)
{
    t_cmd *doc;
    doc = cmd;
    while (doc != NULL)
    {
        if (doc->heredoc_delimiter != NULL)
            {
                doc->heredoc_content = heredoc(doc->heredoc_delimiter, 1);
            }
        doc = doc->next;
    }
}
int main(int argc, char **argv, char *envp[])
{
    int t;
    char *line;
    t_cmd *cmd, *f;
    cmd = make(cmd, envp);
   // t_env *ff = create_env_list(envp);
    

    //usepipe();
    int x[2];
    int pr_fd;
   // char *envp[] = {NULL};
    int dd;
    int n_pipes = 1;
    int j = 1;
    int i = 0;
    int s = 0;
    setup_signal_handlers();
    heredoc_check(cmd);
   
    if (exec_built_ins(cmd))
        {
            i++;
            //cmd = cmd->next;
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
            if (i != 0)
                {
                    dup2(pr_fd, STDIN_FILENO);  
                    close(pr_fd);
                }
            if (i < n_pipes -1)
                {
                    dup2(x[1],STDOUT_FILENO);
                    close(x[1]);
                }
            redirections_set(cmd);
            heredoc_pipe(cmd);
            char *ls_args[] = {cmd->cmd,cmd->args[1], NULL};
            exec_piped_built_ins(cmd); // not completed!

            if (execve(cmd->full_path, cmd->args, NULL) == -1)
                write_fd(strerror(errno), 2);
            //exit(1);
        }
        if (i !=0 )
            close(pr_fd);
        pr_fd = dup(x[0]);// pr_fd
        close(x[1]);
        close(x[0]);
       i++;
       cmd = cmd->next ;
       
    }
    close(pr_fd);
     
    for (i = 0; i < n_pipes; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
    }
    

    return (0);
}



