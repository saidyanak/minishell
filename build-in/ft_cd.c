/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:30:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/24 16:30:00 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Environment variable helper functions
char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return;
		}
		current = current->next;
	}
	
	// If key doesn't exist, create new node
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return;
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = *env;
	*env = new_node;
}

static char	*get_target_path(t_token *token, t_env *env)
{
	char	*path;
	char	*home;

	// Move to the argument after "cd"
	if (token && token->next)
		token = token->next;
	
	// If no argument provided, go to HOME
	if (!token || !token->content)
	{
		home = get_env_value(env, "HOME");
		if (!home)
		{
			printf("minishell: cd: HOME not set\n");
			return (NULL);
		}
		return (ft_strdup(home));
	}
	
	// Handle "cd -" (go to previous directory)
	if (ft_strcmp(token->content, "-") == 0)
	{
		path = get_env_value(env, "OLDPWD");
		if (!path)
		{
			printf("minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
		printf("%s\n", path); // Print the directory we're going to
		return (ft_strdup(path));
	}
	
	// Regular path
	return (ft_strdup(token->content));
}

static int	change_directory(char *path, t_base *base)
{
	char	*old_pwd;
	char	cwd[1024];

	// Get current directory before changing
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("minishell: cd: getcwd");
		return (1);
	}
	old_pwd = ft_strdup(cwd);
	
	// Try to change directory
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		free(old_pwd);
		return (1);
	}
	
	// Update OLDPWD with previous directory
	set_env_value(&(base->env), "OLDPWD", old_pwd);
	
	// Update PWD with new directory
	if (getcwd(cwd, sizeof(cwd)))
		set_env_value(&(base->env), "PWD", cwd);
	
	free(old_pwd);
	return (0);
}

void	ft_cd(t_token *current_prompt, t_base base)
{
	char	*target_path;
	int		result;

	if (!current_prompt || !base)
		return;
	
	// Birden fazl argüman hatası
	if (current_prompt->next && current_prompt->next->next)
	{
		printf("minishell: cd: too many arguments\n");
		base->exit_status = 1;
		return;
	}
	
	// Target path i alıyruz.
	target_path = get_target_path(current_prompt, base->env);
	if (!target_path)
	{
		base->exit_status = 1;
		return;
	}
	
	// Caliştığımız dizini değiştirio setliyoruz
	result = change_directory(target_path, base);
	base->exit_status = result;
	printf("%s\n", target_path);
	free(target_path);
}