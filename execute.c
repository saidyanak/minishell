/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/14 11:39:03 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Forward declarations
static void	free_argv(char **argv);

int	check_build_in(char *input)
{
	return (ft_strcmp(input, "cd") == 0 || ft_strcmp(input, "echo") == 0
		|| ft_strcmp(input, "env") == 0 || ft_strcmp(input, "pwd") == 0
		|| ft_strcmp(input, "unset") == 0 || ft_strcmp(input, "exit") == 0
		|| ft_strcmp(input, "export") == 0 || ft_strcmp(input, "list") == 0);
}

// argv array'ini temizle
static void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

char	*find_command_path(char *command, t_base *base)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	// Mutlak yol mu kontrol et
	if (access(command, F_OK) == 0)
		return (ft_strdup(command));
	// PATH environment'ı al
	path_env = get_env_value(*base, "PATH");
	if (!path_env)
		return (NULL);
	// PATH'i parçala
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	// Her PATH'te ara
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, command);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			// paths array'ini temizle
			free_argv(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	// paths array'ini temizle
	free_argv(paths);
	return (NULL);
}

// Token'ları argv array'ine çevir
static char	**tokens_to_argv(t_token *token)
{
	t_token	*current;
	char	**argv;
	int		count;
	int		i;

	// Token sayısını say
	count = 0;
	current = token;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
			count++;
		current = current->next;
	}
	// argv array'i oluştur
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	// Token'ları kopyala
	i = 0;
	current = token;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
		{
			argv[i] = ft_strdup(current->content);
			i++;
		}
		current = current->next;
	}
	argv[i] = NULL;
	return (argv);
}

// Environment'ı execve için char** formatına çevir
char	**env_to_envp(t_env *env)
{
	t_env	*current;
	char	**envp;
	char	*temp;
	int		count;
	int		i;

	// Environment variable sayısını say
	count = 0;
	current = env;
	while (current)
	{
		if (current->exported)
			count++;
		current = current->next;
	}
	// envp array'i oluştur
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	// Environment variables'ı kopyala
	i = 0;
	current = env;
	while (current)
	{
		if (current->exported)
		{
			temp = ft_strjoin(current->key, "=");
			envp[i] = ft_strjoin(temp, current->value);
			free(temp);
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}

// envp array'ini temizle
static void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

// External command execution (fork + execve)
int	execute_external_command(t_token *token, t_base *base)
{
	char	*command_path;
	char	**argv;
	char	**envp;
	pid_t	pid;
	int		status;

	// PATH'te komutu ara
	command_path = find_command_path(token->content, base);
	if (!command_path)
	{
		printf("minishell: %s: command not found\n", token->content);
		base->exit_status = 127;
		return (127);
	}
	// argv array'i oluştur
	argv = tokens_to_argv(token);
	if (!argv)
	{
		free(command_path);
		return (1);
	}
	// envp array'ini oluştur
	envp = env_to_envp(base->env);
	if (!envp)
	{
		free(command_path);
		free_argv(argv);
		return (1);
	}
	// Fork yap
	pid = fork();
	if (pid == 0)
	{
		// Child process
		execve(command_path, argv, envp); // Burada execve çağrısı!
		perror("execve failed");
		exit(127);
	}
	else if (pid > 0)
	{
		// Parent process
		waitpid(pid, &status, 0);
		base->exit_status = WEXITSTATUS(status);
	}
	else
	{
		perror("fork failed");
		base->exit_status = 1;
	}
	// Cleanup
	free(command_path);
	free_argv(argv);
	free_envp(envp);
	return (base->exit_status);
}

void	execute_command(t_base *base)
{
	t_token	*current_promt;

	current_promt = base->token;
	if (base->token->type == TOKEN_WORD
		|| base->token->type == TOKEN_QUOTED_WORD)
	{
		if (check_build_in(current_promt->content))
			ft_build_in(current_promt, base);
		else
		{
			execute_external_command(current_promt, base);
		}
	}
}
