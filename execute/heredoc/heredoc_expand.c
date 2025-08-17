/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 14:30:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 17:44:14 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../minishell.h"
#include <unistd.h>

char	*expand_heredoc_var(char *line, int *i, t_base *base)
{
	char	*var_name;
	char	*var_value;
	int		start;
	int		len;

	start = ++(*i);
	if (line[*i] == '?')
		return ((*i)++, ft_itoa(base->exit_status));
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (ft_strdup("$"));
	var_name = ft_substr(line, start, len);
	var_value = get_value(base->env, var_name);
	free(var_name);
	if (var_value)
		return (ft_strdup(var_value));
	else
		return (ft_strdup(""));
}

char	*expand_heredoc_line(char *line, t_base *base, int should_expand)
{
	char	*result;
	char	*temp;
	int		i;

	if (!should_expand || !line)
		return (check_return(line));
	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			temp = expand_heredoc_var(line, &i, base);
			result = ft_strjoin_free(result, temp);
		}
		else
		{
			temp = ft_substr(line, i, 1);
			result = ft_strjoin_free(result, temp);
			i++;
		}
	}
	return (result);
}

int	should_expand_delimiter(char *delimiter)
{
	int	i;

	i = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '\'' || delimiter[i] == '"')
			return (0);
		i++;
	}
	return (1);
}
#include "minishell.h"
#include "minishell.h"

int	join_quoted_delimiter(char *delimiter, int i, char **result)
{
	int		start;
	char	quote_char;
	char	*content;

	quote_char = delimiter[i++];
	start = i;
	while (delimiter[i] && delimiter[i] != quote_char)
		i++;
	if (delimiter[i] != quote_char)
		return (-1);
	content = ft_substr(delimiter, start, i - start);
	*result = join_and_free(*result, content);
	return (i + 1);
}

char	*remove_quotes_from_delimiter(char *delimiter)
{
	char	*result;
	char	temp[2];
	int		i;
	int		next_i;

	if (!delimiter)
		return (NULL);
	result = initialize_empty_content_safe();
	if (!result)
		return (NULL);
	i = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '\'' || delimiter[i] == '"')
		{
			next_i = join_quoted_delimiter(delimiter, i, &result);
			if (next_i == -1)
				return (free(result), NULL);
			i = next_i;
		}
		else
		{
			temp[0] = delimiter[i];
			temp[1] = '\0';
			result = join_and_free(result, ft_strdup(temp));
			i++;
		}
	}
	return (result);
}

char	*create_temp_filename(void)
{
	char	*pid_str;
	char	*counter_str;
	char	*temp;
	char	*temp_file;

	temp_file = NULL;
	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (NULL);
	counter_str = ft_itoa(get_unique_heredoc_id());
	if (!counter_str)
	{
		free(pid_str);
		return (NULL);
	}
	temp = ft_strjoin(pid_str, "_");
	free(pid_str);
	if (!temp)
	{
		free(counter_str);
		return (NULL);
	}
	temp_file = unique_name_return(temp, counter_str, temp_file);
	return (temp_file);
}
