/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thisai <thisai@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/25 19:46:18 by thisai            #+#    #+#             */
/*   Updated: 2020/11/01 10:15:14 by thisai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	substr_len(char *p, char c)
{
	int	sublen;

	sublen = 0;
	while (p[sublen] && p[sublen] != c)
		sublen++;
	return (sublen);
}

static int	construct_array(char **dest, int count, char *buf, char c)
{
	int			i;
	size_t		sublen;

	i = 0;
	while (i < count)
	{
		while (*buf && *buf == c)
			buf++;
		sublen = substr_len(buf, c);
		buf[sublen] = '\0';
		dest[i] = ft_strdup(buf);
		if (!dest[i])
		{
			while (i)
				free(dest[--i]);
			return (0);
		}
		i++;
		buf += sublen + 1;
	}
	return (1);
}

int			count_words_and_length(size_t *len, char const *s, char c)
{
	int		count;
	size_t	l;

	l = 0;
	count = 0;
	while (s[l])
	{
		if ((l == 0 || s[l - 1] == c) && s[l] != c)
			count++;
		l++;
	}
	*len = l;
	return (count);
}

char		**ft_split(char const *s, char c)
{
	size_t		len;
	int			count;
	char		**dest;
	char		*buf;

	count = count_words_and_length(&len, s, c);
	buf = ft_strdup(s);
	if (!buf)
		return (NULL);
	if (!(dest = malloc(sizeof(void*) * (count + 1))))
	{
		free(buf);
		return (NULL);
	}
	dest[count] = NULL;
	if (!construct_array(dest, count, buf, c))
	{
		free(dest);
		dest = NULL;
	}
	free(buf);
	return (dest);
}
