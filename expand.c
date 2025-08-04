/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 12:16:15 by syanak            #+#    #+#             */
/*   Updated: 2025/08/04 09:54:09 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_tilde_sign(char *str)
{
	if (!str)
		return (0);
	if (str[0] == '~' && (str[1] == 0 || str[1] == '/'))
		return (1);
	return (0);
}

int	has_dollar_sign(char *str)
{
	int	i;

	if (!str)
		return (0);
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
	if (str[0] == '?')
	{
		*len = 1;
		return (ft_strdup("?"));
	}
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

	if (!s1 && !s2)
		return (ft_strdup(""));
	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
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
	if (str[i] != '\'')
		return (-1);
	content = ft_substr(str, start, i - start);
	*result = join_and_free(*result, content);
	return (i + 1);
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
	if (str[i] != '"')
		return (-1);
	content = ft_substr(str, start, i - start);
	expanded = expand_variables(content, base);
	*result = join_and_free(*result, expanded);
	free(content);
	return (i + 1);
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
		{
			i = process_single_quote_section(str, i, &result);
			if (i == -1)
				return (NULL);
		}
		else if (str[i] == '"')
		{
			i = process_double_quote_section(str, i, &result, base);
			if (i == -1)
				return (NULL);
		}
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
	if (expanded)
	{
		free(token->content);
		token->content = expanded;
	}
}

void	free_single_token(t_token *token)
{
	if (!token)
		return ;
	if (token->content)
		free(token->content);
	free(token);
}

static t_token	*remove_first_null_token(t_token **head)
{
	t_token	*temp;

	if (!head || !*head)
		return (NULL);
	if ((*head)->content == NULL || ft_strlen((*head)->content) == 0)
	{
		temp = *head;
		*head = (*head)->next;
		free_single_token(temp);
		return (*head);
	}
	return (*head);
}

static void	remove_middle_null_tokens(t_token *current)
{
	t_token	*temp;

	while (current && current->next)
	{
		if (current->next->content == NULL
			|| ft_strlen(current->next->content) == 0)
		{
			temp = current->next;
			current->next = temp->next;
			free_single_token(temp);
		}
		else
			current = current->next;
	}
}

void	delete_null_tokens(t_base *base)
{
	t_token	*current;

	if (!base || !base->token)
		return ;
	while (base->token && (base->token->content == NULL
			|| ft_strlen(base->token->content) == 0))
	{
		base->token = remove_first_null_token(&base->token);
	}
	if (base->token)
	{
		current = base->token;
		remove_middle_null_tokens(current);
	}
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
	word_splitting(base);
	delete_null_tokens(base);
}
