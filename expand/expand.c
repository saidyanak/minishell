/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:16:15 by syanak            #+#    #+#             */
/*   Updated: 2025/08/06 10:33:32 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	expand_dollar_var(char *str, int i, char **result, t_base *base)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	var_name = extract_var_name(str + i + 1, &var_len);
	if (var_name && var_len > 0)
	{
		var_value = find_env_value(base, var_name);
		if (var_value)
			*result = join_and_free(*result, var_value);
		free(var_name);
		return (i + var_len + 1);
	}
	if (var_name)
		free(var_name);
	*result = join_and_free(*result, ft_strdup("$"));
	return (i + 1);
}

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
