/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/04 15:07:24 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <linux/limits.h>

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

static void	update_pwd_vars(t_env **env, char *old_dir)
{
	char	new_dir[PATH_MAX];

	set_env_value(env, "OLDPWD", old_dir);
	if (getcwd(new_dir, PATH_MAX))
		set_env_value(env, "PWD", new_dir);
}

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
