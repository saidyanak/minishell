/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_linked_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:46:17 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/02 14:41:27 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static int	allocate_key_value(t_env *node, char *env, char *equal_sign)
{
	int	key_len;

	if (!equal_sign)
	{
		node->key = ft_strdup(env);
		node->value = ft_strdup("");
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
	env_null_check(env_list);//tekrar dönülecek çünkü env'nin null gelme drumu bashte farklı çalışıyor
	return (env_list);
}

void	update_shlvl(t_base *base)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_value;

	shlvl_str = get_env_value(*base, "SHLVL");
	shlvl = ft_atoi(shlvl_str);
	shlvl++;
	new_value = ft_itoa(shlvl);
	set_env_value(&(base->env), "SHLVL", new_value);
	free(new_value); 
}

