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
void add_it_to_env(t_cmd *cmd, char *new)
{
   env_addback(cmd->env,env_new(new));
   // prints the changed env
     t_env *ff = cmd->env;
    //
    // -> env updates didnt make it to the other side (prent process)
    // ?? way to updated among all processes!!
    //


    // while (ff != NULL)
    // {
    //     write(2, ff->line, ft_strlen(ff->line));
    //     write(2, "\n", 1);
    //     ff = ff->next;
    // }
}
void parse_indetifier(t_cmd *cmd, char *str)
{
    int i;
    int ret;

    i = 0;
    

    ret = check_before_env(str) ;
    if (ret == 0)
    {
        write(2, str, ft_strlen(str));
        write(2, ": not a valid identifier\n", ft_strlen(": not a valid identifier\n"));
        return ;
    }
    else if(ret == -1)
    {
        //here we should know first if we have to add it to the export list or the env list or do nothing!
    }
    add_it_to_env(cmd, str);
}