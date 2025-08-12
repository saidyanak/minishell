/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/12 10:19:12 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	cleanup_heredoc_files(t_base *base)
{
	(void)base;
}

void	cleanup_heredocs(t_base *base)
{
	t_heredoc_info	*current;
	t_heredoc_info	*next;

	if (!base)
		return ;
	current = base->heredocs;
	while (current)
	{
		next = current->next;
		if (current->temp_filename)
		{
			unlink(current->temp_filename);
			free(current->temp_filename);
		}
		if (current->original_delimiter)
			free(current->original_delimiter);
		free(current);
		current = next;
	}
	base->heredocs = NULL;
	base->heredoc_count = 0;
}

void	process_all_heredocs(t_base *base)
{
	t_token	*current;

	if (!base || !base->token)
		return ;
	current = base->token;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			if (!handle_heredoc_token(current, base))
			{
				base->exit_status = 1;
				return ;
			}
		}
		current = current->next;
	}
}

void	restore_heredocs_in_redirections(t_token *cmd, t_base *base)
{
	t_token	*current;
	int		fd;

	(void)base;
	current = cmd;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC && current->next
			&& current->next->content)
		{
			fd = open(current->next->content, O_RDONLY);
			if (fd != -1)
			{
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
		}
		current = current->next;
	}
}

static void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		heredoc_static_flag(1);
		// Readline'ı tamamen kapat ve temizle
		rl_done = 1;
		rl_replace_line("", 0);
		rl_clear_history();
		rl_cleanup_after_signal();
		rl_free_line_state();
		// STDIN'i kapat ki readline bitsin
		close(STDIN_FILENO);
	}
}

static char	*expand_heredoc_var(char *line, int *i, t_base *base)
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
	return (var_value ? ft_strdup(var_value) : ft_strdup(""));
}

static char	*expand_heredoc_line(char *line, t_base *base, int should_expand)
{
	char	*result;
	char	*temp;
	int		i;

	if (!should_expand || !line)
		return (ft_strdup(line ? line : ""));
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

static int	should_expand_delimiter(char *delimiter)
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

static void	cleanup_child_resources(t_base *base)
{
	if (base->env)
		free_env_list(base->env);
	if (base->heredocs)
		cleanup_heredocs(base);
	if (base->token)
		free_tokens(base->token);
}

static void	heredoc_child_process(int pipefd[2], char *delimiter, t_base *base)
{
	char	*line;
	char	*expanded;
	char	*clean_delimiter;
	int		should_expand;
	int		line_count;

	close(pipefd[0]);
	signal(SIGINT, heredoc_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	should_expand = should_expand_delimiter(delimiter);
	clean_delimiter = remove_quotes_from_delimiter(delimiter);
	line_count = 1;
	while (1)
	{
		heredoc_static_flag(-1);
		line = readline("> ");
		// EOF (Ctrl+D) kontrolü
		if (*heredoc_static_flag(0) == 1)
		{
			free(line);
			free(clean_delimiter);
			close(pipefd[1]);
			cleanup_child_resources(base);
			exit(130);
		}
		if (!line)
		{
			printf("minishell: warning: here-document at line \
				%d delimited by end-of-file (wanted `%s')\n",
					line_count,
					clean_delimiter);
			break ;
		}
		// Ctrl+C kontrolü
		// Delimiter kontrolü
		if (ft_strcmp(line, clean_delimiter) == 0)
		{
			free(line);
			break ;
		}
		expanded = expand_heredoc_line(line, base, should_expand);
		write(pipefd[1], expanded, ft_strlen(expanded));
		write(pipefd[1], "\n", 1);
		free(line);
		free(expanded);
		line_count++;
	}
	free(clean_delimiter);
	close(pipefd[1]);
	cleanup_child_resources(base);
	exit(0);
}

static char	*read_heredoc_content(int pipefd[2], pid_t pid, t_base *base)
{
	char	*content;
	char	*temp;
	char	buffer[1024];
	int		bytes_read;
	int		status;

	close(pipefd[1]);
	content = ft_strdup("");
	// Non-blocking read ile child'dan veri oku
	while ((bytes_read = read(pipefd[0], buffer, 1023)) > 0)
	{
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(content, buffer);
		free(content);
		content = temp;
	}
	close(pipefd[0]);
	// Child process'i bekle ve temizle
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		base->exit_status = 130;
		free(content);
		return (NULL);
	}
	return (content);
}

char	*process_heredoc(char *delimiter, t_base *base)
{
	int		pipefd[2];
	pid_t	pid;
	char	*content;

	heredoc_static_flag(-1); // Reset flag
	if (pipe(pipefd) == -1)
		return (NULL);
	pid = fork();
	if (pid == 0)
	{
		// Child process - heredoc readline'ı burada
		heredoc_child_process(pipefd, delimiter, base);
	}
	else if (pid > 0)
	{
		// Parent process - child'ı bekle
		content = read_heredoc_content(pipefd, pid, base);
		// Child bittiğinde eğer Ctrl+C varsa cleanup yap
		if (base->exit_status == 130)
		{
			// Tüm pipes'ları kapat
			close(pipefd[0]);
			close(pipefd[1]);
			// Main readline'ı reset et
			rl_cleanup_after_signal();
			rl_reset_after_signal();
			rl_initialize();
			return (NULL);
		}
		return (content);
	}
	else
	{
		// Fork error
		close(pipefd[0]);
		close(pipefd[1]);
		return (NULL);
	}
	return (NULL);
}

static int	get_unique_heredoc_id(void)
{
	static int	counter = 0;

	return (++counter);
}

int	handle_heredoc_token(t_token *token, t_base *base)
{
	char *content;
	char *temp_file;
	char *pid_str;
	char *counter_str;
	char *unique_name;
	char *temp;
	int fd;

	if (!token || !token->next || !token->next->content)
		return (0);
	content = process_heredoc(token->next->content, base);
	if (!content)
		return (0);

	// DÜZELTME: Manuel strjoin kullan, ft_strjoin_free kullanma
	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (free(content), 0);
	counter_str = ft_itoa(get_unique_heredoc_id());
	if (!counter_str)
		return (free(content), free(pid_str), 0);

	// Manuel olarak birleştir
	temp = ft_strjoin(pid_str, "_");
	unique_name = ft_strjoin(temp, counter_str);
	free(temp);
	temp_file = ft_strjoin("/tmp/heredoc_", unique_name);

	// Memory temizliği
	free(pid_str);
	free(counter_str);
	free(unique_name);

	if (!temp_file)
		return (free(content), 0);

	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		return (free(content), free(temp_file), 0);
	write(fd, content, ft_strlen(content));
	close(fd);
	free(token->next->content);
	token->next->content = temp_file;
	return (free(content), 1);
}