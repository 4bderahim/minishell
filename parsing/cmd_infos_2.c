#include <minishell.h>

static char **get_all_paths(char *cmd)
{
  char **all_paths;
  char *path = ft_strdup(getenv("PATH"));
  
  all_paths = ft_split(path, ':');
  if(!all_paths)
    return NULL;
  free(path);
  path = NULL;

  return all_paths;
}

static size_t get_delimiters_len(char **args)
{
  size_t i;
  size_t len;

  i = 0;
  len = 0;
  while(args[i])
  {
    if(!ft_strcmp(args[i], "<") && !ft_strcmp(args[i + 1], "<")) // and [i+1] != '\0'
      len++;
    i++;
  }
  return len;
}

char *isolate_cmd_from_path(char *cmd)
{
  size_t i;
  char *command;
  char **path;

  i = 0;
  path = ft_split(cmd, '/');
  while(path[i + 1] != NULL)
    i++;
  command = ft_strdup(path[i]);
  // free(cmd);
  // cmd = NULL;
  ft_free(path);
  return command;
}

bool correct_path(char *path, char *cmd, bool is_path)
{
  if(is_path)
  {
    if(!ft_strcmp(path, cmd))
      return true;
    return false;
  }  
  return true;
}

char  *get_path(char *cmd)
{
  int i;
  char *path;
  char *command;
  char **all_paths;
  char *line;
  bool is_path;

  i = 0;
  is_path = false;
  all_paths = get_all_paths(cmd);
	// printf("%s-----------> checkpoint <-----------%s\n", RED, NC);
  // printf("cmd line ---> %s\n", cmd);
  if(cmd[0] && cmd[0] == SLASH && cmd[ft_strlen(cmd) - 1])
  {
    line = isolate_cmd_from_path(cmd);
    is_path = true;
  }
  else
    line = cmd;
  while (all_paths[i])
  {
    command = ft_strjoin(all_paths[i], "/");
    path = ft_strjoin(command, line);
    free(command);
    if(access(path, X_OK) == 0)
    {
      ft_free(all_paths);
      if (correct_path(path, cmd, is_path))
        return path;
      return (free(path), NULL);
      // return path;
    }
    free(path);
    path = NULL;
    i++;
  }
  ft_free(all_paths);
  // printf("should return NULL\n");
  return NULL;
}

char **get_herdoc_delimiter(char **args)
{
  size_t i;
  size_t len;
  char **delimiters;

  i = 0;
  len = get_delimiters_len(args);
  if(!len)
    return NULL;
  delimiters = malloc(sizeof(char *) * (len + 1));
  i = 0;
  len = 0;
  while(args[i])
  {
    if(!ft_strcmp(args[i], "<") && !ft_strcmp(args[i + 1], "<")) // and [i+1] != '\0'
    {
      i++;
      if(!args[i + 1])
        throw_error("syntax error near unexpected token `newline'");
      else
        delimiters[len++] = ft_strdup(args[i + 1]);
    }
    i++;
  }
  delimiters[len] = NULL;
  return delimiters;
}
