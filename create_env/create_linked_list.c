/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_linked_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:46:17 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/14 02:22:25 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	allocate_key_value(t_env *node, char *env, char *equal_sign)
{
	int	key_len;

	if (!equal_sign)
	{
		node->key = ft_strdup(env);
		node->value = initialize_empty_content_safe();
	}
	else
	{
		key_len = equal_sign - env;
		node->key = malloc(key_len + 1);
		if (node->key)
		{
			ft_strlcpy(node->key, env, key_len + 1);
			node->key[key_len] = '\0';
		}
		node->value = ft_strdup(equal_sign + 1);
	}
	if (!(node->key && node->value))
		return (0);
	return (1);
}

static void	cleanup_node(t_env *node)
{
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
}

t_env	*create_new_node(char *env)
{
	t_env	*new_node;
	char	*equal_sign;

	if (!env)
		return (NULL);
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	equal_sign = ft_strchr(env, '=');
	if (!allocate_key_value(new_node, env, equal_sign))
	{
		cleanup_node(new_node);
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

t_env	*create_minimal_env(void)
{
	t_env	*env_list;
	t_env	*node;
	char	*pwd_path;
	char	*pwd_env;

	env_list = NULL;
	
	// PWD değişkenini ekle
	pwd_path = getcwd(NULL, 0);
	if (pwd_path)
	{
		pwd_env = ft_strjoin("PWD=", pwd_path);
		if (pwd_env)
		{
			node = create_new_node(pwd_env);
			if (node)
				add_new_node(&env_list, node);
			free(pwd_env);
		}
		free(pwd_path);
	}
	
	// SHLVL=1 ekle
	node = create_new_node("SHLVL=1");
	if (node)
		add_new_node(&env_list, node);
	
	// PATH değişkenini minimal olarak ekle
	node = create_new_node("PATH=/usr/local/bin:/usr/bin:/bin");
	if (node)
		add_new_node(&env_list, node);
	
	// _ değişkenini ekle
	node = create_new_node("_=/usr/bin/env");
	if (node)
		add_new_node(&env_list, node);
	
	return (env_list);
}

t_env	*init_env(char **env)
{
	int		i;
	t_env	*env_list;
	t_env	*node;

	if (!env || !env[0])
		return (create_minimal_env());
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
			ft_putstr_fd("minishell: error initializing environment\n", 2);
			exit(1);
		}
		i++;
	}
	return (env_list);
}
