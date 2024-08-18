#include <minishell.h>

size_t get_slash_index(char *cmd)
{
  size_t i;
  size_t index;

  i = 0;
  index = -1;
  while(cmd[i])
  {
    if(cmd[i] == SLASH)
      index = i;
    i++;
  }
  return index;
}

void get_executable(char *cmd)
{
  size_t i;
  char **arr;
  char *filename;
  char *path;
  size_t slash_index;

  i = 0;
  slash_index = 0;
  arr = ft_split(cmd, SLASH);
  if(!arr)
    return;
  while(arr[i + 1] != NULL)
    i++;
  filename = ft_strdup(arr[i]);
  slash_index = get_slash_index(cmd);
  path = ft_strndup(cmd, slash_index + 1);
  printf("filename: %s\npath is: %s\n", filename, path);
  // printf("file name: %s\n", filename);
}
