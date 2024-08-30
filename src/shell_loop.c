#include "../includes/minishell.h"

static void	prepare_exitcodes(t_prompt *prompt, char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
			count++;
		i++;
	}
	prompt->exit_codes = malloc(count * sizeof(int));
	if (!prompt->exit_codes)
		return ;
	ft_memset(prompt->exit_codes, 0, count * sizeof(int));
}

static bool	only_spaces(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!ft_isspace(str[i]))
			return (0);
	return (1);
}

static char	*handle_input(t_prompt *prompt, char *str)
{
	if (str == NULL)
	{
		printf("exit\n");
		ms_exit(prompt, NULL);
	}
	if (str[0] && !only_spaces(str))
		add_history(str);
	if (str[0])
	{
		prompt->flag3 = 0;
		prepare_exitcodes(prompt, str);
		if (!prompt->exit_codes)
			return (NULL);
		expand_env_vars(prompt, &str);
	}
	return (str);
}

void	run_shell_loop(t_prompt *prompt, char **argv, char **env)
{
	char	*input;

	while (prompt)
	{
		prompt->interact = true;
		input = readline("\001\e[1;32m\002minishell$ \001\e[0m\002");
		prompt->interact = false;
		input = handle_input(prompt, input);
		prompt->lexer = lexer(input);
		if (!prompt->lexer)
		{
			restart_prompt(prompt, argv, env);
			continue ;
		}
		if (prompt->lexer && !validate_tokens(prompt))
		{
			parser(prompt);
			if (prompt->command && !init_pid(prompt))
				execute(prompt);
		}
		restart_prompt(prompt, argv, env);
	}
}
