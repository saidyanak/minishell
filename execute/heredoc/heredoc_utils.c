/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/12 03:43:11 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*expand_heredoc_line(char *line, t_base *base, int expand)
{
	if (!line || !expand || !has_dollar_sign(line))
		return (ft_strdup(line));
	return (expand_variables(line, base));
}

char	*join_heredoc_content(char *content, char *line)
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

char	*process_heredoc_line(char *line, char *content, t_base *base,
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

char	*finalize_heredoc_content(char *content)
{
	char	*final_content;

	if (!content)
		return (initialize_empty_content_safe());
	final_content = ft_strjoin(content, "\n");
	free(content);
	if (final_content)
		return (final_content);
	else
		return (ft_strdup("\n"));
}

int	process_readline_input(char **line, char *clean_delimiter, char **content,
		t_base *base)
{
	int	expand;
	int	*flag;

	expand = should_expand_heredoc(clean_delimiter);
	*line = readline("> ");
	flag = heredoc_static_flag(0);
	if (*flag == 1)
	{
		printf("geldim\n");
		base->exit_status = 130;
		free(*line);
		return (0);
	}
	if (!*line)
	{
		printf("minishell: warning: here-document deli\
			mited by end-of-file (wanted `%s')\n",
				clean_delimiter);
		return (0);
	}
	if (ft_strcmp(*line, clean_delimiter) == 0)
	{
		free(*line);
		return (0);
	}
	*content = process_heredoc_line(*line, *content, base, expand);
	free(*line);
	return (*content != NULL);
}
