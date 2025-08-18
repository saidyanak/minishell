/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:59 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/18 21:18:27 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define INT_MAX 2147483647
# define INT_MIN -2147483648
# define LLONG_MAX 9223372036854775807

# include <sys/types.h>

typedef struct s_heredoc_info
{
	char					*temp_filename;
	char					*original_delimiter;
	int						heredoc_id;
	struct s_heredoc_info	*next;
}							t_heredoc_info;

typedef enum e_quote_type
{
	DOUBLE_QUOTE,
	SINGLE_QUOTE,
	NONE_QUOTE
}							t_quote_type;

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
}							t_token_type;

typedef struct s_token
{
	char					*content;
	t_token_type			type;
	struct s_token			*next;
	t_quote_type			q_type;
}							t_token;

typedef struct s_env
{
	char					*key;
	char					*value;
	int						exported;
	struct s_env			*next;
}							t_env;

typedef struct s_parse_data
{
	int						quoted;
	t_quote_type			detected;
}							t_parse_data;

typedef struct s_base
{
	t_token					*token;
	t_env					*env;
	int						exit_status;
	t_heredoc_info			*heredocs;
	int						heredoc_count;
	struct s_exec_data		*data;
}							t_base;
typedef struct s_exec_params
{
	char					**argv;
	char					**envp;
	char					*command_path;
}							t_exec_params;

typedef struct s_exec_data
{
	int						cmd_count;
	int						pipe_count;
	int						**pipes;
	t_token					**commands;
	pid_t					*pids;
	t_base					*base;
}							t_exec_data;

typedef struct s_shell_signal
{
	int						shell_level;
	int						in_heredoc;
	int						in_child;
	int						last_exit_status;
}							t_shell_signal;

typedef struct s_heredoc_data
{
	int						should_expand;
	char					*clean_delimiter;
	int						line_count;
	t_base					*base;
	int						pipefd;
}							t_heredoc_data;

void						debug_all(t_base *base);
void						print_tokens(t_token *token);
t_token						*extract_command_tokens(t_token *token);
char						*initialize_empty_content_safe(void);
char						*parse_word_with_quotes(char *input, int *i,
								t_token_type *type, t_quote_type *q_type);
void						tokenize_input(char *input, t_base *base);
void						add_token(t_token **head, char *str,
								t_token_type type, t_quote_type q_type);
int							is_special(char c);

void						handle_word_or_error(char *input, int *i,
								t_token **head);
void						handle_operator(char *input, int *i,
								t_token **head);
char						*handle_quoted_word(char *input, int *i,
								char *result);
char						*handle_unquoted_word(char *input, int *i,
								char *result);
t_quote_type				detect_first_quote_type(char *input, int start,
								int end);
int							is_fully_quoted(char *str, char quote_char);
t_quote_type				get_quote_type(char *str);
char						*ft_strjoin_free(char *s1, char *s2);

void						expand_tokens(t_base *base);
void						word_splitting(t_base *base);
char						*expand_variables(char *str, t_base *base);
char						*process_mixed_quotes(char *str, t_base *base);
int							has_dollar_sign(char *str);
int							has_tilde_sign(char *str);
void						delete_null_tokens(t_base *base);
t_token						*split_string_to_tokens(char *str);
void						free_token_content(t_token *token);
int							should_split_token(t_token *token);
char						*find_env_value(t_base *base, char *key);
char						*extract_var_name(char *str, int *len);
int							is_ifs_char(char c);
void						skip_ifs_chars(char *str, int *i);
char						*extract_word(char *str, int *i);
void						skip_ifs_chars(char *str, int *i);
int							is_only_empty_variable(char *str, t_base *base);
void						swap_token(t_token *token);
t_token						*get_last_token(t_token *tokens);
void						restore_heredocs_in_redirections(t_token *cmd,
								t_base *base);

t_env						*create_new_node(char *env);
int							check_redirection(t_token *token);
void						add_new_node(t_env **head, t_env *new_node);
int							allocate_key_value(t_env *node, char *env,
								char *equal_sign);
