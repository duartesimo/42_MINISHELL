#include "../includes/minishell.h"

extern int	g_exitcode;

int	errors_exit(int error)
{
	g_exitcode = error;
	ft_putstr_fd("-minishell: ", 2);
	if (error == 1)
		ft_putstr_fd("memory error: unable to assign memory\n", 2);
	else if (error == 2)
		ft_putstr_fd("Failed to create pipe\n", 2);
	else if (error == 3)
		ft_putstr_fd("Failed to fork\n", 2);
	else if (error == 5)
		ft_putstr_fd("outfile: Error\n", 2);
	else if (error == 6)
		ft_putstr_fd("infile: No such file or directory\n", 2);
	return (1);
}

static int	is_number(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (0);
	return (1);
}

static void	get_exit_code(char **str)
{
	int	code;

	if (!str[0])
		code = 0;
	else if (str[0][0] == '#')
		code = g_exitcode;
	else if (str[0][0] == '-' && is_number(str[0] + 1))
		code = 256 - ft_atoi(str[0] + 1);
	else if (str[0][0] == '+' && is_number(str[0] + 1))
		code = ft_atoi(str[0] + 1);
	else if (is_number(str[0]))
		code = ft_atoi(str[0]);
	else
	{
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(str[0], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", 2);
		code = 2;
	}
	free_array(str);
	exit(code);
}

int	ms_exit(t_prompt *prompt, t_command *command)
{
	char	**str;

	if (!command)
	{
		free_data(prompt, false);
		exit(g_exitcode);
	}
	if (!command->prev)
		printf("exit\n");
	if (command->str[1])
	{
		if (is_number(command->str[0]))
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			g_exitcode = 1;
			return (g_exitcode);
		}
	}
	str = duplicate_array(command->str);
	free_data(prompt, false);
	rl_clear_history();
	get_exit_code(str);
	return (0);
}
