/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_linked_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:46:17 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/24 14:17:13 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_env	*create_new_node(char *env)
{
	t_env	*new_node;
	char	*equal_sign;
	int		key_len;

	if (!env)
		return (NULL);
	
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	
	equal_sign = ft_strchr(env, '=');
	if (!equal_sign)
	{
		// = işareti yoksa, değer boş string olarak ayarla
		new_node->key = ft_strdup(env);
		new_node->value = ft_strdup("");
	}
	else
	{
		key_len = equal_sign - env;
		new_node->key = ft_strndup(env, key_len);
		new_node->value = ft_strdup(equal_sign + 1);
	}
	
	// Bellek tahsisi kontrolleri
	if (!new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	
	new_node->next = NULL;
	return (new_node);
}

void	add_new_node(t_env **head, t_env *new_node)
{
	t_env	*tmp;
	
	if (!head || !new_node)
		return;
	
	if (!*head)
		*head = new_node;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

t_env	*init_env(char **env)
{
	int		i;
	t_env	*env_list;
	t_env	*node;

	if (!env)
		return (NULL);
	
	env_list = NULL;
	i = 0;
	
	while (env[i])
	{
		node = create_new_node(env[i]);
		if (node)
			add_new_node(&env_list, node);
		else
		{
			// Bellek hatası durumunda tüm listeyi temizle
			free_env(env_list);
			return (NULL);
		}
		i++;
	}
	return (env_list);
}
