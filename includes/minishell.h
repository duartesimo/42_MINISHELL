#ifndef MINISHELL_H
# define MINISHELL_H

# include "../includes/libft/libft.h"
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <sys/types.h>
# include <fcntl.h>
# include <signal.h>
# include <limits.h>
# include <sys/stat.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/ioctl.h>

typedef enum e_sig_state
{
	SIG_DEFAULT = 1,
	SIG_HEREDOC,
	SIG_CHILD,
	SIG_PARENT
}			t_sig_state;

typedef enum s_tokens
{
	IDENTIFIER,
	PIPE,
	GREAT,
	GREAT_GREAT,
	LESS,
	LESS_LESS,
}			t_tokens;

typedef struct s_heredoc
{
	int		err_num;
	bool	status;
}			t_heredoc;

typedef struct s_lexer
{
	char			*str;
	t_tokens		token;
	struct s_lexer	*prev;
	struct s_lexer	*next;
}			t_lexer;

typedef struct s_command
{
	char					**str;
	char					*builtin;
	char					*heredoc_name;
	t_lexer					*redirect;
	int						num_redirections;
	struct s_command		*prev;
	struct s_command		*next;
}			t_command;

typedef struct s_prompt
{
	t_command	*command;
	t_lexer		*lexer;
	t_heredoc	*heredoc;
	int			*pid;
	int			*exit_codes;
	int			flag1;
	int			flag2;
	int			flag3;
	int			flag4;
	char		**env;
	char		*pwd;
	char		*oldpwd;
	bool		reset;
	bool		interact;
}			t_prompt;

typedef struct s_expansion
{
	char		**word;
	int			*len;
	int			*index;
	t_prompt	*prompt;
	int			start;
	int			end;
}			t_expansion;

typedef struct s_processing
{
	char		**str;
	int			*len;
	int			index;
	bool		in_single_quote;
	bool		in_double_quote;
	t_prompt	*prompt;
}	t_processing;

/* PROMPT */
t_prompt		*init_prompt(char **argv, char **env);
void			restart_prompt(t_prompt *prompt, char **argv, char **env);
void			run_shell_loop(t_prompt *prompt, char **argv, char **env);

/* INIT */
t_command		*init_simple_cmds(void);
int				init_pid(t_prompt *prompt);
void			init_flags(t_prompt *prompt);
void			set_env_vars(t_prompt *prompt, char **argv);
t_prompt		*reset_init(t_prompt *prompt, char **argv, char **env);

/* SIGNALS */
void			set_signal_handlers(int state);

/* EXPANDER */
void			expand_env_vars(t_prompt *prompt, char **str);
void			handle_expand(t_prompt *prompt, char **word, int *len, int *i);
bool			is_valid_var(t_prompt *prompt, const char *var_name);
char			*swap_exitmark(t_prompt *prompt, char **str, int *len, int *i);
char			*replace(t_prompt *prompt, char *str);
char			*get_word(char *str);
char			*create_str(t_prompt *prompt, char *str, char *var, char *word);
int				key_len(char *str);

/* LEXER */
t_lexer			*lexer(char *str);
int				add_identifier(t_lexer **lexer, char **str, int i);
bool			skip_quotes(char *line, int *i);
void			delete_quotes(char *str);
void			delete_lexer(t_lexer **lexer);
t_lexer			*get_last_node(t_lexer *node);
void			add_node(t_lexer **lexer, t_lexer *node);
t_lexer			*create_node(char *str, t_tokens token);

/* PARSER */
void			parser(t_prompt *prompt);
int				count_lexer_nodes(t_lexer *lexer);
void			process_identifier(t_prompt *prompt, t_command *command);
bool			is_redirection_or_pipe(t_prompt *prompt);
int				is_redirection_token(t_tokens token);

/* EXECUTE */
int				execute(t_prompt *prompt);
int				single_command(t_prompt *prompt, t_command *command);
int				execute_command(t_prompt *prompt, t_command *command);
int				execute_system_command(t_prompt *prompt, t_command *command);
int				check_fd_heredoc(t_prompt *prompt, t_command *cmd, int end[2]);
int				dup_process(t_prompt *prompt, t_command *command,
					int fd_in, int end[2]);
int				configure_redirections(t_command *command);
int				validate_tokens(t_prompt *prompt);
int				print_command_error(t_command *command);
void			free_exit(t_prompt *prompt);

/* HEREDOCS */
t_command		*single_command_heredoc(t_command *command);
int				process_heredoc(t_prompt *prompt, t_command *command);
int				make_heredoc(t_prompt *prompt, t_lexer *lexer,
					bool quotes, char *name);

/* BUILTINS */
int				builtin(t_prompt *prompt, t_command *command);
int				is_builtin_command(char *str);
int				ms_cd(t_prompt *prompt, t_command *command);
int				ms_export(t_prompt *prompt, t_command *command);
int				ms_unset(t_prompt *prompt, t_command *commmand);
int				ms_exit(t_prompt *prompt, t_command *command);
void			ms_echo(t_prompt *prompt, t_command *command);
void			ms_env(t_prompt *prompt);

/* CD */
void			change_path(t_prompt *prompt);
void			add_path_env(t_prompt *prompt);

/* EXPORT */
void			add_or_update_env(t_prompt *prompt, char *cmd);
char			**add_key_only(char **env, char *key);
void			update_env_value(t_prompt *prompt, t_command *cmds, int i);
int				key_exists(t_prompt *prompt, t_command *cmds, int i);
int				search_equal_sign(t_prompt *prompt, t_command *cmds, int i);
int				ft_strcmp(const char *s1, const char *s2);

/* ECHO */
int				get_exit_stat(t_prompt *prompt);
bool			dollar_question(char *str);

/* ENV */
char			*ms_getenv(char **env, char *var);
char			**ms_setenv(char *variable, char *value, char **env);

/* EXIT */
int				errors_exit(int error);

/* FREE */
void			free_data(t_prompt *prompt, bool restart);
void			free_array(char **array);

/* UTILS */
char			*char_to_str(char c);
char			**duplicate_array(char **arr);
char			*remove_quotes(char *str, char quote);
int				equal_sign(char *str);
int				dollar_sign(char *str);
bool			has_dollar_sign(char *str, int i);
bool			solo_dollar_sign(const char *str);
bool			doll_between_quotes(char *str);
char			*ft_strndup(const char *s, size_t n);

/* IS SOMETHING */
int				is_quote(char c);
int				is_separator(char *c);
int				ft_isspace(char c);
int				is_invalid_char(char c);
int				if_digit(char *str, int i);

#endif
