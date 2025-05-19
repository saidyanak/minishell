/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr >  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:12:03 by yuocak            #+#    #+#             */
/*   Updated: 2025/05/19 12:34:35 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>

// 🟩 Environment Linked List Yapısı
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_cmd
{
	char			*cmd;
	char			*param;
	int				build_in;
	struct t_cmd	*next;
}					t_cmd;

typedef struct s_token
{
	struct s_token	*next;

}					t_token;

typedef struct s_base
{
	t_token			*token;
	t_env			*env;
}					t_base;

// 🔧 Environment Fonksiyonları
t_env				*init_env(char **envp);
t_env				*create_env_node(char *env_str);
void				add_new_node(t_env **head, t_env *new_node);
void				tokenize_input(char *input, t_base my_base);

// 🔧 String Yardımcı Fonksiyonlar
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s1);
char				*ft_strndup(const char *s1, size_t n);

// execute
void				execute_command(char **args, t_env *my_env);

#endif
