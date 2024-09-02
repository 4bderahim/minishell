#include "minishell.h"

size_t ft_strlen(char *str)
{
  size_t i;

  i = 0;
  if(!str)
    return (i);
  while (str[i])
    i++;
  return i;  
}

char *ft_strdup(char *str)
{
  char *buf;
  int i;

  i = 0;
  if(!str)  
    return NULL;
  buf = malloc(sizeof(char) * (ft_strlen(str) + 1));
  while(str[i])
  {
    buf[i] = str[i];
    i++;
  }
  buf[i] = '\0';
  return buf;
}

char	*ft_substr(char   *s, unsigned int start, size_t len)
{
	int				i;
	unsigned int	slen;
	char			*res;

	i = 0;
	slen = (unsigned int)ft_strlen(s);
	if (!s || start > slen)
		return (ft_strdup(""));
	if (len > (size_t)slen || (len < (size_t)slen && len
			+ (size_t)start > (size_t)slen))
		len = (size_t)slen - (size_t)start;
	res = (char *)malloc(sizeof(char) * (len) + 1);
	if (res == NULL)
		return (NULL);
	while (i < (int)len && s[start + i] != '\0')
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*res;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if(!s1 && s2)
		return ft_strdup(s2);
	else if(!s2 && s1)
		return ft_strdup(s1);
	else if (!s1 && !s2)
		return NULL;
	res = (char *)malloc(s1_len + s2_len + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1, s1_len + 1);
	ft_strlcpy(res + s1_len, s2, s1_len + s2_len + 1);
	free(s1);
	s1 = NULL;
	return (res);
}

size_t	ft_strlcpy(char *dst,   char *src, size_t dstsize)
{
	size_t	src_size;
	size_t	i;

	i = 0;
	src_size = ft_strlen(src);
	if (dstsize)
	{
		while (src[i] && i < dstsize - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (src_size);
}

int ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char *ft_strncpy(char *str, int size)
{
  int i;
  char *result;

  i = 0;
  result = malloc(sizeof(char) * size + 1);
  while (i < size)
  {
    result[i] = str[i];
    i++;
  }
  result[i] = '\0';
  return result;
}

int ft_isalpha(char c)
{
	if((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return 1;
	return 0;
}

bool search_for_2nd_quote(char *str, char c)
{
	int i;
	int counter;

	i = 0;
	counter = 0;
	while(str[i])
	{
		if(str[i] == c)
			counter++;
		i++;
	}
	if(counter % 2 == 1)
		return true;
	return false;
}

int ft_strchr(char *str, char c)
{
	int i;
	int in_quotes;
	int count_quotes;

	i = 0;
	in_quotes = 1;
	count_quotes = 0;
	if(!search_for_2nd_quote(str, c))
		return -1;
	while(str[i])
	{
		if(!in_quotes && (str[i] == SPACE || str[i] == PIPE))
			return i;
		if(str[i] == c)
		{
			if(in_quotes == 1)
				in_quotes = 0;
			else
				in_quotes = 1;
		}
		i++;
	}
	return i;
}

int ft_strchr_pro(char *str, char c1, char c2, bool inside_quotes)
{
	int i;

	i = 0;
	if(!str)
		return -1;
	while(str[i] && str[i] != c1 && str[i] != c2)
	{
		i++;
		if(str[i] == '\0')
			return -1;
	}
	return i + 1;
}

char *ft_strndup(char *str, size_t n)
{
	size_t i;
	char *result;

	if(!str)
		return NULL;
	i = 0;
	result = (char *)malloc(n + 1);
	while(i < n)
	{	
		result[i] = str[i];
		i++;
	}
	result[i] = '\0';
	return result;
}