void						cleanup_node(t_env *node);
t_env						*init_env(char **env);
t_env						*create_minimal_env(void);
char						*get_env_value(t_base base, char *key);
void						update_shlvl(t_base *base);
int							is_valid_var_char(char c, int first);
void						set_underscore_variable(t_base *base,
								t_token *token);

char						*check_return(char *line);
void						cleanup_heredocs(t_base *base);
void						process_all_heredocs(t_base *base);
void						restore_heredocs_in_redirections(t_token *cmd,
								t_base *base);
void						cleanup_child_resources(t_base *base);
int							get_unique_heredoc_id(void);
char						*unique_name_return(char *temp, char *counter_str,
								char *temp_file);
char						*heredoc_parent_process(char *content, int *pipefd,
								pid_t pid, t_base *base);
char						*expand_heredoc_var(char *line, int *i,
								t_base *base);
char						*expand_heredoc_line(char *line, t_base *base,
								int should_expand);
int							should_expand_delimiter(char *delimiter);
char						*remove_quotes_from_delimiter(char *delimiter);
char						*create_temp_filename(void);

void						heredoc_child_init(int pipefd[2], char *delimiter,
								t_base *base, t_heredoc_data *data);
int							heredoc_check_exit_conditions(char *line,
								char *clean_delimiter,
								t_heredoc_data *data);
void						heredoc_process_line(char *line,
								t_heredoc_data *data);
void						heredoc_child_process(int pipefd[2],
								char *delimiter,
								t_base *base);
char						*process_heredoc(char *delimiter, t_base *base);

char						*init_heredoc_content(int pipefd[2], pid_t pid,
								t_base *base);
char						*read_heredoc_buffer(int pipefd, char *content);
int							handle_child_exit_status(pid_t pid, t_base *base);
char						*read_heredoc_content(int pipefd[2], pid_t pid,
								t_base *base);
t_heredoc_info				*create_heredoc_info(char *temp_file,
								char *delimiter);

char						*get_heredoc_content(t_token *token, t_base *base);
int							create_heredoc_file(char *content,
								char **temp_file);
int							save_heredoc_info(char *temp_file, t_token *token,
								t_base *base);
void						update_token_content(t_token *token,
								char *temp_file);
int							handle_heredoc_token(t_token *token, t_base *base);

int							ft_isspace(char c);
char						*join_and_free(char *s1, char *s2);
char						*get_value(t_env *env, char *key);

char						*process_heredoc(char *delimiter, t_base *base);
int							handle_heredoc_token(t_token *token, t_base *base);
void						cleanup_heredocs(t_base *base);

void						cleanup_tokens_and_heredocs(t_base *base);
void						init_base_struct(t_base *base, char **env);

char						*ft_strjoin_free(char *s1, char *s2);
int							*heredoc_static_flag(int control);
char						*initialize_empty_content_safe(void);

void						execute_command(t_base *base);
void						swap_redirect_tokens(t_token **head);
int							single_execute_command(t_base *base);
int							execute_multiple_command(t_base *base);
char						*expand_variables(char *str, t_base *base);
int							execute_external_command(t_base *base);
int							handle_heredoc_processing(t_base *base);
int							handle_heredoc_processing(t_base *base);
int							check_heredoc(t_token *token);
int							expand_dollar_var(char *str, int i, char **result,
								t_base *base);
void						save_standard_fds(int *saved_stdin,
								int *saved_stdout);
void						restore_standard_fds(int saved_stdin,
								int saved_stdout);
int							handle_redirection_execution(t_base *base);

int							count_commands(t_token *token);
int							**create_pipes(int pipe_count);
t_token						**split_commands(t_token *token, int cmd_count);
void						cleanup_pipes(int **pipes, int pipe_count);
int							handle_redirections(t_token *cmd, t_base *base);
int							has_output_redirection(t_token *cmd);
int							has_input_redirection(t_token *cmd);
void						redirect_control(t_exec_data *data, int ctrl);
int							redirect_out(t_token *current, int fd);
int							redirect_append(t_token *current, int fd);
int							redirect_in(t_token *current, int fd);
t_token						*find_last_heredoc(t_token *cmd);
int							handle_heredoc_redirection(t_token *current);
int							process_redirection_token(t_token *current, int fd,
								t_token *last_heredoc, int error);
