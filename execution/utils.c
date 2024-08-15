#include "minishell.h"

void ft_write(char *str, int fd)
{

    if (str == NULL)
        return ;
    if (write(fd, str, ft_strlen(str)) == -1)
        {
            write(2, "error\n", 7);
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
        ft_write("\n", fd);
    if (match_word("-n", *str))
        {
            if (str[1] == NULL)
                return ;
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

    if (flag != 1)
        ft_write("\n", fd);

}
void ft_pwd(t_all *all)
{
    char buff[1024];
    char *ret;
    ret = getcwd(buff, 1024);
    if (ret == NULL)
        ft_error(all);
    
    ft_write(ret, STDOUT_FILENO);
}
