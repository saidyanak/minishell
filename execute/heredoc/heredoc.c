/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 11:47:46 by syanak            #+#    #+#             */
/*   Updated: 2025/08/04 15:12:40 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <signal.h>

static int	should_expand_heredoc(char *delimiter)
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

static char	*remove_quotes_from_delimiter(char *delimiter)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = malloc(ft_strlen(delimiter) + 1);
	if (!result)
		return (NULL);
	while (delimiter[i])
	{
		if (delimiter[i] != '\'' && delimiter[i] != '"')
			result[j++] = delimiter[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

static char	*expand_heredoc_line(char *line, t_base *base, int expand)
{
	if (!line || !expand || !has_dollar_sign(line))
		return (ft_strdup(line));
	return (expand_variables(line, base));
}

static char	*join_heredoc_content(char *content, char *line)
{
	char	*temp;
	char	*result;

	if (!line)
		return (content);
	if (!content)
		return (ft_strdup(line));
	temp = ft_strjoin(content, "\n");
	if (!temp)
	{
		free(content);
		return (NULL);
	}
	result = ft_strjoin(temp, line);
	free(temp);
	free(content);
	return (result);
}

static char	*handle_eof_case(char *delimiter, char *content)
{
	char	*final_content;

	printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
		delimiter);
	if (content)
	{
		final_content = ft_strjoin(content, "\n");
		free(content);
		if (final_content)
			return (final_content);
		else
			return (ft_strdup("\n"));
	}
	return (ft_strdup("\n"));
}

static int	is_delimiter_match(char *line, char *delimiter)
{
	return (ft_strcmp(line, delimiter) == 0);
}

static char	*process_heredoc_line(char *line, char *content, t_base *base,
		int expand)
{
	char	*expanded_line;

	expanded_line = expand_heredoc_line(line, base, expand);
	if (!expanded_line)
	{
		if (content)
			free(content);
		return (NULL);
	}
	content = join_heredoc_content(content, expanded_line);
	free(expanded_line);
	return (content);
}

static char	*return_content(char *content)
{
	char	*final_content;

	final_content = NULL;
	if (content)
	{
		final_content = ft_strjoin(content, "\n");
		free(content);
		if (final_content != NULL)
			return (final_content);
		else
			return (ft_strdup("\n"));
	}
	return (NULL);
}

static char	*read_heredoc_input(char *delimiter, t_base *base, int expand)
{
	char	*line;
	char	*content;

	content = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (handle_eof_case(delimiter, content));
		if (is_delimiter_match(line, delimiter))
		{
			free(line);
			break ;
		}
		content = process_heredoc_line(line, content, base, expand);
		free(line);
		if (!content)
			return (NULL);
	}
	return (return_content(content));
}

static void	setup_heredoc_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

static void	restore_original_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

char	*handle_heredoc(char *delimiter, t_base *base)
{
	char	*content;
	char	*clean_delimiter;
	int		expand;

	if (!delimiter || !base)
		return (NULL);
	expand = should_expand_heredoc(delimiter);
	clean_delimiter = remove_quotes_from_delimiter(delimiter);
	if (!clean_delimiter)
		return (NULL);
	setup_heredoc_signals();
	content = read_heredoc_input(clean_delimiter, base, expand);
	restore_original_signals();
	free(clean_delimiter);
	return (content);
}
