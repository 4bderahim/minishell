#include "mini.h"

void ft_write(char *str, int fd)
{
    if (str == NULL)
        return ;
    if (write(fd, str, ft_strlen(str)) == -1)
        {
            //error
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
    if (all->cmd->pipe == 1)
        {
            ft_write(ret,STDOUT_FILENO );
            ft_write("\n", STDOUT_FILENO);
        }
    else    
        ft_write(ret, STDIN_FILENO);

}
void add_to_env(t_all *all, char *new_dir)
{
    t_env *tmp;
    t_cmd *tmp2;
    t_cmd *tmp_to_del;
    tmp = all->env;
    //more checks here for SEGV
     while (tmp != NULL)
     {
        if ( ft_strlen(tmp->variable) > 3 &&
         tmp->variable[0] == 'P' && tmp->variable[1] == 'W' && tmp->variable[2] == 'D')
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
void change_dir(t_all *all, char *new_dir)
{
    if (all->cmd->pipe || chdir(new_dir) == -1) 
        {
            ft_write( "bash: cd: ", 2);
            ft_write(new_dir, 2);
            ft_write(": No such file or directory\n", 2);
            exit(1);
        }
    // t_env *ff = all->env;
    
    add_to_env(all, new_dir);
    // while (ff != NULL)
    // {
    //     write(2, ff->line, ft_strlen(ff->line));
    //     write(2, "\n", 1);
    //     ff = ff->next;
    // }
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
        exit(1);
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
t_env *create_env_list(char **envp)
{
    int i;
    t_env *head;
    t_env *last;
    i = 1;
    head = env_new(envp[0]);
    while (envp[i] != NULL)
    {
        env_addback(head,env_new(envp[i]));
        last = env_getlast(head);
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
 