/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_linked_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:46:17 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/24 13:25:38 by yuocak           ###   ########.fr       */
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
		new_node->key = malloc(ft_strlen(env) + 1);
		if (new_node->key)
			ft_strlcpy(new_node->key, env, ft_strlen(env) + 1);
		new_node->value = malloc(1);
		if (new_node->value)
			new_node->value[0] = '\0';
	}
	else
	{
		key_len = equal_sign - env;
		new_node->key = malloc(key_len + 1);
		if (new_node->key)
		{
			ft_strlcpy(new_node->key, env, key_len + 1);
			new_node->key[key_len] = '\0';
		}
		new_node->value = malloc(ft_strlen(equal_sign + 1) + 1);
		if (new_node->value)
			ft_strlcpy(new_node->value, equal_sign + 1, ft_strlen(equal_sign + 1) + 1);
	}
	if (!new_node->key || !new_node->value)
	{
		if (new_node->key)
			free(new_node->key);
		if (new_node->value)
			free(new_node->value);
		free(new_node);
		return (NULL);
	}
	new_node->exported = 1;
	new_node->next = NULL;
	return (new_node);
}

void	add_new_node(t_env **head, t_env *new_node)
{
	t_env	*tmp;

	if (!head || !new_node)
		return ;
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

void	env_null_check(t_env *env_lst)
{
	if (!env_lst)
	{
		printf("minishell: error initializing environment\n");
		exit(1);
	}
}

t_env	*init_env(char **env)
{
	int		i;
	t_env	*env_list;
	t_env	*node;

	if (!env) //kontrol edilmesi gerekiyor çünkü env null olabilmesi için ihtimal kalmıyor olursa zaten hata bastırılıyor
		exit(1);
	env_list = NULL;
	i = 0;
	while (env[i])
	{
		node = create_new_node(env[i]);
		if (node)
			add_new_node(&env_list, node);
		else
		{
			free_env_list(env_list);
			printf("minishell: error initializing environment\n");
			exit(1);
		}
		i++;
	}
	env_null_check(env_list);
	return (env_list);
}

