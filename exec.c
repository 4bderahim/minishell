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
t_cmd* make(t_cmd *cmd) {
    cmd = list_new(); // For "echo hello > file"
    t_cmd *cmd2 = list_new(); // For "grep < file.c"
    t_cmd *cmd3 = list_new(); // For "ls"


    // Command 3: ls
    // cmd->cmd = strdup("sleep");
    // cmd->full_path = strdup("/bin/sleep"); 
    // cmd->args = (char **)malloc(sizeof(char *) * 2);
    // cmd->arg_count = 1;
    // cmd->args[0] = strdup("sleep");
    // cmd->args[1] = strdup("3");
    // cmd->in_file = NULL;
    // cmd->out_file = NULL;
    // cmd->append_file = NULL;
    // cmd->heredoc_delimiter = NULL;
    // cmd->heredoc_content = NULL;
    // cmd->pipe = 0; 

    // Command 1: echo hello > file
    cmd->cmd = strdup("cat");
    cmd->full_path = strdup("/bin/cat"); 
    cmd->args = (char **)malloc(sizeof(char *) * 3);
    cmd->arg_count = 2;
    cmd->args[0] = strdup("cat");
    cmd->args[1] = NULL;// strdup("2");
    cmd->args[2] = NULL;
    cmd->out_file = NULL;//strdup("file");
    cmd->in_file = "file";
    cmd->append_file = NULL;
    cmd->heredoc_delimiter = "<";
    cmd->heredoc_content = NULL;
    cmd->pipe = 1; // Pipes to next command

    // Command 2: grep < file.c
    cmd3->cmd = strdup("grep");
    cmd3->full_path = strdup("/usr/bin/grep"); 
    cmd3->args = (char **)malloc(sizeof(char *) * 2);
    cmd3->arg_count = 1;
    cmd3->args[0] = strdup("grep");
    cmd3->args[1] = strdup("aa");
    cmd3->in_file = NULL;//strdup("exec.c");
    cmd3->out_file = NULL;
    cmd3->append_file = NULL;
    cmd3->heredoc_delimiter = NULL;
    cmd3->heredoc_content = NULL;
    cmd3->pipe = 1; // Pipes to next command

    

    cmd->next = cmd3;
    //cmd2->next = cmd3;
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

//    if (i == 0)
        //    {
        //      int fdfd = open(argv[j+2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        //      if (fdfd == -1) 
        //         {
        //             return 0;
        //         }
        //      dup2(fdfd, STDOUT_FILENO);
        //      close(fdfd);
        //      dup2(pr_fd, STDIN_FILENO);
             
        //      char *ls_args[] = {argv[j+1], argv[j+2], NULL};
        //      execve(argv[j], ls_args, NULL);
        //    }
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
int main(int argc, char **argv)
{
    int t;
    char *line;
    t_cmd *cmd, *f;
    cmd = make(cmd);
    //usepipe();
    int x[2];
    int pr_fd;
    char *envp[] = {NULL};
    int dd;
    
    int n_pipes = 1;
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
            execve(cmd->full_path, ls_args, NULL);
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






// #include "mini.h"



// int main(int argc, char **argv)
// {
//     int t;
//     char *line;
//     //usepipe();
//     int x[2];
//     int pr_fd;
//     char *envp[] = {NULL};
//     int dd;
    
//     int n_pipes = 2;
//     int j = 1;
//     int i = 0;
//     int s = 0;

//     pid_t pids[n_pipes];  

        
//     i = 0;
//     while (i < n_pipes)
//     {   
//         pipe(x);
//         pids[i] = fork();
//         if (pids[i] == 0)
//         {
//            if (i == 0)
//            {
//              int fdfd = open(argv[j+2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
//              if (fdfd == -1) 
//                 {
//                     return 0;
//                 }
//              dup2(fdfd, STDOUT_FILENO);
//              close(fdfd);
//              dup2(pr_fd, STDIN_FILENO);
             
//              char *ls_args[] = {argv[j+1], argv[j+2], NULL};
//              execve(argv[j], ls_args, NULL);
//            }
//            else
//            {
//                 char *envp[] = {NULL};
//                 // redirect
//                 if (i != 0)
//                     {
//                         dup2(pr_fd, STDIN_FILENO);  
//                         close(pr_fd);
//                     }
//                 if (i < n_pipes -1)
//                     {
//                         dup2(x[1],STDOUT_FILENO);
//                         close(x[1]);
//                     }
//                     char *ls_args[] = {argv[j+1], argv[j+2], NULL};
//                     execve(argv[j], ls_args, NULL);
//            }
//         }
//         if (i !=0 )
//             close(pr_fd);
//         pr_fd = dup(x[0]);// pr_fd
//         close(x[1]);
//         close(x[0]);
//        i++;
//        j += 3;
//     }
//     close(pr_fd);
     
//     for (i = 0; i < n_pipes; i++)
//     {
//         int status;
//         waitpid(pids[i], &status, 0);
//     }
//     return (0);
// }


