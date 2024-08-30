#include "../includes/minishell.h"

extern int	g_exitcode;

static void	print_exit_stat(char *str)
{
	int		i;
	char	*code;

	i = 0;
	while (str[i] && str[i] != '$')
	{
		ft_putchar_fd(str[i], 1);
		i++;
	}
	if (str[i] == '$' && str[i + 1] == '?')
	{
		code = ft_itoa(g_exitcode);
		if (code)
		{
			printf("%s", code);
			free(code);
		}
		i += 2;
	}
	while (str[i])
	{
		ft_putchar_fd(str[i], 1);
		i++;
	}
}

int	get_exit_stat(t_prompt *prompt)
{
	int	i;

	i = 0;
	if (!prompt || !prompt->exit_codes)
		return (-1);
	while (prompt->exit_codes[i] == 2)
		i++;
	return (i);
}

bool	dollar_question(char *str)
{
	while (*str)
	{
		if (*str == '$' && *(str + 1) == '?')
			return (true);
		str++;
	}
	return (false);
}

static void	print_echo(t_prompt *prompt, char **args, int i)
{
	while (args[i])
	{
		if (dollar_question(args[i]))
		{
			if (prompt->exit_codes[get_exit_stat(prompt)] == 1)
				print_exit_stat(args[i]);
			else
				printf("%s", args[i]);
			prompt->exit_codes[get_exit_stat(prompt)] = 2;
		}
		else
			printf("%s", args[i]);
		i++;
		if (args[i])
			printf(" ");
	}
}

void	ms_echo(t_prompt *prompt, t_command *command)
{
	char	**args;
	int		i;
	int		j;
	int		flag_n;

	i = 0;
	flag_n = 0;
	args = command->str;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
			flag_n = 1;
		else
			break ;
		i++;
	}
	print_echo(prompt, args, i);
	if (!flag_n)
		printf("\n");
}
