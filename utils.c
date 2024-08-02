#include "mini.h"

void ft_write(char *str, int fd)
{
    if (write(fd, str, ft_strlen(str)) == -1)
        {
            //error
            write(2, "error\n", 6);
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
void ft_pwd(t_cmd *cmd)
{
    char buff[1024];
    char *ret;

    ret = getcwd(buff, 1024);
    if (ret == NULL)
    {
        //error
        write(2,"error\n", 6);
        exit(1);
    }
    if (cmd->pipe == 1)
        {
            ft_write(ret,STDOUT_FILENO );
            ft_write("\n", STDOUT_FILENO);
        }
    else    
        ft_write(ret, STDIN_FILENO);

}
void add_to_env(t_cmd *cmd, char *new_dir)
{
    t_env *tmp;
    t_cmd *tmp2;
    t_cmd *tmp_to_del;
    tmp = cmd->env;
    //more checks here for SEGV
     while (tmp != NULL)
     {
        if ( ft_strlen(tmp->line) > 3 &&
         tmp->line[0] == 'P' && tmp->line[1] == 'W' && tmp->line[2] == 'D')
         {
           // tmp_to_del = tmp;
            if (tmp->next != NULL)
            {
                tmp->prev->next = tmp->next;
                tmp->next->prev = tmp->prev->next;
                free(tmp);
                env_addback(tmp, env_new(new_dir));
                return ;
            }
            tmp->prev->next = NULL;
            free(tmp);
            env_addback(tmp, env_new(new_dir));
            return ;
         }
        tmp = tmp->next;
     }
}
void change_dir(t_cmd *cmd, char *new_dir)
{
    if (cmd->pipe || chdir(new_dir) == -1) 
        {
            ft_write( "bash: cd: ", 2);
            ft_write(new_dir, 2);
            ft_write(": No such file or directory\n", 2);
            exit(1);
        }
     t_env *ff = cmd->env;
    
    add_to_env(cmd, new_dir);
    while (ff != NULL)
    {
        write(2, ff->line, ft_strlen(ff->line));
        write(2, "\n", 1);
        ff = ff->next;
    }
}
t_env *env_new(char *new_line)
{
    t_env *new;
    new = (t_env *) malloc(sizeof(t_env));
    if (!new)
        exit(1);
    new->line = strdup(new_line);
    new->next = NULL;
    new->prev = NULL;
    return (new);
}

t_env *env_getlast(t_env *env)
{
    t_env *tmp;
    tmp = env;

    if (env == NULL)
        return NULL;
    if (env->next == NULL)
        return (env);
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    return (tmp);
}

// void env_addback(t_env *head, t_env *new)
// {
// 	t_list	*tmp;

// 	if (!head)
// 		return ;
// 	tmp = p_last(*head);
// 	if (!tmp)
// 	{
// 		*head = new;
// 		return ;
// 	}
// 	tmp->next = new;
// 	new->prev = tmp;
// }
void env_addback(t_env *head, t_env *new)
{
    t_env *tmp;
    // if (!head)
    //     {
    //         if (new == NULL)
    //             return ;
    //         head = new;
    //         return ;
    //     }
    tmp = env_getlast(head);
    tmp->next = new;
    new->prev = tmp;
}
t_env *create_env_list(char **env)
{
    int i;
    t_env *head;

    head = env_new(env[0]);
    i = 1;
    while (env[i])
    {
        env_addback(head,env_new(env[i]));
        i++;
    }
    printf("-----------%s-----\n", env[0]);
    return (head);
}
// int main(int argc, char **argv, c har *envp[])
// {
    
//     // char *s = readline(">") ;
//     // printf("[%s]\n", s);
//     // char buf[1024];
//     int i;
//    i = 0;
//     t_env *f = create_env_list(envp);
//     extern char **environ;
//     char **s = environ ;
//     // while (envp[i])
//     // {
//     //     printf("%s\n")
//     //     i++;
//     // }
    
//     while ( f != NULL)
//     {
//         //printf("%s\n", s[i]);
//         printf("[[%s]]\n", f->line);
//       i++;
//         f = f->next;
//     }

//    //char *r = getcwd(buf, 100);
//    // char buf[100];
   
//  }
 