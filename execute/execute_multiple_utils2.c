/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 13:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/12 10:15:02 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*find_previous_token(t_token *start, t_token *target)
{
	t_token	*current;

	if (!start || start == target)
		return (NULL);
	current = start;
	while (current && current->next != target)
		current = current->next;
	return (current);
}

static void	assign_new_command(t_token **current, t_token **commands,
		int *cmd_index)
{
	t_token	*pipe_token;
	t_token	*next_command;
	t_token	*prev_token;

	pipe_token = *current;
	next_command = pipe_token->next;
	// Önceki token'ı bul ve bağlantıyı kes
	prev_token = find_previous_token(commands[*cmd_index], pipe_token);
	if (prev_token)
		prev_token->next = NULL; // Bağlantıyı kes!
	// Pipe token'ını free et
	if (pipe_token->content)
		free(pipe_token->content);
	free(pipe_token);
	// Yeni komut başlat
	(*cmd_index)++;
	commands[*cmd_index] = next_command;
	*current = next_command;
}

t_token	**split_commands(t_token *token, int cmd_count)
{
	t_token	**commands;
	t_token	*current;
	int		cmd_index;

	commands = malloc(sizeof(t_token *) * (cmd_count + 1));
	if (!commands)
		return (NULL);
	current = token;
	cmd_index = 0;
	commands[cmd_index] = current;
	while (current && cmd_index < cmd_count - 1)
	{
		if (current->type == TOKEN_PIPE)
		{
			assign_new_command(&current, commands, &cmd_index);
		}
		else
		{
			current = current->next;
		}
	}
	commands[cmd_count] = NULL;
	return (commands);
}
