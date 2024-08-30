#include "minishell.h"

void throw_error(char *msg, t_all *all)
{
  printf("%sError: %s%s\n", RED, msg, NC);
  
  all->error = 1;
  // exit(1);
}

int find_pipe_index(char *str)
{
  int i;
  size_t len;

  i = 0;
  len = ft_strlen(str);
  while (i < len && str[i])
  {
    if(str[i] == PIPE)
      return i;
    else if(i < len && str[i] == DOUBLE_QUOTE)
    {
      i++;
      while(i < len && str[i] && str[i] != DOUBLE_QUOTE)
        i++;
    }
    else if(i < len && str[i] == SINGLE_QUOTE)
    {
      i++;
      while(i < len && str[i] && str[i] != SINGLE_QUOTE)
        i++;
    }
    i++;
  }
  return -1;
}

size_t args_counter(char *str, int len)
{
  int i;
  int s_quote;
  int d_quote;
  size_t words;

  i = 0;
  s_quote = 0;
  d_quote = 0;
  words = 0;
  if(len == -1)
    len = ft_strlen(str);
  while(str[i] && i < len)
  {
    while (str[i] && str[i] == SPACE && i < len)
      i++;
    if (str[i] && str[i] != SPACE && s_quote == 0 && d_quote == 0 && i < len)
      words++;
    while (str[i] && str[i] != SPACE && i < len)
    {
      if (str[i] == SINGLE_QUOTE)
        s_quote++;
      else if (str[i] == DOUBLE_QUOTE)
        d_quote++;
      if (s_quote == 2)
        s_quote = 0;
      if (d_quote == 2)
        d_quote = 0;
      i++;
    }
  }
  return words;
}

void ft_free(char **args)
{
  int i;

  i = 0;
  // int j = 0;
  while(args[i])
  {
    // printf("%s%s%s\n", YELLOW, args[i], NC);
    if(args[i] != NULL)
      free(args[i]);
    i++;
  }
  if(args)
  {
    free(args);
    args = NULL;
  }
  free(args);
  args = NULL;
}

void print_list(t_cmd *head)
{
  int i;

  i = 0;
  if(!head)
    printf("head is null\n");
  printf("-----------------------------\n");
  while(head)
  {
    int i;

    i = 0;
    printf("%s --> ", head->cmd);
    while(head->args[i])
    {
      printf("%s{ %s } %s", YELLOW, head->args[i], NC);
      i++;
    }
    printf("%s %s %s", CYAN, head->full_path, NC);
    printf("%s %s %s", RED, head->in_file, NC);
    printf("%s %s %s", GREEN, head->out_file, NC);
    printf("%s %s %s", RED, head->append_file, NC);
    printf("%s %d %s", GREEN, head->pipe, NC);
    head = head->next;
  }
  printf("\n-----------------------------\n");
}

size_t nums_of_chars(char *str, char c)
{
  int i;
  int counter;

  i = 0;
  counter = 0;
  if(!str)
    return counter;
  while(str[i])
  {
    if(str[i] == c)
      counter++;
    i++;
  }
  return counter;
}

int skip_reds(char *str, int *i, char c, t_all *all)
{
  int counter;
  char redirection;
  size_t len;

  counter = 0;
  len = ft_strlen(str);
  while(*i < len && str[*i] && str[*i] == c)
  {
    counter++;
    *i += 1;
    if(counter > 2)
    {
      throw_error("parse error", all);
      return 0;
    }
  }
  while (*i < len && str[*i] && str[*i] == SPACE)
    *i += 1;
  if(*i < len && is_symbol(str[*i]))
  {
    throw_error("parse error", all);
    return 0;
  }
  return 1;
}

size_t get_vars_length(char *str)
{
  int i;
  size_t length;
  size_t str_len;

  i = 0;
  length = 0;
  str_len = ft_strlen(str);
  if(!str)
    return length;
  while(str[i])
  {
    while(str[i] && ft_isspace(str[i]))
      i++;
    // if(str[i] && str[i] == VAR_SIGN && str[i - 1] != BACK_SLASH)
    // printf("outside the func\n");
    if(str[i] && str[i] == VAR_SIGN && (i == 0 || (i > 0 && str[i - 1] != BACK_SLASH)))
    {
      // printf("inside the func\n");
      // if(i > 0 && str[i - 1] != BACK_SLASH)
      while(str[i] && str[i] != DOUBLE_QUOTE && str[i] != PIPE)
      {
        i++;
        length++;
      }
      break;
    }
    i++;
  }
  // printf("length should returned : %zu\n", length);
  return length;
}

char *find_variable(char *str)
{
  size_t i;
  size_t j;
  char *vars;

  i = 0;
  j = 0;
  if(!str)
    return NULL;
  vars = malloc(get_vars_length(str) + 1);
  while(str[i])
  {
    while(str[i] && ft_isspace(str[i]))
        i++;
    if(str[i] == VAR_SIGN && str[i - 1] != BACK_SLASH)
    {
      while(str[i] && str[i] != DOUBLE_QUOTE)
      {
        vars[j++] = str[i];
        i++;
      }
    }
    i++;
  }
  // printf("%sall vars: %s%s\n", YELLOW, vars, NC);
  return vars;
}

// char *get_variable(char *str)
// {
//   size_t i;
//   char var;

//   i = 0;
//   while()
//   {

//   }
//   return  var
// }



char *find_and_remove(char *str, char c)
{
  int i;
  int j;
  size_t len;
  char *res;

  i = 0;
  j = 0;

  len = ft_strlen(str) - nums_of_chars(str, c) + 1;
  if(!str)
    return NULL;
  // printf("%s --------> %s%s\n", RED, str, NC);
  res = (char *)malloc(sizeof(char) * len);
  while(str[i])
  {
    if(str[i] != c)
    {
      res[j] = str[i];
      j++;
    }
    i++;
  }
  res[j] = '\0';
  free(str);
  str = NULL;
  // printf("after fixing: %s\n", res);
  return res;
}
