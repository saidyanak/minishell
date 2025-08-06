/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/06 10:32:56 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_signal_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, heredoc_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	should_expand_heredoc(char *delimiter)
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

	result = malloc(ft_strlen(delimiter) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
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

static void	print_eof_warning(char *delimiter)
{
	printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
		delimiter);
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

static char	*finalize_heredoc_content(char *content)
{
	char	*final_content;

	if (!content)
		return (initialize_empty_content_safe());
	final_content = ft_strjoin(content, "\n");
	free(content);
	return (final_content ? final_content : ft_strdup("\n"));
}

char	*read_heredoc_input_child(char *delimiter, t_base *base, int expand)
{
	char *line;
	char *content;
	char *clean_delimiter;

	clean_delimiter = remove_quotes_from_delimiter(delimiter);
	if (!clean_delimiter)
		return (NULL);
	content = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_eof_warning(clean_delimiter);
			break ;
		}
		if (ft_strcmp(line, clean_delimiter) == 0)
		{
			free(line);
			break ;
		}
		content = process_heredoc_line(line, content, base, expand);
		free(line);
		if (!content)
		{
			free(clean_delimiter);
			return (NULL);
		}
	}
	free(clean_delimiter);
	return (finalize_heredoc_content(content));
}