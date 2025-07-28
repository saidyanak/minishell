/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 14:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/28 12:19:05 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

// Syntax error mesajı yazdır
static void	print_syntax_error(char *token)
{
	printf("minishell: syntax error near unexpected token `%s'\n", token);
}

// Token'ın operator olup olmadığını kontrol et
static int	is_operator_token(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_REDIRECT_IN
		|| type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND
		|| type == TOKEN_HEREDOC);
}

// Pipe syntax'ını kontrol et
static int	check_pipe_syntax(t_token *token)
{
	t_token	*current;
	t_token	*prev;

	current = token;
	prev = NULL;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			// Başta pipe olamaz
			if (!prev)
			{
				print_syntax_error("|");
				return (0);
			}
			// Pipe'dan sonra herhangi bir token olmalı
			if (!current->next)
			{
				print_syntax_error("newline");
				return (0);
			}
			// Pipe'dan hemen sonra başka pipe olamaz
			if (current->next->type == TOKEN_PIPE)
			{
				print_syntax_error(current->next->content);
				return (0);
			}
			// Art arda pipe olamaz
			if (prev && prev->type == TOKEN_PIPE)
			{
				print_syntax_error("|");
				return (0);
			}
		}
		prev = current;
		current = current->next;
	}
	return (1);
}

// Redirection syntax'ını kontrol et
static int	check_redirection_syntax(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (is_redirection_token(current->type))
		{
			// Redirection'dan sonra dosya adı olmalı
			if (!current->next)
			{
				print_syntax_error("newline");
				return (0);
			}
			// Redirection'dan hemen sonra başka redirection olamaz
			if (is_redirection_token(current->next->type))
			{
				print_syntax_error(current->next->content);
				return (0);
			}
			// Dosya adı word olmalı
			if (current->next->type != TOKEN_WORD
				&& current->next->type != TOKEN_QUOTED_WORD)
			{
				print_syntax_error(current->next->content);
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

// Heredoc syntax'ını kontrol et
static int	check_heredoc_syntax(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			// Heredoc'dan sonra delimiter olmalı
			if (!current->next)
			{
				print_syntax_error("newline");
				return (0);
			}
			// Delimiter word olmalı
			if (current->next->type != TOKEN_WORD
				&& current->next->type != TOKEN_QUOTED_WORD)
			{
				print_syntax_error(current->next->content);
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

// Art arda gelen operator'ları kontrol et
static int	check_consecutive_operators(t_token *token)
{
	t_token	*current;
	t_token	*prev;

	current = token;
	prev = NULL;
	while (current)
	{
		if (is_operator_token(current->type) && prev
			&& is_operator_token(prev->type))
		{
			// Sadece art arda aynı tip operator'lar hata
			// Örnek: >> veya || gibi (ama > | > geçerli)
			if (current->type == prev->type)
			{
				print_syntax_error(current->content);
				return (0);
			}
		}
		prev = current;
		current = current->next;
	}
	return (1);
}

// Son token operator olamaz (pipe hariç zaten kontrol edildi)
static int	check_ending_operator(t_token *token)
{
	t_token	*current;
	t_token	*last;

	if (!token)
		return (1);
	current = token;
	last = NULL;
	while (current)
	{
		last = current;
		current = current->next;
	}
	if (last && is_redirection_token(last->type))
	{
		print_syntax_error("newline");
		return (0);
	}
	return (1);
}

// Ana syntax checker fonksiyonu
int	check_syntax_errors(t_token *token)
{
	if (!token)
		return (1);
	if (!check_pipe_syntax(token))
		return (0);
	if (!check_redirection_syntax(token))
		return (0);
	if (!check_heredoc_syntax(token))
		return (0);
	if (!check_consecutive_operators(token))
		return (0);
	if (!check_ending_operator(token))
		return (0);
	return (1);
}