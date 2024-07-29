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

    
    cmd->cmd = strdup("cat");
    cmd->full_path = strdup("/bin/cat"); 
    cmd->args = (char **)malloc(sizeof(char *) * 2);
    cmd->arg_count = 1;
    cmd->args[0] = strdup("cat");
    cmd->args[1] = NULL;//strdup("-aa");
    cmd->in_file = NULL;
    cmd->out_file = "rett";
    cmd->append_file = NULL;
   // cmd->heredoc_delimiter = "s";
    cmd->heredoc_content = NULL;
    cmd->pipe = 1; 
    cmd->env = f;

    // Command 1: echo hello > file
    cmd1->cmd = strdup("grep");
    cmd1->full_path = strdup("/usr/bin/grep"); 
    cmd1->args = (char **)malloc(sizeof(char *) * 3);
    cmd1->arg_count = 2;
    cmd1->args[0] = strdup("grep");
    cmd1->args[1] = strdup("-oE" );// strdup("2");
    cmd1->args[2] = strdup("\\b\\w{3,}\\b");
    cmd1->args[3] = NULL;//strdup("NUsdLL");
    cmd1->args[4] = NULL;
    cmd1->env = f;
    cmd1->out_file = NULL;//strdup("doneee"); //strdup("file");
    cmd1->in_file = "rett";
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


int count_quots(char *str)
{
    int i;
    int s_count;
    int d_count;

    s_count = 0;
    d_count = 0;
    i = 0;
    while (str[i])
    {
        if (str[i] == "\'")
            s_count++;
        if (str[i] == "\"")
            d_count++;
        i++;
    }
    if (s_count%2 || d_count%2)
        return (-1);
    return (1);
}
char *parse_export_args(t_cmd *args)
{
    char *full;
    int i;

    full = (char *)malloc(1);
    if (full == NULL)
        exit(1);
    full[0] = 0;
    i = 0;
    while (args->args[i])
    {
        full = ft_strjoin(full, args->args[i]);
        i++;
    }
    return (full);

}
// "command"
// export > "a"='b' 
void exec_built_ins(t_cmd *cmd)
{
    t_env *env;
    int i;
    char *str;

    i = 0;
    if (match_word(cmd->cmd, "echo"))
        {
            if (cmd->pipe == 1)
                ft_echo(cmd->args+1 ,STDOUT_FILENO);
            else
                ft_echo(cmd->args+1 ,STDOUT_FILENO);
            exit(0);
        }
    if (match_word(cmd->cmd, "export"))
    {
        if (cmd->args[0] == NULL)
            exit(0);
        if (cmd->args[1] != NULL)
            str = parse_export_args(cmd);   
        else
            str = cmd->args[0];
        while (str[i])
        {
            if (str[i]= '\"')
                {
                    write(2, "not a valid identifier", 22);
                    exit(1);
                }
            if (count_quots(str) == -1)
                exit(1);
            i++;
        }

        i = 0;
        while (cmd->args[i])
        {
            env = env_getlast(cmd->env);
            if (env == NULL)
                return ;
            env->next = env_new(cmd->cmd);
            i++;
        }
    }
    
}

int main(int argc, char **argv, char **envp)
{
    int t;
    char *line;
    t_cmd *cmd, *f;
    cmd = make(cmd, envp);
    //usepipe();
    int x[2];
    int pr_fd;
   // char *envp[] = {NULL};
    int dd;
    
    int n_pipes = 3;
    int j = 1;
    int i = 0;
    int s = 0;
    f = cmd;
    while (f != NULL)
    {
        if (f->heredoc_delimiter != NULL)
            {
                f->heredoc_content = heredoc(f->heredoc_delimiter, 1);
            }
        f = f->next;
    }
    pid_t pids[n_pipes];  
    i = 0;
    while (i < n_pipes)
    {   
        if (match_word(cmd->cmd, "exit") && i == 0)
            exit(0);
        pipe(x);
        pids[i] = fork();
        if (pids[i] == 0)
        {
            char *envp[] = {NULL};
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
            //exec_built_ins(cmd); not completed!
           
            if (execve(cmd->full_path, cmd->args, NULL) == -1)
                write_fd(strerror(errno), 2);
            exit(1);
        }
        if (i !=0 )
            close(pr_fd);
        pr_fd = dup(x[0]);// pr_fd
        close(x[1]);
        close(x[0]);
       i++;
       j += 3;
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



