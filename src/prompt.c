#include "../includes/minishell.h"

void	restart_prompt(t_prompt *prompt, char **argv, char **env)
{
	char		**old_env;

	old_env = duplicate_array(prompt->env);
	free_data(prompt, true);
	reset_init(prompt, argv, env);
	prompt->reset = true;
	if (prompt->env)
	{
		free_array(prompt->env);
		prompt->env = old_env;
	}
	else
	{
		free_array(old_env);
		prompt->env = NULL;
	}
}

t_prompt	*init_prompt(char **argv, char **env)
{
	t_prompt	*prompt;

	prompt = malloc(sizeof(t_prompt));
	if (!prompt)
		return (NULL);
	prompt->lexer = NULL;
	prompt->heredoc = malloc(sizeof(t_heredoc));
	if (!prompt->heredoc)
		return (NULL);
	prompt->command = init_simple_cmds();
	init_flags(prompt);
	prompt->pid = NULL;
	prompt->env = duplicate_array(env);
	prompt->pwd = getcwd(NULL, 0);
	set_env_vars(prompt, argv);
	prompt->oldpwd = NULL;
	prompt->exit_codes = NULL;
	return (prompt);
}
