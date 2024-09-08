
#include "minishell.h"

t_exp	*new_exp_(t_env *env)
{
	t_exp	*new;

	new = (t_exp *)malloc(sizeof(t_exp));
	if (!new)
		return (NULL);
	new->value = ft_strdup(env->value);
	new->variable = ft_strdup(env->variable);
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_exp	*set_export_list(t_all *all)
{
	t_env	*env;
	t_exp	*exp;
	t_exp	*new;

	env = all->env;
	exp = new_exp_(env);
	env = env->next;
	while (env != NULL)
	{
		new = new_exp_(env);
		if (new == NULL)
			return (NULL);
		exp_addback(exp, new);
		env = env->next;
	}
	return (exp);
}
