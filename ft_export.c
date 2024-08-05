#include "mini.h"

int check_(char *s, int index)
{
    int i;  
    int j;
    char *sp;

    sp = "'`~#$&*()\\|[]{};<>\"/?!";
    i = 0;
    while (i < index)
        {
            j =  0;
            while(sp[j])
            {
                if (sp[j] == s[i])
                    return (0);
                j++;
            }
            i++;
        }
    return (1);
}
int check_before_env(char *s)
{
    int i;
    i = 0;
    if ((s[i] >= '0' && s[i] <= '9') || s[i] == '=')//  also check if empty
        return (0);
    while (s[i])
        {
            if (s[i] == '=')
                {
                    if (!check_(s, i))
                        return (0);
                    return (1);
                }
            i++;
        }
    return (-1);// if we should add an (export var) to the env or not;
}
void add_it_to_env(t_all *all, char *new)
{
   env_addback(all->env,env_new(new));
   // prints the changed env
    // t_env *ff = all->env;
   


    // while (ff != NULL)
    // {
    //     write(2, ff->line, ft_strlen(ff->line));
    //     write(2, "\n", 1);
    //     ff = ff->next;
    // }
}
void identifier_error(char *indentifer)
{
    write(2, "export: ", 8);
    write(2, indentifer, ft_strlen(indentifer));
    write(2, ": not a valid identifier\n", ft_strlen(": not a valid identifier\n"));
}
void parse_indetifier(t_all *all, char *str)
{
    int i;
    int ret;

    i = 0;
    

    ret = check_before_env(str) ;
    if (ret == 0)
    {
        identifier_error(str);
        return ;
    }
    else if(ret == -1)
    {
       exp_addback(all->exp, exp_new(str));//t_exp    *head, t_exp    *new)
       return ;
    }

    add_it_to_env(all, str);
}