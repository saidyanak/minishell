/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:30:00 by syanak            #+#    #+#             */
/*   Updated: 2025/08/13 20:05:53 by syanak           ###   ########.fr       */
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

	if (!base || !base->heredocs)
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
				if (base->exit_status != 130)
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
		rl_done = 1;
		rl_replace_line("", 0);
		rl_clear_history();
		rl_cleanup_after_signal();
		rl_free_line_state();
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
		
		// Ctrl+C kontrolü
		if (*heredoc_static_flag(0) == 1)
		{
			if (line)
				free(line);
			free(clean_delimiter);
			close(pipefd[1]);
			cleanup_child_resources(base);
			exit(130);
		}
		
		// EOF (Ctrl+D) kontrolü
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document at line ", 2);
			ft_putnbr_fd(line_count, 2);
			ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(clean_delimiter, 2);
			ft_putstr_fd("')\n", 2);
			break;
		}
		
		// Delimiter kontrolü
		if (ft_strcmp(line, clean_delimiter) == 0)
		{
			free(line);
			break;
		}
		
		// Satırı genişlet ve yaz
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
	content = initialize_empty_content_safe();
	if (!content)
	{
		close(pipefd[0]);
		waitpid(pid, &status, 0);
		return (NULL);
	}
	
	// Child'dan veri oku
	while ((bytes_read = read(pipefd[0], buffer, 1023)) > 0)
	{
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(content, buffer);
		if (!temp)
		{
			free(content);
			close(pipefd[0]);
			waitpid(pid, &status, 0);
			return (NULL);
		}
		free(content);
		content = temp;
	}
	close(pipefd[0]);
	
	// Child process'i bekle
	waitpid(pid, &status, 0);
	
	// Ctrl+C durumu
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		base->exit_status = 130;
		free(content);
		return (NULL);
	}
	
	// Normal durum - content'i döndür (boş da olsa)
	return (content);
}

char	*process_heredoc(char *delimiter, t_base *base)
{
	int		pipefd[2];
	pid_t	pid;
	char	*content;

	heredoc_static_flag(-1);
	if (pipe(pipefd) == -1)
		return (NULL);
	
	pid = fork();
	if (pid == 0)
	{
		// Child process
		heredoc_child_process(pipefd, delimiter, base);
	}
	else if (pid > 0)
	{
		// Parent process
		content = read_heredoc_content(pipefd, pid, base);
		
		// Ctrl+C durumunda cleanup
		if (base->exit_status == 130)
		{
			rl_cleanup_after_signal();
			rl_reset_after_signal();
			rl_initialize();
			return (NULL);
		}
		return (content);
	}
	else
	{
		// Fork hatası
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

static t_heredoc_info	*create_heredoc_info(char *temp_file, char *delimiter)
{
	t_heredoc_info	*new_heredoc;

	new_heredoc = malloc(sizeof(t_heredoc_info));
	if (!new_heredoc)
		return (NULL);
	
	new_heredoc->temp_filename = ft_strdup(temp_file);
	new_heredoc->original_delimiter = ft_strdup(delimiter);
	new_heredoc->heredoc_id = get_unique_heredoc_id();
	new_heredoc->next = NULL;
	
	if (!new_heredoc->temp_filename || !new_heredoc->original_delimiter)
	{
		if (new_heredoc->temp_filename)
			free(new_heredoc->temp_filename);
		if (new_heredoc->original_delimiter)
			free(new_heredoc->original_delimiter);
		free(new_heredoc);
		return (NULL);
	}
	
	return (new_heredoc);
}

static char	*create_temp_filename(void)
{
	char	*pid_str;
	char	*counter_str;
	char	*temp;
	char	*unique_name;
	char	*temp_file;

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
	
	unique_name = ft_strjoin(temp, counter_str);
	free(temp);
	free(counter_str);
	if (!unique_name)
		return (NULL);
	
	temp_file = ft_strjoin("/tmp/heredoc_", unique_name);
	free(unique_name);
	
	return (temp_file);
}

int	handle_heredoc_token(t_token *token, t_base *base)
{
	char			*content;
	char			*temp_file;
	int				fd;
	t_heredoc_info	*new_heredoc;

	if (!token || !token->next || !token->next->content)
		return (0);
	
	// Heredoc içeriğini al
	content = process_heredoc(token->next->content, base);
	
	// Ctrl+C durumu
	if (!content && base->exit_status == 130)
		return (0);
	
	// EOF durumunda boş string
	if (!content)
		content = ft_strdup("");
	
	// Temp dosya adı oluştur
	temp_file = create_temp_filename();
	if (!temp_file)
	{
		free(content);
		return (0);
	}
	
	// Dosyayı oluştur ve yaz
	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(content);
		free(temp_file);
		return (0);
	}
	
	write(fd, content, ft_strlen(content));
	close(fd);
	free(content);  // İçeriği artık kullanmayacağız, free et
	
	// Heredoc bilgisini sakla
	new_heredoc = create_heredoc_info(temp_file, token->next->content);
	if (new_heredoc)
	{
		new_heredoc->next = base->heredocs;
		base->heredocs = new_heredoc;
		base->heredoc_count++;
	}
	
	// Token content'ini güncelle
	free(token->next->content);
	token->next->content = temp_file;
	
	return (1);
}