/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:59 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/08 18:14:39 by yuocak           ###   ########.fr       */
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
}		t_quote_type;

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
}		t_token_type;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
	t_quote_type	q_type;
}		t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}		t_env;

typedef struct s_base
{
	t_token	*token;
	t_env	*env;
	int		exit_status;
}		t_base;

typedef struct s_exec_params
{
	char	**argv;
	char	**envp;
	char	*command_path;
}	t_exec_params;

typedef struct s_gc_node
{

	s_gc_scope			type;
	struct s_gc_node	*next;
};


typedef enum s_gc_scope
{
	GC_CMD,
	GC_GLOBAL
}	t_gc_scope;

/* Tokenizer functions */
char	*parse_word_with_quotes(char *input, int *i, t_token_type *type);
void	tokenize_input(char *input, t_base *base);
void	add_token(t_token **head, char *str, t_token_type type);
int		is_special(char c);

/* Expand functions */
void	expand_tokens(t_base *base);

/* Environment functions */
t_env	*create_new_node(char *env);
void	add_new_node(t_env **head, t_env *new_node);
t_env	*init_env(char **env);
char	*get_env_value(t_base base, char *key);

/* Utility functions */
char	*ft_strdup(const char *s1);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *s, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_isspace(char c);
char	*ft_strjoin_free(char *s1, char *s2);

/* Cleanup functions */
void	free_tokens(t_token *tokens);
void	free_env(t_env *env);
void	cleanup_base(t_base *base);

/* Execution functions */
void	ft_build_in(t_token *current_prompt, t_base *base);
void	execute_command(t_base *base);
int		execute_external_command(t_token *token, t_base *base);
int		single_execute_command(t_token *token, t_base *base);
void	multiple_execute_command(t_token *token, t_base *base);
int		is_single_execute(t_token *token);
int		is_redirection_token(t_token_type type);
void	free_string_array(char **array);
char	*find_command_path(char *command, t_base *base);
char	*build_full_path(char *dir, char *command);
char	**build_argv_from_tokens(t_token *token);
char	**env_to_envp(t_env *env);
int		check_build_in(char *input);
int		prepare_execution(t_token *token, t_base *base, t_exec_params *params);

/* Debug functions */
void	print_tokens(t_token *token);
void	debug_parse_quotes(char *input);

/* Built-in commands */
int		ft_echo(t_token *current_prompt, t_base base);
t_base	*ft_cd(t_token *current_prompt, t_base *base);
int		ft_pwd(t_base base);
t_base	*ft_export(t_token *token, t_base *base);
int		ft_env(t_base *base);
t_base	*ft_unset(t_token *token, t_base *base);
void	ft_exit(t_token *current_prompt, t_base *base);

#endif
