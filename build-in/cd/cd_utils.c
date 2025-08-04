/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:04:12 by syanak            #+#    #+#             */
/*   Updated: 2025/08/04 15:07:11 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static void	add_new_nodes(t_env *new_node, t_env **env, char *new_value)
{
	new_node = create_new_node(new_value);
	if (!new_node)
		return ;
	add_new_node(env, new_node);
	new_node->exported = 0;
}

void	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;
	char	*new_value;

	current = *env;
	new_value = NULL;
	new_node = NULL;
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
	add_new_nodes(new_node, env, new_value);
}

char	*get_home_dir(t_env *env)
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

char	*get_oldpwd_dir(t_env *env)
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
