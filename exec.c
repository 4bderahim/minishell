#include "mini.h"



int main(int argc, char **argv)
{
    int t;
    char *line;
    //usepipe();
    int x[2];
    int pr_fd;
    char *envp[] = {NULL};
    int dd;
    
    int n_pipes = 2;
    int j = 1;
    int i = 0;
    int s = 0;

    pid_t pids[n_pipes];  

        
    
    i = 0;
    while (i < n_pipes)
    {   
        pipe(x);
        pids[i] = fork();
        if (pids[i] == 0)
        {
           if (i == 0)
           {
             int fdfd = open(argv[j+2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
             if (fdfd == -1)
                {
                    write(2, "\t\t######\n", 8);
                    return 0;
                }
             dup2(fdfd, STDOUT_FILENO);
             close(fdfd);
             dup2(pr_fd, STDIN_FILENO);
             
             char *ls_args[] = {argv[j+1], argv[j+2], NULL};
             execve(argv[j], ls_args, NULL);
           }
           else
           {
                char *envp[] = {NULL};
                // redirect
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
                    char *ls_args[] = {argv[j+1], argv[j+2], NULL};
                    execve(argv[j], ls_args, NULL);
           }
        }
        if (i !=0 )
            close(pr_fd);
        pr_fd = dup(x[0]);// pr_fd
        close(x[1]);
        close(x[0]);
       i++;
       j += 3;
    }
    close(pr_fd);
     
    for (i = 0; i < n_pipes; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
    }
    return (0);
}

