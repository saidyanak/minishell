/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_filtering.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 00:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/17 00:00:00 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"

static t_token	*create_command_token(char *content, t_token_type type,
		t_quote_type q_type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->content = ft_strdup(content);
	new_token->type = type;
	new_token->q_type = q_type;
	new_token->next = NULL;
	if (!new_token->content)
		return (free(new_token), NULL);
	return (new_token);
}

static void	add_to_command_list(t_token **cmd_head, t_token **cmd_tail,
		t_token *new_token)
{
	if (!*cmd_head)
	{
		*cmd_head = new_token;
		*cmd_tail = new_token;
	}
	else
	{
		(*cmd_tail)->next = new_token;
		*cmd_tail = new_token;
	}
}

static void	process_command_token(t_token *current, t_token **cmd_head,
		t_token **cmd_tail)
{
	t_token	*new_token;

	if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
	{
		new_token = create_command_token(current->content,
				current->type, current->q_type);
		if (new_token)
			add_to_command_list(cmd_head, cmd_tail, new_token);
	}
}

t_token	*extract_command_tokens(t_token *token)
{
	t_token	*current;
	t_token	*cmd_head;
	t_token	*cmd_tail;

	if (!token)
		return (NULL);
	current = token;
	cmd_head = NULL;
	cmd_tail = NULL;
	while (current && current->type != TOKEN_PIPE)
	{
		process_command_token(current, &cmd_head, &cmd_tail);
		if (is_redirection_token(current->type) && current->next)
			current = current->next;
		current = current->next;
	}
	return (cmd_head);
}