/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:59 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/07 17:41:46 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define SIG_NONE 0;
# define SIG_INT 1;
# define SIG_QUIT 2;

# define INT_MAX 2147483647
# define INT_MIN (-2147483648)
# define LLONG_MAX 9223372036854775807LL

typedef struct s_heredoc_info
{
	char			*temp_filename;
	char			*original_delimiter;
	int				heredoc_id;
	struct s_heredoc_info *next; // Linked list için
}					t_heredoc_info;

typedef enum e_quote_type
{
	DOUBLE_QUOTE,
	SINGLE_QUOTE,
	NONE_QUOTE
}					t_quote_type;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_QUOTED_WORD,
	TOKEN_ENV_VAR
}					t_token_type;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
	t_quote_type	q_type;
}					t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}					t_env;

typedef struct s_parse_data
{
	int				quoted;
	t_quote_type	detected;
}					t_parse_data;

typedef struct s_base
{
	t_token			*token;
	t_env			*env;
	int				exit_status;
	t_heredoc_info	*heredocs;
	int				heredoc_count;
}					t_base;

typedef struct s_exec_params
{
	char			**argv;
	char			**envp;
	char			*command_path;
}					t_exec_params;

typedef struct s_exec_data
{
	int				cmd_count;
	int				pipe_count;
	int				**pipes;
	t_token			**commands;
	pid_t			*pids;
	t_base			*base;
}					t_exec_data;

typedef struct s_shell_signal
{
	int				shell_level;
	int				in_heredoc;
	int				in_child;
	int				last_exit_status;
}					t_shell_signal;

/* Tokenizer functions */
char				*parse_word_with_quotes(char *input, int *i,
						t_token_type *type, t_quote_type *q_type);
void				tokenize_input(char *input, t_base *base);
void				add_token(t_token **head, char *str, t_token_type type,
						t_quote_type q_type);
int					is_special(char c);

/* Parser functions */
void				handle_word_or_error(char *input, int *i, t_token **head);
void				handle_operator(char *input, int *i, t_token **head);
char				*handle_quoted_word(char *input, int *i, char *result);
char				*handle_unquoted_word(char *input, int *i, char *result);
t_quote_type		detect_first_quote_type(char *input, int start, int end);
int					is_fully_quoted(char *str, char quote_char);
t_quote_type		get_quote_type(char *str);
char				*ft_strjoin_free(char *s1, char *s2);

/* Expand functions */
void				expand_tokens(t_base *base);
void				word_splitting(t_base *base);
char				*expand_variables(char *str, t_base *base);
char				*process_mixed_quotes(char *str, t_base *base);
int					has_dollar_sign(char *str);
int					has_tilde_sign(char *str);
void				delete_null_tokens(t_base *base);
t_token				*split_string_to_tokens(char *str);
void				free_token_content(t_token *token);
int					should_split_token(t_token *token);
char				*find_env_value(t_base *base, char *key);
char				*extract_var_name(char *str, int *len);
int					is_ifs_char(char c);
void				skip_ifs_chars(char *str, int *i);
char				*extract_word(char *str, int *i);
void				skip_ifs_chars(char *str, int *i);
int					is_only_empty_variable(char *str, t_base *base);
t_token				*get_last_token(t_token *tokens);

/* Environment functions */
t_env				*create_new_node(char *env);
void				add_new_node(t_env **head, t_env *new_node);
t_env				*init_env(char **env);
char				*get_env_value(t_base base, char *key);
void				update_shlvl(t_base *base);
void				env_null_check(t_env *env_lst);
int					is_valid_var_char(char c, int first);

/* Utility functions */
int					ft_isspace(char c);
char				*join_and_free(char *s1, char *s2);
char				*get_value(t_env *env, char *key);

/* ====== HEREDOC_CHILD.C ====== */
void				setup_heredoc_signals(void);
int					should_expand_heredoc(char *delimiter);
char				*remove_quotes_from_delimiter(char *delimiter);
char				*read_heredoc_input_child(char *delimiter, t_base *base,
						int expand);

/* ====== HEREDOC_UTILS.C ====== */
char				*expand_heredoc_line(char *line, t_base *base, int expand);
char				*join_heredoc_content(char *content, char *line);
char				*process_heredoc_line(char *line, char *content,
						t_base *base, int expand);
char				*finalize_heredoc_content(char *content);
int					process_readline_input(char **line, char *clean_delimiter,
						char **content, t_base *base);

/* ====== HEREDOC_LIST.C ====== */
t_heredoc_info		*create_heredoc_node(int heredoc_id);
void				add_heredoc_to_list(t_heredoc_info **head,
						t_heredoc_info *new_node);
t_heredoc_info		*find_heredoc_by_id(t_heredoc_info *head, int id);
void				cleanup_heredocs_list(t_heredoc_info *head);
void				cleanup_failed_heredocs_list(t_heredoc_info *head);

/* ====== HEREDOC_PROCESS.C ====== */
char				*create_temp_filename(int heredoc_id);
int					write_content_to_temp_file(char *filename, char *content);
char				*create_heredoc_placeholder(int heredoc_id);
int					count_heredocs(t_token *token);
int					preprocess_heredocs(t_base *base);

/* ====== HEREDOC_FORK.C ====== */
void				cleanup_child_resources(t_base *base);
int					handle_child_process(int pipefd[2], char *delimiter,
						t_base *base);
int					handle_parent_process(int pipefd[2], pid_t pid,
						t_base *base);
