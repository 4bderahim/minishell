#include <minishell.h>

static char **get_all_paths(char *cmd, t_env *env)
{
  char **all_paths;
  char *path; 
  
  path = get_var_value(ft_strdup("PATH"), env);
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
  ft_free(path, i + 1);
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

int get_arr_len(char **arr)
{
  int i;

  i = 0;
  while(arr[i])
    i++;
  return i;
}

char *search_at_curr_dir(char *cmd)
{
  int i;
  char cwd[256];
  char *path;

  i = 0;
  path = NULL;
  getcwd(cwd, sizeof(cwd));
  path = ft_strjoin(ft_strjoin(ft_strdup(cwd), "/"), cmd);
  if(access(path, X_OK) == 0)
    return path;
  free(path);
  path = NULL;
  return NULL;
}

char *get_right_path(char **all_paths, char *cmd, char *line, bool is_path)
{
  int i;
  char *path;
  char *curr_path;

  i = 0;
  path = NULL;
  while (all_paths[i])
  {
    curr_path = ft_strjoin(ft_strdup(all_paths[i]), "/");
    path = ft_strjoin(curr_path, line);
    if(access(path, X_OK) == 0)
    {
      if (correct_path(path, cmd, is_path))
        return path;
    }
    free(path);
    path = NULL;
    i++;
  }
  return path;
}

char  *get_path(char *cmd, t_env *env)
{
  char *path;
  char **all_paths;
  char *line;
  bool is_path;

  is_path = false;
  path = NULL;
  if(!cmd)
    return NULL;
  all_paths = get_all_paths(cmd, env);
  if(cmd[0] && cmd[0] == SLASH && cmd[ft_strlen(cmd) - 1])
  {
    line = isolate_cmd_from_path(cmd);
    is_path = true;
  }
  else
    line = cmd;
  path = get_right_path(all_paths, cmd, line, is_path);
  ft_free(all_paths, get_arr_len(all_paths));
  if(!path)
    path = search_at_curr_dir(cmd);
  return path;
}

char **get_herdoc_delimiter(char **args, t_all *all)
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
    if(!ft_strcmp(args[i], "<") && !ft_strcmp(args[i + 1], "<"))
    {
      i++;
      if(!args[i + 1])
        return (throw_error("syntax error near unexpected token", all, 258), NULL);
      else
        delimiters[len++] = ft_strdup(args[i + 1]);
    }
    i++;
  }
  delimiters[len] = NULL;
  return delimiters;
}
