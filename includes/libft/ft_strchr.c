#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	while (*str && *str != (char)c)
		str++;
	if ((char)c == *str)
		return ((char *)str);
	return (NULL);
}
