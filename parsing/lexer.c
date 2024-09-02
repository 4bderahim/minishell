#include "minishell.h"

int ft_isspace(char c)
{
  if((c >= 9 && c <= 13) || c == 32)
    return 1;
  return 0;
}

int find_len(char *str, bool inside_quotes)
{
  int len;
  int i;
  size_t str_len;

  i = 0;
  // if(!str)
  //   return -1;
  str_len = ft_strlen(str);
  while(i < str_len && str[i] != SPACE && str[i] != PIPE)
  {
    if(i < len && (str[i] == SINGLE_QUOTE || str[i] == DOUBLE_QUOTE))
    {
      i++;
      skip_str_inside_quote(str, &i, str[i - 1]);
    }
    i++;
    if(i< str_len && str[i] == '\0')
			return -1;
  }
  // len = ft_strchr_pro(str + 1, '\0', '|', inside_quotes); 
  // if(len != -1)
  //   return len;
  return i;
}

char *get_str_in_quotes(char *command, int *i, char c, t_all *all)
{
  int len;
  char *buffer;
  char *rest;
  char *var_value;

  *i += 1;
  len = ft_strchr(command + *i, c);
  if(len == -1)
  {
    if(c == SINGLE_QUOTE)
      throw_error("single quote must be closed", all, 1);
    else
      throw_error("double quote must be closed", all, 1);
    return NULL;
  }
  buffer = ft_substr(command + *i, 0, len);
  if(c == DOUBLE_QUOTE && get_vars_length(buffer) > 0)
    buffer = handle_variables(buffer, all->env, get_vars_length(buffer), all);
  *i += len;
  buffer = find_and_remove(buffer, c);
  return buffer;
}

bool str_has_quotes(char *str, char c)
{
  int i;

  i = 0;
  while (str[i])
  {
    if(str[i] == c)
      return true;
    i++;
  }
  return false;
}

bool no_herdoc_delemiter(char *cmd, int i)
{
  if(!cmd)
    false;
  while(i >= 0)
  {
    if(cmd[i] == SINGLE_QUOTE || cmd[i] == DOUBLE_QUOTE)
    {
      i--;
      while(i >= 0 && cmd[i] != cmd[i + 1])
        i--;
    }
    if(i >= 0 && cmd[i] == PIPE)
      return true;
    else if(cmd[i] == '<')
    {
      i--;
      if(i >= 1 && cmd[i] == SPACE && cmd[i - 1] == '<')
        return false;
    }
    i--;
  }
  return true;
}

char *get_str_without_quotes(char *command, int *i, t_env *env, t_all *all)
{
  int len;
  char *buffer;
  int index;

  len = find_len(command + *i, false);
  buffer = ft_substr(command, *i, len);
  if(get_vars_length(buffer) > 0)
  {
    if(no_herdoc_delemiter(command, *i - 1))
      buffer = handle_variables(buffer, env, get_vars_length(buffer), all);
  }
  index = ft_strchr_pro(buffer, DOUBLE_QUOTE, SINGLE_QUOTE, false); 
  if(index != -1 && buffer[index - 1] == SINGLE_QUOTE)
    buffer = find_and_remove(buffer, SINGLE_QUOTE);    
  else if (index != -1 && buffer[index - 1] == DOUBLE_QUOTE)
    buffer = find_and_remove(buffer, DOUBLE_QUOTE);
  *i += len;
  return buffer;
}

int create_cmd(t_all **all, char **args, int words, int is_pipe)
{
  t_cmd *cmd;

  cmd = ft_lstnew(all, args, words, is_pipe);
  if(!cmd)
    return ft_free(args, get_arr_len(args)), 0;
  ft_lstadd_back(&(*all)->cmd, cmd);
  ft_free(args, get_arr_len(args));
  return 1;
}

char **allocate_args(char *command, int *pipe_idx, int *words, int i)
{
  char **args;

  *pipe_idx = find_pipe_index(command + i);
  if(*pipe_idx == -1)
    *pipe_idx = ft_strlen(command);
  *words = args_counter(command + i, *pipe_idx);
  args = (char **)malloc(sizeof(char *) * (*words + 2));
  if(!args)
    return NULL;
  return args;
}

bool is_pipe_after(char *str)
{
  int i;

  i = 0;
  while (str[i] && ft_isspace(str[i]))
    i++;
  if(str[i] == PIPE)
    return true;
  return false;
}

void skip_str_inside_quote(char *cmd, int *i, char c)
{
  while(cmd[*i] && cmd[*i] != c)
    *i += 1;
}

bool is_symbol(char c)
{
	if(c == PIPE|| c == IN_RED || c ==  OUT_RED)
		return true;
	return false;
}

size_t reds_counter(char *cmd, t_all *all)
{
  int i;
  size_t counter;

  i = 0;
  counter = 0;
  if(!cmd)  
    return counter;
  while(i < ft_strlen(cmd))
  {
    if(i < ft_strlen(cmd) && (cmd[i] == SINGLE_QUOTE || cmd[i] == DOUBLE_QUOTE))
    {
      i++;
      counter += 2;
      skip_str_inside_quote(cmd, &i, cmd[i - 1]);
    }
    if(cmd[i] == PIPE)
      counter++;
    else if(i < ft_strlen(cmd) && (cmd[i] == IN_RED || cmd[i] == OUT_RED))
    {
      counter++;
      if (!skip_reds(cmd, &i, cmd[i], all))
        return 0;
    }
    i++;
  }
  return ((counter * 2));
}

