/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:16:15 by syanak            #+#    #+#             */
/*   Updated: 2025/08/16 16:40:11 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"

static int	copy_regular_text(char *str, int i, char **result)
{
	int		j;
	char	*text;

	j = i;
	while (str[j] && str[j] != '$')
		j++;
	if (j > i)
	{
		text = ft_substr(str, i, j - i);
		*result = join_and_free(*result, text);
	}
	return (j);
}

char	*initialize_empty_content_safe(void)
{
	char	*result;

	result = malloc(sizeof(char) * 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	return (result);
}

char	*expand_variables(char *str, t_base *base)
{
	char	*result;
	int		i;

	if (!str)
		return (initialize_empty_content_safe());
	result = initialize_empty_content_safe();
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			i = expand_dollar_var(str, i, &result, base);
		else
			i = copy_regular_text(str, i, &result);
	}
	return (result);
}

void	expand_token_content(t_token *token, t_base *base)
{
	char	*expanded;

	if (!token || !token->content)
		return ;
	if (is_only_empty_variable(token->content, base))
	{
		free(token->content);
		token->content = NULL;
		return ;
	}
	expanded = process_mixed_quotes(token->content, base);
	if (expanded)
	{
		free(token->content);
		token->content = expanded;
	}
}

void	expand_tokens(t_base *base)
{
	t_token	*current;
	char	*expanded;

	if (!base || !base->token)
		return ;
	current = base->token;
	while (current)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
		{
			if (has_tilde_sign(current->content)
				&& current->q_type == NONE_QUOTE)
			{
				free(current->content);
				expanded = find_env_value(base, "HOME");
				current->content = expanded;
			}
			else
				expand_token_content(current, base);
		}
		current = current->next;
	}
	word_splitting(base);
	delete_null_tokens(base);
}
