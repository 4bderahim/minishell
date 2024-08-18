#include "minishell.h"

static  size_t  count_words(  char *s, char del)
{
  size_t  i;
  size_t  words;

  i = 0;
  words = 0;
  while (s[i])
  {
    while (s[i] && s[i] == del)
      i++;
    if (s[i] && s[i] != del)
      words++;
    while (s[i] && s[i] != del)
      i++;
  }
  return (words);
}

static  size_t  wordlen(char *s, char c)
{
  size_t  i;

  i = 0;
  while (s[i] != '\0' && s[i] != c)
    i++;
  return (i);
}

static  char    **set_free(char **res, size_t i)
{
  size_t  j;

  j = 0;
  while (j < i)
  {
    free(res[j]);
    j++;
  }
  free(res);
  return (NULL);
}

char    **ft_split(char   *s, char c)
{
  char    **res;
  size_t  words;
  size_t  i;

  if (!s)
    return (NULL);
  i = 0;
  words = count_words(s, c);
  res = (char **)malloc(sizeof(char *) * (words + 1));
  if (!res)
          return (NULL);
  while (i < words)
  {
    while (*s != '\0' && *s == c)
            s++;
    res[i] = ft_substr(s, 0, wordlen((char *)s, c));
    s += wordlen((char *)s, c);
    if (!res[i])
            return (set_free(res, i));
    i++;
  }
  res[i] = NULL;
  return res;
}