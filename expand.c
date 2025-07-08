/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:16:15 by syanak            #+#    #+#             */
/*   Updated: 2025/07/08 18:01:16 by syanak           ###   ########.fr       */
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

char	*extract_var_name(char *str, int *len)
{
	int	i;

	*len = 0;
	if (!str || !str[0])
		return (NULL);
	if (!is_valid_var_char(str[0], 1))
		return (NULL);
	i = 0;
	while (str[i] && is_valid_var_char(str[i], i == 0))
		i++;
	*len = i;
	return (ft_substr(str, 0, i));
}

char	*find_env_value(t_base *base, char *key)
{
	t_env	*current;

	if (!key)
		return (ft_strdup(""));
	if (ft_strcmp(key, "?") == 0)
		return (ft_itoa(base->exit_status));
	current = base->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (ft_strdup(""));
}

static char	*join_and_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

static int	expand_dollar_var(char *str, int i, char **result, t_base *base)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	var_name = extract_var_name(str + i + 1, &var_len);
	printf("var_name: %s\n", var_name);
	if (var_name && var_len > 0)
	{
		var_value = find_env_value(base, var_name);
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
		return (ft_strdup(""));
	result = ft_strdup("");
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

static int	process_single_quote_section(char *str, int i, char **result)
{
	int		start;
	char	*content;

	i++;
	start = i;
	while (str[i] && str[i] != '\'')
		i++;
	content = ft_substr(str, start, i - start);
	*result = join_and_free(*result, content);
	if (str[i] == '\'')
		i++;
	return (i);
}

static int	process_double_quote_section(char *str, int i, char **result,
		t_base *base)
{
	int		start;
	char	*content;
	char	*expanded;

	i++;
	start = i;
	while (str[i] && str[i] != '"')
		i++;
	content = ft_substr(str, start, i - start);
	expanded = expand_variables(content, base);
	*result = join_and_free(*result, expanded);
	free(content);
	if (str[i] == '"')
		i++;
	return (i);
}

static int	process_unquoted_section(char *str, int i, char **result,
		t_base *base)
{
	int		start;
	char	*content;
	char	*expanded;

	start = i;
	while (str[i] && str[i] != '\'' && str[i] != '"')
		i++;
	content = ft_substr(str, start, i - start);
	expanded = expand_variables(content, base);
	*result = join_and_free(*result, expanded);
	free(content);
	return (i);
}

char	*process_mixed_quotes(char *str, t_base *base)
{
	char	*result;
	int		i;

	if (!str)
		return (ft_strdup(""));
	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			i = process_single_quote_section(str, i, &result);
		else if (str[i] == '"')
			i = process_double_quote_section(str, i, &result, base);
		else
			i = process_unquoted_section(str, i, &result, base);
	}
	return (result);
}

void	expand_token_content(t_token *token, t_base *base)
{
	char	*expanded;

	if (!token || !token->content)
		return ;
	expanded = process_mixed_quotes(token->content, base);
	free(token->content);
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
}