#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	size_t		i;
	char		*copy;

	copy = (char *)str;
	i = 0;
	while (i < n)
	{
		copy[i] = c;
		i++;
	}
	return (str);
}
