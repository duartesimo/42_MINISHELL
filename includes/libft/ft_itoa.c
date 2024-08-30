#include "libft.h"

static int	itoa_size(int n)
{
	int			size;
	long long	number;

	number = n;
	size = 0;
	if (number == 0)
		return (1);
	if (number < 0)
	{
		number *= -1;
		size++;
	}
	while (number > 0)
	{
		number /= 10;
		size++;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	char		*string;
	int			size;
	long long	number;

	number = n;
	size = itoa_size(number);
	string = (char *)malloc(sizeof(char) * size + 1);
	if (!string)
		return (NULL);
	string[size] = '\0';
	if (number == 0)
		string[0] = '0';
	if (number < 0)
	{
		string[0] = '-';
		number *= -1;
	}
	while (number > 0)
	{
		string[--size] = number % 10 + '0';
		number /= 10;
	}
	return (string);
}
