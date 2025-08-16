/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 15:59:21 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

void	cleanup_tokens_and_heredocs(t_base *base)
{
	if (base->token)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	cleanup_heredocs(base);
	heredoc_static_flag(-1);
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	if (tokens)
	{
		while (current)
		{
			next = current->next;
			if (current->content)
			{
				free(current->content);
				current->content = NULL;
			}
			free(current);
			current = NULL;
			current = next;
		}
	}
}

void	free_env_list(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_child_env_and_commands(t_exec_data *data)
{
	int	i;

	if (data->base && data->base->env && data->base->env->key
		&& data->base->env->value)
	{
		free_env_list(data->base->env);
		data->base->env = NULL;
	}
	if (data->commands)
	{
		i = 0;
		while (data->commands[i])
		{
			free_tokens(data->commands[i]);
			i++;
		}
		free_commands(data->commands);
		data->commands = NULL;
	}
}
