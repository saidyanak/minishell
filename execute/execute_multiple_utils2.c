/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 13:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/05 13:23:17 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	assign_new_command(t_token **current, t_token **commands, int *cmd_index)
{
	t_token	*temp;

	temp = (*current)->next;
	(*current)->next = NULL;
	(*cmd_index)++;
	commands[*cmd_index] = temp;
	*current = temp;
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
	while (current && cmd_index < cmd_count)
	{
		if (current->type == TOKEN_PIPE)
			assign_new_command(&current, commands, &cmd_index);
		else
			current = current->next;
	}
	commands[cmd_count] = NULL;
	return (commands);
}
