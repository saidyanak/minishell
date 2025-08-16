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

char	*remove_quotes_from_delimiter(char *delimiter)
{
	char	*result;
	char	quote_char;
	int		i;
	int		j;

	if (!delimiter || (delimiter[0] != '\'' && delimiter[0] != '"'))
		return (ft_strdup(delimiter));
	quote_char = delimiter[0];
	result = malloc(ft_strlen(delimiter) - 1);
	if (!result)
		return (NULL);
	i = 1;
	j = 0;
	while (delimiter[i] && delimiter[i] != quote_char)
		result[j++] = delimiter[i++];
	result[j] = '\0';
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
