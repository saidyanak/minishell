/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:05:21 by syanak            #+#    #+#             */
/*   Updated: 2025/07/30 12:06:25 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

int	is_valid_unset_name(char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!((name[0] >= 'a' && name[0] <= 'z') || (name[0] >= 'A'
				&& name[0] <= 'Z') || name[0] == '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A'
					&& name[i] <= 'Z') || (name[i] >= '0' && name[i] <= '9')
				|| name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	handle_unset_error(char *name)
{
	printf("minishell: unset: `%s': not a valid identifier\n", name);
	return (1);
}

void	free_env_node(t_env *node)
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

int	process_unset_arg(char *arg, t_base *base)
{
	if (ft_strchr(arg, '='))
		return (0);
	if (!is_valid_unset_name(arg))
		return (handle_unset_error(arg));
	unset_env_var(&(base->env), arg);
	return (0);
}

int	process_unset_args(t_token *token, t_base *base)
{
	t_token	*current;
	int		exit_status;

	current = token->next;
	exit_status = 0;
	while (current)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
		{
			if (process_unset_arg(current->content, base) != 0)
				exit_status = 1;
		}
		current = current->next;
	}
	return (exit_status);
}

t_base	*ft_unset(t_token *token, t_base *base)
{
	if (!token || !base)
		return (base);
	if (!token->next)
	{
		base->exit_status = 0;
		return (base);
	}
	base->exit_status = process_unset_args(token, base);
	return (base);
}
