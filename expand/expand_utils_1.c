/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:21:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/04 12:28:26 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_env_value(t_base *base, char *key)
{
	t_env	*current;

	if (!key)
		return (ft_strdup(""));
	if (ft_strcmp(key, "?") == 0)
		return (ft_itoa(base->exit_status));
	current = base->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (ft_strdup(""));
}

void	free_single_token(t_token *token)
{
	if (!token)
		return ;
	if (token->content)
		free(token->content);
	free(token);
}

static t_token	*remove_first_null_token(t_token **head)
{
	t_token	*temp;

	if (!head || !*head)
		return (NULL);
	if ((*head)->content == NULL || ft_strlen((*head)->content) == 0)
	{
		temp = *head;
		*head = (*head)->next;
		free_single_token(temp);
		return (*head);
	}
	return (*head);
}

static void	remove_middle_null_tokens(t_token *current)
{
	t_token	*temp;

	while (current && current->next)
	{
		if (current->next->content == NULL
			|| ft_strlen(current->next->content) == 0)
		{
			temp = current->next;
			current->next = temp->next;
			free_single_token(temp);
		}
		else
			current = current->next;
	}
}

void	delete_null_tokens(t_base *base)
{
	t_token	*current;

	if (!base || !base->token)
		return ;
	while (base->token && (base->token->content == NULL
			|| ft_strlen(base->token->content) == 0))
	{
		base->token = remove_first_null_token(&base->token);
	}
	if (base->token)
	{
		current = base->token;
		remove_middle_null_tokens(current);
	}
}
