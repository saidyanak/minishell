/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_linked_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:23:06 by yuocak            #+#    #+#             */
/*   Updated: 2025/05/09 12:28:14 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

// Yeni env node oluşturur
t_env	*create_env_node(char *env_str)
{
	t_env	*new_node;
	char	*equal_sign;
	int		key_len;

	new_node = malloc(sizeof(t_env));






	if (!new_node)
		return (NULL);
	equal_sign = ft_strchr(env_str, '=');
	if (!equal_sign)
		return (NULL);
	key_len = equal_sign - env_str;
	new_node->key = ft_strndup(env_str, key_len);
	new_node->value = ft_strdup(equal_sign + 1);
	new_node->next = NULL;
	return (new_node);
}

// Linked list’e sona yeni node ekler
void	add_new_node(t_env **head, t_env *new_node)
{
	t_env	*temp;

	if (!*head)
		*head = new_node;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}

// init_env: tüm envp’yi linked list’e aktarır
t_env	*init_env(char **envp)
{
	t_env	*env_list;
	int		i;
	t_env	*node;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		node = create_env_node(envp[i]);
		if (node)
			add_new_node(&env_list, node);
		i++;
	}
	return (env_list);
}

