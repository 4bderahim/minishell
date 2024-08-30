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
  // printf("len : %zu\n", len);
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
  // printf("%sft_strtok:%s---> %zu%s\n", MAGENTA, result, ft_strlen(result), NC);
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
  // printf("only variable: %s\n", result);
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
      result = ft_strjoin(ft_strdup(env->value), rest);
      break;
    }
    env = env->next;
  }
  free(str);
  str = NULL;
  free(var);
  if(!result)
    return rest;
  free(rest);
  return result;
}

int	ft_isalnum(int c)
{
	if ((c >= 48 && c <= 57) || (c >= 97 && c <= 122) || (c >= 65 && c <= 90))
		return (1);
	return (0);
}

char *handle_variables(char *str, t_env *env, size_t length, t_all *all)
{
  // free();
  size_t i = 0;
  size_t len = strlen(str);
  // char* output = malloc(len * 3);
  char output[1024];
  char* var_value;
  output[0] = '\0';

  while (i < len)
  {
    if (str[i] == '$') 
    {
      i++;
      if(i < len && str[i] == '?')
      {

        var_value = ft_itoa(all->exit_status);
        strcat(output, var_value);
        free(var_value);
        var_value = NULL;
        i++;
      }
      else
      {
        char var_name[1024];
        size_t var_len = 0;
        while (i < len && (ft_isalnum(str[i]) || str[i] == '_'))
        var_name[var_len++] = str[i++];
        var_name[var_len] = '\0';
        var_value = get_var_value(ft_strdup(var_name), env);
        // printf("varname is : %s\n", var_name);
        strcat(output, var_value);
        free(var_value);
        var_value = NULL;
      }
      
    }
    else
    {
      size_t j = ft_strlen(output);
      output[j] = str[i];
      output[j + 1] = '\0';
      i++;
    }
  }
  // printf("-------> waaaaaaaaaaaaaaaaa3\n");
  free(var_value);
  free(str);
  str = NULL;
  return ft_strdup(output);
}
