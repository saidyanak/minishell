/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:16:15 by syanak            #+#    #+#             */
/*   Updated: 2025/07/11 19:01:37 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_tilde_sign(char *str)
{
	if (str[0] == '~' && str[1] == 0)
		return (1);
	return (0);
}

int	has_dollar_sign(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	is_valid_var_char(char c, int first)
{
	if (first)
		return (ft_isalpha(c) || c == '_' || c == '?');
	return (ft_isalnum(c) || c == '_');
}

char	*extract_braced_var(char *str, int *len)
{
	int	i;

	i = 1;
	while (str[i] && str[i] != '}')
		i++;
	if (str[i] == '}')
	{
		*len = i + 1;
		return (ft_substr(str, 1, i - 1));
	}
	*len = 0;
	return (NULL);
}

char	*extract_var_name(char *str, int *len, t_gc *gc)
{
	int	i;

	*len = 0;
	if (!str || !str[0])
		return (NULL);
	if (str[0] == '{')
		return (extract_braced_var(str, len));
	if (!is_valid_var_char(str[0], 1))
		return (NULL);
	i = 0;
	while (str[i] && is_valid_var_char(str[i], i == 0))
		i++;
	*len = i;
	return (ft_substr_gc(gc, str, 0, i));
}

char	*find_env_value(t_base *base, char *key)
{
	t_env	*current;

	if (!key)
		return (ft_strdup_gc(base->gc, ""));
	if (ft_strcmp(key, "?") == 0)
		return (ft_strdup_gc(base->gc, ft_itoa(base->exit_status)));
	current = base->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0 && current->exported)
			return (ft_strdup_gc(base->gc, current->value));
		current = current->next;
	}
	return (ft_strdup_gc(base->gc, ""));
}

int	handle_dollar_sign(char *content, char **result, int i, t_base *base)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		var_len;

	var_name = extract_var_name(content + i + 1, &var_len, base->gc);
	if (var_name && var_len > 0)
	{
		var_value = find_env_value(base, var_name);
		char *joined = ft_strjoin(*result, var_value);
		temp = ft_strdup_gc(base->gc, joined);
		free(joined);
		*result = temp;
		return (i + var_len + 1);
	}
	if (var_name)
		free(var_name);
	temp = ft_strjoin(*result, "$");
	free(*result);
	*result = temp;
	return (i + 1);
}

int	handle_regular_chars(char *content, char **result, int i)
{
	char	*temp;
	char	*substr;
	int		j;

	j = i;
	while (content[j] && content[j] != '$')
		j++;
	substr = ft_substr(content, i, j - i);
	temp = ft_strjoin(*result, substr);
	free(*result);
	free(substr);
	*result = temp;
	return (j);
}

char	*process_expansion(char *content, t_base *base)
{
	char	*result;
	int		i;

	result = ft_strdup_gc(base->gc, "");
	i = 0;
	while (content[i])
	{
		if (content[i] == '$')
			i = handle_dollar_sign(content, &result, i, base);
		else
			i = handle_regular_chars(content, &result, i);
	}
	return (result);
}

int	should_expand(t_token *token)
{
	if (!token || !token->content)
		return (0);
	return (has_dollar_sign(token->content));
}

void	expand_token_content(t_token *token, t_base *base)
{
	char	*expanded;

	if (!token || !token->content)
		return ;
	expanded = process_expansion(token->content, base);
	token->content = expanded;
}

void	expand_tokens(t_base *base)
{
	t_token *current;
	char *expanded;

	if (!base || !base->token)
		return ;
	current = base->token;
	while (current)
	{
		if (should_expand(current))
			expand_token_content(current, base);
		else if (has_tilde_sign(current->content)
			&& current->type == TOKEN_WORD)
		{
			expanded = ft_strdup_gc(base->gc, get_env_value(*base, "HOME"));
			current->content = expanded;
		}
		current = current->next;
	}
}