char				*read_from_pipe(int fd);
char				*run_heredoc_child_to_file(char *delimiter, t_base *base,
						t_heredoc_info *info_node);

/* ====== HEREDOC_COLLECT.C ====== */
int					setup_heredoc_node(t_token *heredoc_token,
						t_heredoc_info *info_node);
int					create_temp_file_from_content(t_heredoc_info *info_node,
						char *content);
int					create_placeholder_token(t_token *heredoc_token,
						t_heredoc_info *info_node);
int					process_single_heredoc_child(t_token *heredoc_token,
						t_heredoc_info *info_node, t_base *base);
int					collect_heredocs_child(t_token *token, t_base *base);

/* ====== HEREDOC_MANAGER.C ====== */
int					create_heredoc_list(t_token *token, t_base *base);
int					process_heredoc_list(t_token *token, t_base *base);
int					is_heredoc_placeholder(char *str);
int					extract_heredoc_id(char *placeholder);
void				cleanup_heredocs(t_base *base);

/* ====== HEREDOC_RESTORE.C ====== */
int					open_heredoc_file(t_heredoc_info *heredoc_node);
void				setup_heredoc_input(t_token *heredoc_token, t_base *base);
t_token				*find_last_heredoc(t_token *cmd);
void				restore_heredocs_in_redirections(t_token *cmd,
						t_base *base);
char				*initialize_empty_content_safe(void);

/* Core execution functions */
void				execute_command(t_base *base);
int					single_execute_command(t_base *base);
int					execute_multiple_command(t_base *base);
int					execute_external_command(t_base *base);

/* Multiple command execution utility functions */
int					count_commands(t_token *token);
int					**create_pipes(int pipe_count);
t_token				**split_commands(t_token *token, int cmd_count);
void				cleanup_pipes(int **pipes, int pipe_count);
int					handle_redirections(t_token *cmd, t_base *base);
int					has_output_redirection(t_token *cmd);
int					has_input_redirection(t_token *cmd);
void				redirect_control(t_exec_data *data, int ctrl);
char				*initialize_empty_content_safe(void);

/* Multiple command execution helper functions */
void				init_exec_data(t_exec_data *data);
void				free_tokens_safe(t_exec_data *data);
void				free_pids(t_exec_data *data);
int					wait_for_children_utils(int last_exit_status, int status,
						t_exec_data *data);

/* Execution utility functions */
int					is_redirection_token(t_token_type type);
int					is_special_token(t_token_type type);
int					has_special_tokens(t_token *token);
char				*find_command_path(char *command, t_base *base);
int					check_build_in(char *input);
int					prepare_execution(t_token *token, t_base *base,
						t_exec_params *params);

/* Token segment functions */
t_token				*copy_prompt_segment(t_token *start, t_token *end);

/* Argument and environment preparation */
char				**build_argv_from_tokens(t_token *token);
char				**env_to_envp(t_env *env);
int					count_env_vars(t_env *env);
void				free_argv(char **argv);

/* Built-in functions */
void				ft_build_in(t_token *current_prompt, t_base *base);
int					ft_echo(t_token *current_prompt, t_base base);
t_base				*ft_cd(t_token *current_prompt, t_base *base);
int					ft_pwd(t_base base);
t_base				*ft_export(t_token *token, t_base *base);
int					ft_env(t_base *base);
t_base				*ft_unset(t_token *token, t_base *base);
void				ft_exit(t_token *current_prompt, t_base *base);
int					is_numeric(char *str);
int					unset_env_var(t_env **env, char *key);

/* Environment manipulation */
void				set_env_value(t_env **env, char *key, char *value);
char				*get_home_dir(t_env *env);
char				*get_oldpwd_dir(t_env *env);

/* Export utility functions */
int					has_equal_sign(char *content);
char				*parse_token(t_token *token);
char				*parse_value(t_token *token);
int					is_valid_identifier(char *key);
t_env				*find_env_var(t_env *env, char *key);
char				*create_env_string(char *key, char *value);
t_env				**create_env_array(t_env *env, int count);
void				add_env_var(t_token *token, t_base *base);
void				update_env_var(t_env *env_var, t_token *token);
int					validate_and_error(char *key, char *content);
int					add_or_update_env(t_token *token, t_base *base);
int					ascii_strcmp(char *s1, char *s2);
void				sort_env_array(t_env **array, int count);
int					has_equal_sign_in_original(t_env *env);
void				print_export_var(t_env *env);
void				print_sorted_export(t_env *env);
void				sort_and_print_env(t_env *env);
int					process_export_args(t_token *token, t_base *base);

/* Syntax checking functions */
int					check_syntax_errors(t_token *token);
int					check_pipe_syntax(t_token *token);
int					check_redirection_syntax(t_token *token);
void				print_syntax_error(char *token);
int					is_operator_token(t_token_type type);
int					check_heredoc_syntax(t_token *token);

/* Signal handling functions */
void				setup_interactive_signals(void);
void				setup_child_signals(void);
void				setup_execution_signals(void);
int					check_signal_status(int exit_status);
void				restore_signals(void);
void				sigint_handler(int sig);

/* Cleanup functions */
void				free_tokens(t_token *tokens);
void				free_env_list(t_env *env);
void				free_string_array(char **array);
void				cleanup_all(t_base *base);
void				free_commands(t_token **commands);
void				free_child_arg(t_exec_data *data);

#endif