char *fix_cmd(char *cmd, t_all *all)
{
  int i;
  int j;
  int k;
  char *line;
  char quote;
  int reds_nums = reds_counter(cmd, all);

  if(!reds_nums)
    return cmd;
  i = 0;
  j = 0;
  line = malloc(ft_strlen(cmd) + reds_nums + 1);
  if(!line)
    return NULL;
  while (i < ft_strlen(cmd))
  {
    if(cmd[i] == SINGLE_QUOTE || cmd[i] == DOUBLE_QUOTE)
    {
      quote = cmd[i];
      k = i;
      line[j++] = cmd[i++];
      while(i < ft_strlen(cmd) && cmd[i] != quote)
        line[j++] = cmd[i++];
      if(i - k == 1)
        line[j++] = ' ';
    }
    else if(((cmd[i] == IN_RED || cmd[i] == OUT_RED || cmd[i] == PIPE)
      && (i > 0 && (cmd[i - 1] != IN_RED || cmd[i - 1] != OUT_RED))))
      {
        line[j++] = ' ';
      }
    else if (i > 0 && (cmd[i - 1] == IN_RED || cmd[i - 1] == OUT_RED) && cmd[i] != SPACE)
      line[j++] = ' ';
    line[j++] = cmd[i++];
  }
  line[j] = '\0';
  return free(cmd),line;
}

bool full_of_spaces(char *buffer)
{
  size_t i;

  i = 0;
  while(buffer[i] && ft_isspace(buffer[i]))
    i++;
  if(i == ft_strlen(buffer))
    return true;
  return false;
}

int check_command(t_lexer *lexer, t_all **all, char *command)
{
  while(command[lexer->i] && lexer->j < lexer->words)
  {
    while (command[lexer->i] && ft_isspace(command[lexer->i]))
      lexer->i++;
    if(command[lexer->i] == SINGLE_QUOTE)
      lexer->buffer = get_str_in_quotes(command, &lexer->i, SINGLE_QUOTE, *all);
    else if(command[lexer->i] == DOUBLE_QUOTE)
      lexer->buffer = get_str_in_quotes(command, &lexer->i, DOUBLE_QUOTE, *all);
    else
      lexer->buffer = get_str_without_quotes(command, &lexer->i, (*all)->env, *all);
    if((*all)->error == true)
      return (free(lexer->buffer), 0);
    if(ft_strlen(lexer->buffer))
    {
      lexer->args[lexer->j] = ft_strdup(lexer->buffer);
      free(lexer->buffer);
      lexer->buffer = NULL;
      lexer->j++;
    }
    else
      lexer->words--;
    }
  return 1;
}

int handle_new_cmd(t_all **all, t_lexer *lexer, char *command)
{
  if(lexer->words && (is_symbol(lexer->args[lexer->words - 1][0]) 
    || lexer->args[lexer->words - 1][0] == BACK_SLASH))
      return (throw_error("syntax error near unexpected token", *all, 258), 0);
  if(lexer->words)
  {
    if(is_pipe_after(command + lexer->i))
      lexer->pipe = 1;
    if (!create_cmd(all, lexer->args, lexer->words, lexer->pipe))
      return 0;
  }
  else
    ft_free(lexer->args, lexer->j);
  return 1;
}

bool is_incorrect_cmd(char *cmd, int *pipe, t_all *all)
{
  size_t i;

  i = 0;
  if(cmd[i] == PIPE)
  {
    if(!cmd[i + 1])
      return (throw_error("syntax error near unexpected token", all, 258), true);
  }
  if(cmd[i + 1] == PIPE && cmd[i + 2] != PIPE)
  {
    *pipe = -1;
    return false;
  }
  else if(cmd[i + 1] == PIPE && cmd[i + 2] == PIPE)
    return (throw_error("syntax error near unexpected token", all, 258), true);
  return false;
}

void free_lexer(t_lexer *lexer)
{
  if(lexer->buffer)
    free(lexer->buffer);
  if(lexer->args)
    ft_free(lexer->args, get_arr_len(lexer->args));
}

void init_lexer(t_lexer *lexer, char *command)
{
  lexer->i = 0;
  lexer->args = NULL;
  lexer->length = ft_strlen(command);
}

int ft_lexer(char *command, t_all **all)
{
  t_lexer lexer;

  init_lexer(&lexer, command);
  while(lexer.i < lexer.length)
  {
    lexer.j = 0;
    lexer.pipe = 0;
    while (command[lexer.i] && ft_isspace(command[lexer.i]))
      lexer.i++;
    if(!ft_strlen(command + lexer.i) || is_incorrect_cmd(command + lexer.i, &lexer.pipe, *all))
      return (0);
    lexer.args = allocate_args(command, &lexer.pipe_idx, &lexer.words, lexer.i);
    if(!lexer.args || !check_command(&lexer, all, command))
      return (ft_free(lexer.args, lexer.j), 0);
    lexer.args[lexer.j] = NULL;
    if(!handle_new_cmd(all, &lexer, command))
    {
      free_lexer(&lexer);
      return 0;
    }
    lexer.i++;
    if(lexer.pipe == -1)
      return 1;
  }
  return 1;
}
