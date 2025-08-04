/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/04 14:43:38 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <linux/limits.h>

// Environment'tan değer al
static char	*get_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

// Environment'a değer set et
void	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;
	char	*new_value;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			new_value = malloc(ft_strlen(value) + 1);
			if (!new_value)
				return ;
			ft_strlcpy(new_value, value, ft_strlen(value) + 1);
			if (current->value)
				free(current->value);
			current->value = new_value;
			return ;
		}
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = malloc(ft_strlen(key) + 1);
	new_node->value = malloc(ft_strlen(value) + 1);
	if (!new_node->key || !new_node->value)
	{
		if (new_node->key)
			free(new_node->key);
		if (new_node->value)
			free(new_node->value);
		free(new_node);
		return ;
	}
	ft_strlcpy(new_node->key, key, ft_strlen(key) + 1);
	ft_strlcpy(new_node->value, value, ft_strlen(value) + 1);
	new_node->exported = 0;
	new_node->next = *env;
	*env = new_node;
}

// HOME dizini al
static char	*get_home_dir(t_env *env)
{
	char	*home;

	home = get_value(env, "HOME");
	if (!home)
	{
		printf("minishell: cd: HOME not set\n");
		return (NULL);
	}
	return (home);
}

// OLDPWD dizini al ve yazdır
static char	*get_oldpwd_dir(t_env *env)
{
	char	*oldpwd;

	oldpwd = get_value(env, "OLDPWD");
	if (!oldpwd)
	{
		printf("minishell: cd: OLDPWD not set\n");
		return (NULL);
	}
	printf("%s\n", oldpwd);
	return (oldpwd);
}

// Hedef dizini belirle, - yi de handlladım
static char	*get_target_dir(t_token *token, t_env *env)
{
	t_token	*arg;

	arg = token->next;
	if (!arg)
		return (get_home_dir(env));
	if (ft_strcmp(arg->content, "-") == 0)
		return (get_oldpwd_dir(env));
	return (arg->content);
}

// erişim kontrolü
static int	check_dir_access(char *path)
{
	if (access(path, F_OK) != 0)
	{
		printf("minishell: cd: %s: No such file or directory\n", path);
		return (0);
	}
	if (access(path, X_OK) != 0)
	{
		printf("minishell: cd: %s: Permission denied\n", path);
		return (0);
	}
	return (1);
}

// envdeki PWD ve OLDPWD güncelle
static void	update_pwd_vars(t_env **env, char *old_dir)
{
	char	new_dir[PATH_MAX];

	set_env_value(env, "OLDPWD", old_dir);
	if (getcwd(new_dir, PATH_MAX))
		set_env_value(env, "PWD", new_dir);
}

// Dizin değiştir chdir ile
static int	change_to_dir(char *path, t_env **env)
{
	char	current_dir[PATH_MAX];

	if (!getcwd(current_dir, PATH_MAX))
	{
		perror("minishell: cd: getcwd");
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	update_pwd_vars(env, current_dir);
	return (0);
}

t_base	*ft_cd(t_token *token, t_base *base)
{
	char	*target;

	if (!token || !base)
		return (base);
	if (token->next && token->next->next)
	{
		printf("minishell: cd: too many arguments\n");
		base->exit_status = 1;
		return (base);
	}
	target = get_target_dir(token, base->env);
	if (!target)
	{
		base->exit_status = 1;
		return (base);
	}
	if (!check_dir_access(target))
	{
		base->exit_status = 1;
		return (base);
	}
	base->exit_status = change_to_dir(target, &(base->env));
	return (base);
}
