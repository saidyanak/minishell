/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:59 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/29 11:34:30 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

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

typedef struct s_base
{
	t_token			*token;
	t_env			*env;
	int				exit_status;
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

/* Tokenizer functions */
char				*parse_word_with_quotes(char *input, int *i,
						t_token_type *type, t_quote_type *q_type);
void				tokenize_input(char *input, t_base *base);
void				add_token(t_token **head, char *str, t_token_type type,
						t_quote_type q_type);
int					is_special(char c);

/* Expand functions */
void				expand_tokens(t_base *base);
void				word_splitting(t_base *base);
void				handle_export_assignments(t_base *base);
void				process_export_and_expand(t_base *base);
char				*process_mixed_quotes(char *str, t_base *base);
int					has_dollar_sign(char *str);
int					has_tilde_sign(char *str);

/* Environment functions */
t_env				*create_new_node(char *env);
void				add_new_node(t_env **head, t_env *new_node);
t_env				*init_env(char **env);
char				*get_env_value(t_base base, char *key);

/* Utility functions */
int					ft_isspace(char c);

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
int					has_output_redirection(t_token *cmd);
int					has_input_redirection(t_token *cmd);

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
void	free_argv(char **argv);

/* Built-in functions */
void				ft_build_in(t_token *current_prompt, t_base *base);
int					ft_echo(t_token *current_prompt, t_base base);
t_base				*ft_cd(t_token *current_prompt, t_base *base);
int					ft_pwd(t_base base);
t_base				*ft_export(t_token *token, t_base *base);
int					ft_env(t_base *base);
t_base				*ft_unset(t_token *token, t_base *base);
void				ft_exit(t_token *current_prompt, t_base *base);

/* Environment manipulation */
void				set_env_value(t_env **env, char *key, char *value);

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
void				add_or_update_env(t_token *token, t_base *base);
int					ascii_strcmp(char *s1, char *s2);
void				sort_env_array(t_env **array, int count);
int					has_equal_sign_in_original(t_env *env);
void				print_export_var(t_env *env);
void				print_sorted_export(t_env *env);
void				sort_and_print_env(t_env *env);
int					process_export_args(t_token *token, t_base *base);
void				print_export(t_base *base);

/* Debug functions */
void				print_tokens(t_token *token);
void				debug_parse_quotes(char *input);
void				debug_token_with_quotes(t_token *token);

/* Syntax checking functions */
int					check_syntax_errors(t_token *token);

/* Heredoc functions */
char				*handle_heredoc(char *delimiter);
int					setup_heredoc_input(char *delimiter);
int					setup_simple_heredoc(char *delimiter);
void				cleanup_heredoc_files(void);
void    			handle_redirections(t_token *cmd);

/* Cleanup functions */
void				free_tokens(t_token *tokens);
void				free_env_list(t_env *env);
void				free_string_array(char **array);
void				cleanup_all(t_base *base);
void				free_commands(t_token **commands);

#endif