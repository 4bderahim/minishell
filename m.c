
#include "mini.h"
//
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

int main(int argc, char **argv)
{
    int x[2];
    int t;
    char *line;
    char buffer[100];
    
    char *envp[] = {NULL};

    pipe(x);
    t = fork();
    if (t< 0)
        printf("[-] the pipe isnt piping..");
    if (t == 0)
    {

        close(x[1]);
        char *envp[] = {NULL};
        dup2(x[0],STDIN_FILENO);            
        char *ls_args[] = {"grep", "fi", NULL};
        execve("/usr/bin/grep", ls_args, envp );
        close(x[0]);  
    }
    else
    {
        close(x[0]);
        char *g_args[] = {"ls", "-la", NULL};
        char *envp[] = {NULL};
        dup2(x[1], STDOUT_FILENO);
        execve("/bin/ls", g_args, envp);
        close(x[1]);
        
    }

    printf("Hellow orld");
    // t = fork();
    //redirect(argv[2], argv[1]);
    // if (t == 0) 
    // {
    //     printf("to be output:%d | to be input:%d\n", x[0], x[1]);
    // } 
    
    //printf("%s", line);
    return (0);
}
















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
