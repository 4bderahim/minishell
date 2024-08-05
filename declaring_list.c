// #include "mini.h"

// t_env *exp_getlast(t_dec *exp)
// {
//     t_dec *tmp;
//     tmp = exp;

//     if (exp == NULL)
//         return NULL;
//     if (exp->next == NULL)
//         return (exp);
//     while (tmp->next != NULL)
//     {
//         tmp = tmp->next;
//     }
//     return (tmp);
// }

// t_dec *exp_new(char *new_line)
// {
//     t_dec *new;
//     int i;
//     i = 0;
//     while(new_line[i])
//     {
//         if (new_line[i] == '=')
//             {
//                 new_line[i] = '\0';
//                 i++;
//                 break;
//             }
//         i++;
//     }
//     new = (t_dec *) malloc(sizeof(t_exp));
//     if (!new)
//         exit(1);
    
//     new-> = strdup(new_line);

//     new->next = NULL;
//     return (new);
// }

// void exp_addback(t_exp    *head, t_exp    *new)
// {
//     t_dec *tmp;
//     // if (!head)
//     //     {
//     //         if (new == NULL)
//     //             return ;
//     //         head = new;
//     //         return ;
//     //     }
//     tmp = env_getlast(head);
//     tmp->next = new;
//     //new->prev = tmp;
// }

// // void set_lists(t_all *all, char **env)
// // {
// //     while (env)
// //     {
// //         env_addback(all->exp, exp_new(*env) );
// //         env++;
// //     }
// // }