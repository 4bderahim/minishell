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
// int main()
// {
//     char buf[100];
    
//     change_dir("asdas");
//     getcwd(buf, 100);
//     printf("%s\n", buf);
        
// }
 