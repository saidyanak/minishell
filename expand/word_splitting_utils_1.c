/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting_utils_1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:25:03 by syanak            #+#    #+#             */
/*   Updated: 2025/08/16 15:59:15 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"

char	*extract_word(char *str, int *i)
{
	int		start;
	char	*word;

	skip_ifs_chars(str, i);
	if (!str[*i])
		return (NULL);
	start = *i;
	while (str[*i] && !is_ifs_char(str[*i]))
		(*i)++;
	if (*i == start)
		return (NULL);
	word = ft_substr(str, start, *i - start);
	return (word);
}

t_token	*split_string_to_tokens(char *str)
{
	t_token	*head;
	char	*word;
	int		i;

	if (!str)
		return (NULL);
	head = NULL;
	i = 0;
	while (str[i])
	{
		word = extract_word(str, &i);
		if (!word)
			break ;
		add_token(&head, word, 0, 0);
	}
	return (head);
}

int	should_split_token(t_token *token)
{
	int	i;

	if (!token || !token->content)
		return (0);
	if (token->q_type == SINGLE_QUOTE || token->q_type == DOUBLE_QUOTE)
		return (0);
	i = 0;
	while (token->content[i])
	{
		if (is_ifs_char(token->content[i]))
			return (1);
		i++;
	}
	return (0);
}

t_token	*get_last_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens);
}

void	free_token_content(t_token *token)
{
	if (!token)
		return ;
	if (token->content)
		free(token->content);
	free(token);
}
