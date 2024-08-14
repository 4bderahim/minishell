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
    {
        //error
        write(2,"error\n", 6);
        exit(1);
    }
    ft_write(ret, STDOUT_FILENO);
}

t_env *env_new(char *new_line)
{
    t_env *new;
    int index;
    // if (new_line == NULL)
    //     return NULL;
    index = spliter_index(new_line);
        //printf("|s@%s~~|\n\n", new_line+i);
    //else
    
    new = (t_env *) malloc(sizeof(t_env));
    if (!new)
        return (NULL);
    new->variable = strdup(new_line);
    if (*(new_line+index))
        new->value = strdup(new_line+index);
    else
        new->value = NULL;//strdup(new_line+i);
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
    if(new == NULL)
        write(2 ,"\n&&\n\n\n", 6);
        // write(2 ,new->value, 10);
        // write(2 ,"\n\n", 2);


    // if (new == NULL)
    // if (!head)
    //     {
    //         // if (new == NULL)
    //             return ;
    //         head = new;
    //         return ;
    //     }
    tmp = env_getlast(head);
    tmp->next = new;
    
    new->prev = tmp;
}
int get_sign_index(char *line)
{
    int index;
    index = 0;
    while (line[index])
    {
        if (line[index] == '=')
            return (index);
        index++;
    }
    return (index);
}
t_env *create_env_list(char **envp_)
{
    int i;
    t_env *head;
    char **envp;
    t_env *new;
    i = 1;
    envp = envp_;
    head = env_new(envp[0]);

    while (envp[i] != NULL)
    {
        new = env_new(envp[i]);
        if (new == NULL)
            return (NULL);
        env_addback(head,new);
        i++;
    }
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
 