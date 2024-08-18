#include <minishell.h>

char *ft_strtok(char *str)
{
  size_t i;
  size_t j;
  size_t len;
  char *result;

  i = 0;
  j = 0;
  len = ft_strlen(str);
  printf("len : %zu\n", len);
  if(!str)
    return NULL;
  while(str[i] && ft_isspace(str[i]))
    i++;
  while(str[len - 1] && ft_isspace(str[len - 1]))
    len--;
  result = malloc((len - i) + 1);
  while(i < len)
    result[j++] = str[i++];
  result[j] = '\0';
  printf("%sft_strtok:%s---> %zu%s\n", MAGENTA, result, ft_strlen(result), NC);
  free(str);
  return result;
}

char *get_only_var(char *str)
{
  size_t i;
  size_t len;
  char *result;

  i = 0;
  len = 0;
  while(str[len] && !is_symbol(str[len]) && str[len] != SPACE && str[len] != SINGLE_QUOTE)
    len++;
  // result = malloc(len + 1);
  result = ft_substr(str, 0, len);
  printf("only variable: %s\n", result);
  return result;
}

char *get_var_value(char *str, t_env *env)
{
  char *var;
  char *rest;
  char *result;

  result = NULL;
  var = get_only_var(str);
  rest = ft_strdup(str + ft_strlen(var));
  while(env)
  {
    if(!ft_strcmp(var, env->variable))
    {
      result = ft_strjoin(env->value, rest);
      break;
    }
    env = env->next;
  }
  free(str);
  free(var);
  if(!result)
    return rest;
  free(rest);
  return result;
}

char *handle_variables(char *str, t_env *env, size_t length)
{
  size_t i;
  char **vars;
  char *var;
  char *result;
  // char *rest;

  printf("===> +length: %zu\n", length);
  i = 0;
  if(!str)
  {
    printf("%s-------> NULL <----------%s\n", GREEN, NC);
    return NULL;
  }
  vars = ft_split(str, '$');
  result = NULL;
  while (vars[i])
  {
    printf("var ---> %s\n", vars[i]);

    vars[i] = find_and_remove(vars[i], DOUBLE_QUOTE);
    // rest = ft_substr()
    // printf("var length: %zu\n", get_length(vars[i]));
    // result = 
    vars[i] = get_var_value(vars[i], env);
    printf("after getting value: %s\n", vars[i]);
    // var = get_variable(vars[i]);
    result = ft_strjoin(result, vars[i]);
    printf("result: %s\n", result);
    i++;
  }
  free(str);
  str = NULL;
  ft_free(vars);
  return result;
}
