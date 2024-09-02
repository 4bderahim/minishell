#include <minishell.h>

size_t get_slash_index(char *cmd)
{
  size_t i;
  size_t index;

  i = 0;
  index = -1;
  if(!cmd)
    return -1;
  while(cmd[i])
  {
    if(cmd[i] == SLASH)
      index = i;
    i++;
  }
  return index;
}


char *get_executable_path(char *cmd, char *filename)
{
  size_t slash_index;
  char *path;
  char cwd[256];

  slash_index = get_slash_index(cmd);
  if(slash_index == -1)
    return NULL;
  path = ft_strndup(cmd, slash_index + 1);
  chdir(path);
  free(path);
  path = NULL;
  getcwd(cwd, sizeof(cwd));
  path = ft_strjoin(ft_strjoin(ft_strdup(cwd), "/"), filename);
  printf("%s-------> %s%s\n", CYAN, path, NC);
  free(filename);
  filename = NULL;
  return path;
}

char *get_executable(char *cmd)
{
  size_t i;
  char **arr;
  char *filename;
  char *path;

  i = 0;
  if(get_slash_index(cmd) == -1)
    return NULL;
  arr = ft_split(cmd, SLASH);
  if(!arr)
    return NULL;
  while(arr[i + 1] != NULL)
    i++;
  filename = ft_strdup(arr[i]);
  ft_free(arr, get_arr_len(arr));
  if(!filename)
    return NULL;
  path = get_executable_path(cmd, filename);
  if(path != NULL)
    return path;
  return NULL;
}
