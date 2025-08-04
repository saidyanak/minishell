/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:09:42 by syanak            #+#    #+#             */
/*   Updated: 2025/08/04 15:10:23 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_env_node(t_env *node)
{
	if (!node)
		return ;
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
}

int	remove_first_node(t_env **env, char *key)
{
	t_env	*temp;

	if (!*env || ft_strcmp((*env)->key, key) != 0)
		return (0);
	temp = *env;
	*env = (*env)->next;
	free_env_node(temp);
	return (1);
}

int	remove_middle_node(t_env *current, char *key)
{
	t_env	*temp;

	while (current->next)
	{
		if (ft_strcmp(current->next->key, key) == 0)
		{
			temp = current->next;
			current->next = temp->next;
			free_env_node(temp);
			return (1);
		}
		current = current->next;
	}
	return (0);
}

int	unset_env_var(t_env **env, char *key)
{
	if (!env || !*env || !key)
		return (0);
	if (remove_first_node(env, key))
		return (1);
	return (remove_middle_node(*env, key));
}
