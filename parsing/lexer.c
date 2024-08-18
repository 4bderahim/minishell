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

  len = ft_strchr_pro(str + 1, ' ', '|', inside_quotes); 
  if(len != -1)
    return len;
  return ft_strlen(str);
}

char *get_str_in_quotes(char *command, int *i, char c, t_env *env)
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
      throw_error("single quote must be closed");
    else
      throw_error("double quote must be closed");
    return NULL;
  }
  buffer = ft_substr(command + *i, 0, len);
  *i += len;
  if(c == DOUBLE_QUOTE && get_vars_length(buffer) > 0)
    buffer = handle_variables(buffer, env, get_vars_length(buffer));
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

char *get_str_without_quotes(char *command, int *i, t_env *env)
{
  int len;
  char *buffer;

  len = find_len(command + *i, false);
  buffer = ft_substr(command, *i, len);
  if(get_vars_length(buffer) > 0)
    buffer = handle_variables(buffer, env, get_vars_length(buffer));
  else if(str_has_quotes(buffer, DOUBLE_QUOTE))
    buffer = find_and_remove(buffer, DOUBLE_QUOTE);
  else if(str_has_quotes(buffer, SINGLE_QUOTE))
    buffer = find_and_remove(buffer, SINGLE_QUOTE);
  *i += len;
  return buffer;
}

int create_cmd(t_cmd **head, char **args, int words, int is_pipe)
{
  t_cmd *cmd;

  cmd = ft_lstnew(args, words, is_pipe);
  if(!cmd)
  {
    printf("%scommand not found%s\n", RED, NC);
    ft_free(args);
    return 0;
  }
  ft_lstadd_back(head, cmd);
  ft_free(args);
  return 1;
}

char **allocate_args(char *command, int *pipe_idx, int *words, int i)
{
  char **args;

  *pipe_idx = find_pipe_index(command + i);
  if(*pipe_idx == -1)
    *pipe_idx = ft_strlen(command);
  *words = args_counter(command + i, *pipe_idx);
  args = (char **)malloc(sizeof(char *) * (*words + 1));
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

size_t reds_counter(char *cmd)
{
  int i;
  size_t counter;

  i = 0;
  counter = 0;
  while(cmd[i] && i < ft_strlen(cmd))
  {
    if(cmd[i] == SINGLE_QUOTE || cmd[i] == DOUBLE_QUOTE)
    {
      i++;
      skip_str_inside_quote(cmd, &i, cmd[i - 1]);
    }
    if(cmd[i] == IN_RED || cmd[i] == OUT_RED)
    {
      counter++;
      skip_reds(cmd, &i, cmd[i]);
    }
    i++;
  }
  return counter * 2;
}

char *fix_cmd(char *cmd)
{
  int i;
  int j;
  char *line;

  i = 0;
  j = 0;
  line = malloc(sizeof(char) * ft_strlen(cmd) + reds_counter(cmd) + 1);
  while (cmd[i])
  {
    if((cmd[i] == IN_RED || cmd[i] == OUT_RED) && (i > 0 && (cmd[i - 1] != IN_RED || cmd[i - 1] != OUT_RED)))
      line[j++] = ' ';
    else if (i > 0 && (cmd[i - 1] == IN_RED || cmd[i - 1] == OUT_RED) && cmd[i] != SPACE)
      line[j++] = ' ';
    line[j++] = cmd[i++];
  }
  line[j] = '\0';
  free(cmd);
  return line;
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
      lexer->buffer = get_str_in_quotes(command, &lexer->i, SINGLE_QUOTE, NULL);
    else if(command[lexer->i] == DOUBLE_QUOTE)
      lexer->buffer = get_str_in_quotes(command, &lexer->i, DOUBLE_QUOTE, (*all)->env);
    else
      lexer->buffer = get_str_without_quotes(command, &lexer->i, (*all)->env);
    if(lexer->buffer == NULL || full_of_spaces(lexer->buffer))
    {
      ft_free(lexer->args);
      free(lexer->buffer);
      free(command);
      return 0;
    }
    if(!ft_strlen(lexer->buffer))
      break;
    lexer->args[lexer->j] = ft_strdup(lexer->buffer);
    free(lexer->buffer);
    lexer->j++;
  }
  return 1;
}

int handle_new_cmd(t_all **all, t_lexer *lexer, char *command)
{
  if(lexer->words)
  {
    if(is_pipe_after(command + lexer->i))
      lexer->pipe = 1;
    if (!create_cmd(&(*all)->cmd, lexer->args, lexer->words, lexer->pipe))
    {
      free(command);
      return 0;
    }
  }
  else
    ft_free(lexer->args);
  return 1;
}

int ft_lexer(char *command, t_all **all)
{
  t_lexer lexer;

  lexer.i = 0;
  lexer.args = NULL;
  lexer.length = ft_strlen(command);
  command = fix_cmd(command);
  while(lexer.i < lexer.length)
  {
    lexer.j = 0;
    lexer.pipe = 0;
    while (command[lexer.i] && ft_isspace(command[lexer.i]))
      lexer.i++;
    if(!ft_strlen(command + lexer.i))
      return (free(command), 0);
    lexer.args = allocate_args(command, &lexer.pipe_idx, &lexer.words, lexer.i);
    if(!lexer.args || !check_command(&lexer, all, command))
      return 0;
    lexer.args[lexer.j] = NULL;
    if(!handle_new_cmd(all, &lexer, command))
      return 0;
    lexer.i++;
  }
  free(command);
  return 1;
}

// while(command[lexer.i] && lexer.j < lexer.words)
// {
//   while (command[lexer.i] && ft_isspace(command[lexer.i]))
//     lexer.i++;
//   if(command[lexer.i] == '\'')
//     lexer.buffer = get_str_in_quotes(command, &lexer.i, SINGLE_QUOTE, NULL);
//   else if(command[lexer.i] == '\"')
//     lexer.buffer = get_str_in_quotes(command, &lexer.i, DOUBLE_QUOTE, (*all)->env);
//   else
//     lexer.buffer = get_str_without_quotes(command, &lexer.i, (*all)->env);
//   if(lexer.buffer == NULL || full_of_spaces(lexer.buffer))
//   {
//     ft_free(lexer.args);
//     free(lexer.buffer);
//     free(command);
//     return 0;
//   }
//   if(!ft_strlen(lexer.buffer))
//     break;
//   // lexer.args[lexer.j] = lexer.buffer;
//   lexer.args[lexer.j] = ft_strdup(lexer.buffer);
//   free(lexer.buffer);
//   lexer.j++;
// }

// if(lexer.words)
// {
//   if(is_pipe_after(command + lexer.i))
//     lexer.pipe = 1;
//   if (!create_cmd(&(*all)->cmd, lexer.args, lexer.words, lexer.pipe))
//   {
//     free(command);
//     return 0;
//   }
// }
// else
//   ft_free(lexer.args);
