/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:18:32 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/05 13:19:12 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_n_flag(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] != '-')
		return (0);
	i = 1;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static int	process_n_flags(t_token **current)
{
	int	newline;

	newline = 1;
	while (*current && (*current)->content && (*current)->type == TOKEN_WORD
		&& is_valid_n_flag((*current)->content))
	{
		newline = 0;
		*current = (*current)->next;
	}
	return (newline);
}

static void	write_argument(t_token *token, int *first_arg)
{
	if (!(*first_arg))
		write(1, " ", 1);
	write(1, token->content, ft_strlen(token->content));
	*first_arg = 0;
}

static void	print_arguments(t_token *current)
{
	int	first_arg;

	first_arg = 1;
	while (current && current->content)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
		{
			write_argument(current, &first_arg);
			current = current->next;
		}
		else if (current->type == TOKEN_PIPE)
			break ;
		else if (current->type == TOKEN_REDIRECT_IN
			|| current->type == TOKEN_REDIRECT_OUT
			|| current->type == TOKEN_APPEND
			|| current->type == TOKEN_HEREDOC)
		{
			current = current->next;
			if (current)
				current = current->next;
		}
		else
			current = current->next;
	}
}

int	ft_echo(t_token *current_prompt, t_base base)
{
	t_token	*current;
	int		newline;

	(void)base;
	current = current_prompt->next;
	newline = process_n_flags(&current);
	print_arguments(current);
	if (newline)
		write(1, "\n", 1);
	return (0);
}
