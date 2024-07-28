#include "mini.h"

void ft_write(char *str, int fd)
{
    if (write(fd, str, ft_strlen(str)) == -1)
        {
            //error
            write(2, "error", 5);
            exit(1);
        }
}
void ft_echo(char **str, int fd)
{
    int i;
    int flag;

    flag = 0;
    i = 0;
    if (*str == NULL)
        exit(1);
    if (match_word("-n", *str))
        {
            if (str[1] == NULL)
                exit(1);
            flag = 1;
            i++;
    }
    while (str[i])
    {
        ft_write(str[i], fd);
        if (str[i+1] != NULL)
            ft_write(" ", fd);
        i++;
    }

    if (flag == 1)
        ft_write("\n", fd);
}
void ft_pwd()
{
    char buff[1024];
    char *ret;

    ret = getcwd(buff, 1024);
    if (ret == NULL)
    {
        //error
        exit(1);
    }
    ft_write(ret, 1);

}

void change_dir(char *new_dir)
{
    if (chdir(new_dir) == -1) 
        {
            ft_write( "bash: cd: ", 2);
            ft_write(new_dir, 2);
            ft_write(": No such file or directory\n", 2);
            exit(1);
        }
}
#include <dirent.h>
int main(int argc, char **argv, char *envp[])
{
    
    char buf[1024];
    int i;
    i = 0;
    while (envp[i])
        {
            printf("%s\n", envp[i]);
            i++;
        }
   //char *r = getcwd(buf, 100);
   // char buf[100];
   
}
 