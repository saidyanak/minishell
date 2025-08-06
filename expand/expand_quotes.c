/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:19:56 by syanak            #+#    #+#             */
/*   Updated: 2025/08/06 10:33:11 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (expanded)
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
	if (expanded)
		*result = join_and_free(*result, expanded);
	free(content);
	return (i);
}

char	*process_mixed_quotes(char *str, t_base *base)
{
	char	*result;
	int		i;

	if (!str)
		return (initialize_empty_content_safe());
	result = initialize_empty_content_safe();
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
