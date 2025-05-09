/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr >  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:12:03 by yuocak            #+#    #+#             */
/*   Updated: 2025/05/09 12:25:01 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft/libft.h"
# include <stdlib.h>
# include <stdio.h>

// 🟩 Environment Linked List Yapısı
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

// 🔧 Environment Fonksiyonları
t_env				*init_env(char **envp);
t_env				*create_env_node(char *env_str);
void				add_new_node(t_env **head, t_env *new_node);
char **tokenize_input(char *input);

// 🔧 String Yardımcı Fonksiyonlar
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s1);
char				*ft_strndup(const char *s1, size_t n);





#endif
