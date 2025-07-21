/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_linked_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:46:17 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/17 15:21:33 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_env	*create_new_node_gc(char *env, t_gc *gc)
{
	t_env	*new_node;
	char	*equal_sign;
	int		key_len;

	if (!env)
		return (NULL);
	new_node = ft_malloc_env(gc, sizeof(t_env));
	if (!new_node)
		return (NULL);
	equal_sign = ft_strchr(env, '=');
	if (!equal_sign)
	{
		new_node->key = ft_malloc_env(gc, ft_strlen(env) + 1);
		if (new_node->key)
			ft_strlcpy(new_node->key, env, ft_strlen(env) + 1);
		new_node->value = ft_malloc_env(gc, 1);
		if (new_node->value)
			new_node->value[0] = '\0';
	}
	else
	{
		key_len = equal_sign - env;
		new_node->key = ft_malloc_env(gc, key_len + 1);
		if (new_node->key)
		{
			ft_strlcpy(new_node->key, env, key_len + 1);
			new_node->key[key_len] = '\0';
		}
		new_node->value = ft_malloc_env(gc, ft_strlen(equal_sign + 1) + 1);
		if (new_node->value)
			ft_strlcpy(new_node->value, equal_sign + 1, ft_strlen(equal_sign + 1) + 1);
	}
	if (!new_node->key || !new_node->value)
		return (NULL);
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

t_env	*init_env(char **env, t_gc *gc)
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
		node = create_new_node_gc(env[i], gc);
		if (node)
			add_new_node(&env_list, node);
		else
		{
			ft_free_all_env(gc);
			printf("minishell: error initializing environment\n");
			exit(1);
		}
		i++;
	}
	env_null_check(env_list);
	return (env_list);
}

// Artık free_env fonksiyonu kullanılmıyor, GC ile yönetiliyor.
// clean_up.c dosyasına artık ihtiyaç yok, GC ile temizlik yapılacak.
// Program sonunda env'leri temizlemek için:
// ft_free_all_env(&gc);
