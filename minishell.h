/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:59 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/02 11:49:01 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <limits.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
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
	TOKEN_PIPE,         // "|"
	TOKEN_REDIRECT_IN,  // "<"
	TOKEN_REDIRECT_OUT, // ">"
	TOKEN_APPEND,       // ">>"
	TOKEN_HEREDOC,      // "<<"
	TOKEN_QUOTED_WORD
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

// Tokenizer functions
char				*parse_word_with_quotes(char *input, int *i,
						t_token_type *type);
void				tokenize_input(char *input, t_base *base);
void				add_token(t_token **head, char *str, t_token_type type);
int					is_special(char c);

// Environment functions
t_env				*create_new_node(char *env);
void				add_new_node(t_env **head, t_env *new_node);
t_env				*init_env(char **env);

// Utility functions
char				*ft_strdup(const char *s1);
char				*ft_strndup(const char *s, size_t n);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strchr(const char *s, int c);
char				*ft_substr(char const *s, unsigned int start, size_t len);
int					ft_isspace(char c);
char				*ft_strjoin_free(char *s1, char *s2);
char				*get_env_value(t_base base, char *key);

// Cleanup functions
void				free_tokens(t_token *tokens);
void				free_env(t_env *env);
void				cleanup_base(t_base *base);
void				ft_build_in(t_token *current_prompt, t_base *base);
void				execute_command(t_base *base);

// Debug functions
void				print_tokens(t_token *token);
void				debug_parse_quotes(char *input);

// Build-in
int					ft_echo(t_token *current_prompt, t_base base);
t_base				*ft_cd(t_token *current_prompt, t_base *base);
int					ft_pwd(t_base base);
t_base				*ft_export(t_token *token, t_base *base);
int					ft_env(t_base *base);

#endif
