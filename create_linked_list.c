/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_linked_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:46:17 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/09 15:58:14 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static char	*create_perm_string(t_gc *gc, const char *str)
{
	char	*new_str;
	int		len;
	int		i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	new_str = ft_malloc_perm(gc, len + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

static char	*create_perm_string_n(t_gc *gc, const char *str, int n)
{
	char	*new_str;
	int		i;

	if (!str)
		return (NULL);
	new_str = ft_malloc_perm(gc, n + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < n && str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

t_env	*create_new_node(char *env, t_gc *gc)
{
	t_env	*new_node;
	char	*equal_sign;
	int		key_len;

	if (!env)
		return (NULL);
	new_node = ft_malloc_perm(gc, sizeof(t_env));
	if (!new_node)
		return (NULL);
	equal_sign = ft_strchr(env, '=');
	if (!equal_sign)
	{
		new_node->key = create_perm_string(gc, env);
		new_node->value = create_perm_string(gc, "");
	}
	else
	{
		key_len = equal_sign - env;
		new_node->key = create_perm_string_n(gc, env, key_len);
		new_node->value = create_perm_string(gc, equal_sign + 1);
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

	if (!env)
		exit(1);
	env_list = NULL;
	i = 0;
	while (env[i])
	{
		node = create_new_node(env[i], gc);
		if (node)
			add_new_node(&env_list, node);
		else
		{
			printf("minishell: error initializing environment\n");
			exit(1);
		}
		i++;
	}
	env_null_check(env_list);
	return (env_list);
}
