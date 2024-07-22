
#include "mini.h"
#include <signal.h>
int match_word(char *neadle, char *str)
{
    int i;
    i = 0;
    while (str[i])
    {
        if (str[i] != neadle[i])
            return (0);
        i++;
    }
    return (1);
}
int redirect(char *file, char *input)
{
    int fd = open(file, O_WRONLY  | O_CREAT, 0644);
    if (!fd)
        return (0);
    if (dup2(fd , 1) == -1)
        return (0);
    write(fd, input, strlen(input));
    close(fd);
    return (1);
}
// int echo(int x)
// {
//     if
// }p_list *p_new(int val)
void p_addback(cmd_t **head, cmd_t *new)
{
    cmd_t *tmp;
    if (!head)
         return ;
    tmp = *head;
    if (!tmp)
        {
            *head = new;
            return ;
        }
    tmp->next = new;
}
cmd_t *p_new(char *cmd,char *flags)
{
    cmd_t *new;
    new = (cmd_t *) malloc(sizeof(cmd_t));
    new->command = cmd;
    new->flags = flags;
    new->next = NULL;
    return (new);
}
void usepipe()
{
    cmd_t *sec;
    cmd_t *thr;
    int fdd;
    cmd_t *head;
    head = p_new("/usr/bin/which", "ls");
    p_addback(&head, p_new("/usr/bin/grep","/"));
    p_addback(&head, p_new("/usr/bin/wc","-l"));
    
    int fd[head->pipes_n][2];
    head->pipes_n = 3;
    int i = 0;
    while (i < head->pipes_n)
    {
        if (pipe(fd[i]) == -1)
            printf("Hellooo");
        i++;
    }
    i = 0;
    int j = 0;
    while (i < head->pipes_n)
    {   
        int f = fork();
        if (f == -1)    
            printf("Hello from error");
        if (f == 0)
            {
                printf("%ds|%d\t%s<\n",i, head->pipes_n,  head->flags);
                if (i > 0)
                    dup2(fd[i-1][0],STDIN_FILENO);
                j = 0;
                while (j < head->pipes_n)
                    {
                        close(fd[j][0]);
                        close(fd[j][1]);
                    }
                if (i < head->pipes_n -1)
                    dup2(fd[i][1], STDOUT_FILENO);
                char *envp[] = {NULL};
                char *ls_args[] = {(head->command)+12, head->flags, NULL};
                execve(head->command, ls_args, envp );
                // if (i > 0)
                //     close(fd[i-1][0]);
            }

        i++;
        head = head->next;
    }
    
}

int main(int argc, char **argv)
{
    int t;
    char *line;
    //usepipe();
    //int x[2];
    int pr_fd;
    char *envp[] = {NULL};
    int dd;
    
    int n_pipes = 3;
    int j = 1;
    int i = 0;
    int s = 0;
    int x[n_pipes-1][2];
    pid_t pids[n_pipes];  

    while (i < n_pipes-1)
    {
        if (pipe(x[i]) == -1)
            printf("Pipe creation failed");   
        i++;
    }
    
    i = 0;
    while (i < n_pipes)
    {
        pids[i] = fork();
        if (pids[i] == 0)
        {
            char *envp[] = {NULL};
            // redirect
            if (i > 0)
                dup2(x[i-1][0], STDIN_FILENO);
            if (i < n_pipes -1 )
                dup2(x[i][1],STDOUT_FILENO);
            s = 0;
            while (s < n_pipes - 1)
            {
                close(x[s][1]);
                close(x[s][0]);
                s++;
            }
            char *ls_args[] = {argv[j+1], argv[j+2], NULL};
            execve(argv[j], ls_args, NULL);
            //pr_fd = x[i][0];
            exit(1);
        }
         //if (i == 0)
        
       i++;
       j += 3;
    }
     s=0;
    while (s < n_pipes -1)
        {
            close(x[s][1]);
            close(x[s][0]);
            s++;
        }
     for (i = 0; i < n_pipes; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
    }
    return (0);
}



// int t;
//     char *line;
//     char buffer[100];
    
//     //usepipe();
//     int x[2];
//     char *envp[] = {NULL};
//     pipe(x);
//     t = fork();
//     if (t < 0)
//         printf("[-] the pipe isnt piping..");
//     if (t == 0)
//     {
//         close(x[1]);
//         char *envp[] = {NULL};
//         dup2(x[0],STDIN_FILENO);
//         char *ls_args[] = {argv[5], argv[6], NULL};
//         execve(argv[4], ls_args, envp );
        
//     }
//     else
//     {
//         close(x[0]);
//         char *g_args[] = {argv[2], argv[3], NULL};
//         char *envp[] = {NULL};
//         dup2(x[1], STDOUT_FILENO);
//         execve(argv[1], g_args, envp);
//         close(x[1]);
//     }
    
















// while (1)
//         {
//             char *s;
//             int d;
//             char *history ;
//             s = readline("minishell>");
//             if (!empty_line(s))
//             {
//                 pars_echo(line);
//             }
//             char *args[] = {"env",  NULL};
//             char *path = "/usr/bin/env";
//             char *p[] = { NULL};
//             int pid = fork();
//             if (pid == -1)
//                 printf("error on child creation");
//             else if (pid == 0)
//             {

//                 close(1)    ;
//                 int fd = open("hello", O_CREAT | O_WRONLY, 0666);
                
//                 dup2(fd, 1);
//                 close(fd);
//                 int t = execve(path, args, p);
//             }
//             else
//                 wait(&d);
//             if (WIFEXITED(d))
//                 printf("\t|\n");
//             //rl_redisplay();
//             // rl_replace_line("", 0);
//             //add_history(history);
//         }



// int main(int argc, char **argv)
// {
//     int t;
//     char *line;
//     int n_pipes =3;
//     //usepipe();
//     //int x[2];
//     int pr_fd;
//     char *envp[] = {NULL};
//     int i,j, dd;
//     dd= getpid();
//     j = 1;
//     i = 0;
//     int x[n_pipes][2];
//      while (i < n_pipes-1)
//     {
//         if (pipe(x[i]) == -1)
//             printf("Hellooo");
//         i++;
//     }
//     i = 0;
//    while (i < n_pipes)
//     {
    
//         t = fork();
//         if (t == 0)
//         {
//             char *envp[] = {NULL};
//             //close(x[0]);
//             if (i != 0)
//                 dup2(x[i-1][0], STDIN_FILENO);
//             if (i == 0)
//                 close(x[i][1]);
//             if (i != n_pipes -1 )
//                 dup2(x[i][1],STDOUT_FILENO);
            
//             char *ls_args[] = {argv[j+1], argv[j+2], NULL};
//             execve(argv[j], ls_args, envp);
//             //pr_fd = x[i][0];   
//             //return (0);
//             close(x[i][0]);
//             exit(1);
//         }
//          //if (i == 0)
//          close(x[i][1]);
//        i++;
//        j += 3;
//     }
    
//     return (0);
// }