int							next_command_has_input_redirect(t_exec_data *data,
								int cmd_index);
int							should_skip_command_execution(t_exec_data *data,
								int cmd_index);
int							launch_child_processes(t_exec_data *data);
int							wait_for_children(t_exec_data *data);
int							execute_child_process(t_token *cmd,
								t_exec_data *data,
								int cmd_index);
void						setup_child_redirections(t_token *cmd,
								t_exec_data *data,
								int cmd_index);

void						init_exec_data(t_exec_data *data);
void						free_tokens_safe(t_exec_data *data);
void						free_pids(t_exec_data *data);
int							wait_for_children_utils(int last_exit_status,
								int status, t_exec_data *data);

int							is_redirection_token(t_token_type type);
int							is_special_token(t_token_type type);
int							has_special_tokens(t_token *token);
char						*find_command_path(char *command, t_base *base);
int							check_build_in(char *input);
int							prepare_execution(t_token *token, t_base *base,
								t_exec_params *params);

char						**build_argv_from_tokens(t_token *token);
char						**env_to_envp(t_env *env);
int							count_env_vars(t_env *env);
void						free_argv(char **argv);

void						ft_build_in(t_token *current_prompt, t_base *base);
int							ft_echo(t_token *current_prompt, t_base base);
t_base						*ft_cd(t_token *current_prompt, t_base *base);
int							count_cd_arguments(t_token *token);
int							ft_pwd(t_base base);
t_base						*ft_export(t_token *token, t_base *base);
int							ft_env(t_base *base);
t_base						*ft_unset(t_token *token, t_base *base);
void						ft_exit(t_token *current_prompt, t_base *base);
int							is_numeric(char *str);
int							unset_env_var(t_env **env, char *key);

void						set_env_value(t_env **env, char *key, char *value);
char						*get_home_dir(t_env *env);
char						*get_oldpwd_dir(t_env *env);

int							has_equal_sign(char *content);
char						*parse_token(t_token *token);
char						*parse_value(t_token *token);
int							is_valid_identifier(char *key);
t_env						*find_env_var(t_env *env, char *key);
char						*create_env_string(char *key, char *value);
t_env						**create_env_array(t_env *env, int count);
void						add_env_var(t_token *token, t_base *base);
void						update_env_var(t_env *env_var, t_token *token);
int							validate_and_error(char *key, char *content);
int							add_or_update_env(t_token *token, t_base *base);
int							ascii_strcmp(char *s1, char *s2);
void						sort_env_array(t_env **array, int count);
int							has_equal_sign_in_original(t_env *env);
void						print_export_var(t_env *env);
void						print_sorted_export(t_env *env);
void						sort_and_print_env(t_env *env);
int							process_export_args(t_token *token, t_base *base);

int							check_syntax_errors(t_token *token);
int							check_pipe_syntax(t_token *token);
int							check_redirection_syntax(t_token *token);
void						print_syntax_error(char *token);
int							is_operator_token(t_token_type type);
int							check_heredoc_syntax(t_token *token);

void						setup_interactive_signals(void);
void						setup_child_signals(void);
void						setup_execution_signals(void);
void						heredoc_sigint(int sig);
int							check_signal_status(int exit_status);
void						restore_signals(void);
void						sigint_handler(int sig);

void						free_tokens(t_token *tokens);
void						free_env_list(t_env *env);
void						free_string_array(char **array);
void						cleanup_all(t_base *base);
void						free_commands(t_token **commands);
void						free_child_arg(t_exec_data *data);
void						free_child_env_and_commands(t_exec_data *data);
void						cleanup_base_normal(t_base *base);
void						cleanup_base_with_data(t_base *base);

#endif