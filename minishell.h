/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:59 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/19 15:01:49 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stddef.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,			// "|"	
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
}					t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_base
{
	t_token			*token;
	t_env			*env;
}					t_base;

char    *parse_word_with_quotes(char *input, int *i);
void	tokenize_input(char *input, t_base *base);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *s, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_isspace(char c);
char	*ft_strjoin_free(char *s1, char *s2);
t_env *init_env(char **env);




void	print_tokens(t_token *token);
void	debug_parse_quotes(char *input);
#endif