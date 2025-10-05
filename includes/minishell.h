/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msengul <msengul@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 06:56:41 by sungor            #+#    #+#             */
/*   Updated: 2025/08/14 23:48:27 by msengul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdlib.h>

extern int				g_signal;

typedef enum e_quote_state
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}						t_quote_state;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_SEMICOLON,
	TOKEN_EOF
}						t_token_type;

typedef struct s_arg
{
	char				*value;
	char				*original_value;
	t_quote_state		quote_state;
}						t_arg;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	t_quote_state		quote_state;
	struct s_token		*next;
}						t_token;

typedef enum e_mode
{
	MAIN_SHELL,
	EXECUTION,
	HEREDOC_PARENT,
	HEREDOC_CHILD
}						t_mode;

typedef struct s_redir
{
	t_token_type		type;
	char				*filename;
	int					expand_heredoc;
	int					heredoc_fd;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	t_arg				**args;
	t_redir				*redirs;
	struct s_command	*next;
}						t_command;

typedef struct s_shell
{
	char				**envp;
	char				*line;
	t_token				*tokens;
	t_command			*cmds;
	int					last_exit_status;
	char				*last_arg;
}						t_shell;

typedef struct s_arg_builder
{
	t_arg				***new_args;
	int					*k;
}						t_arg_builder;

typedef enum e_error_type
{
	ERROR_SYNTAX,
	ERROR_COMMAND_NOT_FOUND,
	ERROR_FILE_NOT_FOUND,
	ERROR_PERMISSION_DENIED,
	ERROR_IS_DIRECTORY,
	ERROR_ALLOCATION,
	ERROR_BUILTIN_CD,
	ERROR_BUILTIN_UNSET,
	ERROR_BUILTIN_EXIT,
	ERROR_QUOTE_UNCLOSED,
	ERROR_PIPE,
	ERROR_REDIRECTION,
	ERROR_TOO_MANY_ARGS,
	ERROR_HOME_NOT_SET,
	ERROR_OLDPWD_NOT_SET,
	ERROR_INVALID_IDENTIFIER,
	ERROR_FILENAME_REQUIRED,
	ERROR_SEMICOLON
}						t_error_type;

typedef enum e_error_code
{
	EXIT_GENERAL_ERROR = 1,
	EXIT_MISUSE = 2,
	EXIT_CANNOT_EXECUTE = 126,
	EXIT_COMMAND_NOT_FOUND = 127
}						t_error_code;

typedef struct s_pipe_info
{
	int					*pipe_fd;
	int					prev_read;
}						t_pipe_info;

typedef struct s_pipeline_state
{
	int					*prev_pipe_read;
	pid_t				*last_pid;
	t_command			*all_cmds;
}						t_pipeline_state;

typedef struct s_exec_result
{
	int					process_count;
	pid_t				last_pid;
	int					prev_pipe_read;
}						t_exec_result;

typedef struct s_fd_info
{
	int					in_fd;
	int					out_fd;
}						t_fd_info;

typedef struct s_child_context
{
	t_shell				*shell;
	t_command			*all_cmds;
}						t_child_context;

t_token					*lexer(char *line);
t_token					*create_new_token(char *value, t_token_type token_type);
void					add_token_back(t_token **token_list,
							t_token *new_token);
int						handle_metachar(t_token **list, char *line, int i);
int						handle_word(t_token **list, char *line, int i);

t_command				*parser(t_token *tokens);
int						handle_redirection(t_command *cmd,
							t_token **current_token);
int						check_syntax_errors(t_token *tokens);
int						parser_simple_command(t_command *cmd,
							t_token **token_stream);
int						count_args(t_token *token);

void					expander(t_command *commands, t_shell *shell);

void					executor(t_command *commands, t_shell *shell);

int						handle_parent_builtins(t_command *cmd, t_shell *shell);

void					*safe_malloc(size_t bytes);
char					*str_append_free(char *s1, char *s2);
void					free_char_array(char **array);
char					**ft_str_array_dup(char **array);
char					**ft_realloc_str_array(char **old_array, char *new_str);
char					**convert_t_arg_to_char_array(t_arg **args);
void					free_token_list(t_token *list);
void					free_command_list(t_command *list);

void					error_exit(const char *error);
int						is_metachar(char c);
int						is_whitespace(char c);
int						is_quote(char c);

int						dispatch_builtin(t_command *cmd, t_shell *shell);
int						builtin_cd(t_command *cmd, t_shell *shell);
int						builtin_echo(t_command *cmd);
int						builtin_env(t_command *cmd, t_shell *shell);
int						builtin_export(t_command *cmd, t_shell *shell);
int						builtin_unset(t_command *cmd, t_shell *shell);

int						builtin_pwd(void);
void					builtin_exit(t_command *cmd, t_shell *shell);
void					setup_signal_handlers(t_mode mode);
char					*find_command_path(char *cmd, t_shell *shell);
void					print_error_and_exit(char *cmd, char *msg, int code);
int						handle_parent_builtins(t_command *cmd, t_shell *shell);
int						handle_heredoc(t_redir *redir, t_shell *shell,
							t_command *all_cmds);
int						process_redirections(t_command *cmd, t_shell *shell,
							int *in_fd, int *out_fd);
void					update_underscore_env(char *path, t_shell *shell);
char					*get_env_var(t_shell *shell, const char *name);
void					set_env_var(t_shell *shell, const char *name,
							const char *value);
char					*get_target_path(t_shell *shell, char *arg,
							int *needs_free);
void					handle_cd_error(char *path);
void					update_pwd_variables(t_shell *shell);
char					**ft_str_array_dup(char **array);
int						is_valid_identifier(const char *name);
void					update_or_add_env(t_shell *shell, const char *arg);
void					set_simple_env(t_shell *shell, const char *arg,
							char *name);
int						find_env_var_index(char **envp, const char *name);
void					recreate_envp_without_var(t_shell *shell,
							int index_to_remove);

void					expander(t_command *commands, t_shell *shell);

char					*expand_argument_value(char *arg_value, t_shell *shell);

int						original_value_was_quoted(const char *original_value);
void					copy_valid_args(t_arg **new_args, t_arg **old_args,
							int count);
void					remove_empty_args(t_command *cmd);
int						count_total_args(t_command *cmd, t_shell *shell);

void					free_split_args(t_arg **split_args, int count);
int						count_single_arg(t_arg *arg, t_shell *shell);

t_arg					**split_expanded_value(const char *expanded_value,
							int *new_count);

int						count_split_parts(const char *str);
char					*extract_word(const char *str, int *index);
char					**split_by_spaces(const char *str);

char					*handle_dollar_expansion(char **arg_ptr,
							t_shell *shell);

char					*handle_single_quotes(char **arg_ptr);
char					*handle_double_quotes(char **arg_ptr, t_shell *shell);

void					init_shell(t_shell *shell, char **envp);

void					sort_str_array(char **arr, int count);
int						process_all_heredocs(t_command *cmds, t_shell *shell);

void					execute_command(t_command *cmd, t_shell *shell,
							t_command *all_cmds);

void					cleanup_and_exit(char **exec_args, t_shell *shell,
							t_command *all_cmds, int exit_code);
void					close_all_heredoc_fds(t_command *commands);
void					close_unused_heredoc_fds(t_command *commands,
							t_command *current);
int						wait_for_children(pid_t last_pid, int process_count);
void					update_final_underscore(t_command *commands,
							t_shell *shell);

void					handle_child_process(t_command *cmd, t_shell *shell,
							t_pipeline_state *state, int *pipe_fd);
void					handle_parent_process(t_command *cmd,
							t_pipeline_state *state, int *pipe_fd);

void					child_routine(t_command *cmd, t_child_context *ctx,
							t_pipe_info *pipe_info);

void					cleanup_heredoc_child(t_shell *shell,
							t_command *all_cmds, int pipe_fd);
void					handle_heredoc_child(int *pipe_fd, t_redir *redir,
							t_shell *shell, t_command *all_cmds);
int						wait_heredoc_child(pid_t pid, int *pipe_fd);
void					close_previous_heredocs(t_command *cmds,
							t_command *stop_at);
void					heredoc_read_loop(int write_fd, t_redir *redir,
							t_shell *shell);

void					print_error(t_error_type type, const char *context);
void					error_exit_with_type(t_error_type type,
							const char *context, int exit_code);
void					print_minishell_error(const char *context,
							const char *message);
void					print_syntax_error(const char *token);
void					print_builtin_error(const char *builtin,
							const char *arg, const char *message);
void					safe_exit(int exit_code);
int						export_error(const char *arg);
void					print_numeric_error(char *arg_val);
void					print_too_many_args_error(void);
int						handle_invalid_option(const char *option);
int						process_heredoc_line(char *line, t_redir *redir,
							t_shell *shell, int write_fd);
int						check_overflow(const char *str);
int						char_compare(char c1, char c2);
int						compare_digits(const char *str, int i, char *limit);
int						compare_with_limits(const char *str, int i, int sign);

#endif