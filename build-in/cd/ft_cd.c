/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/18 19:52:27 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../minishell.h"
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>

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
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (0);
	}
	if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (0);
	}
	return (1);
}

static void	update_pwd_vars(t_env **env, char *old_dir, char *target)
{
	char	new_dir[PATH_MAX];
	char	*current_pwd;
	char	*updated_pwd;

	set_env_value(env, "OLDPWD", old_dir);
	if (getcwd(new_dir, PATH_MAX))
		set_env_value(env, "PWD", new_dir);
	else
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd:\
			 cannot access parent directories: No such file or directory\n", 2);
		current_pwd = get_value(*env, "PWD");
		if (current_pwd && ft_strcmp(target, "..") == 0)
		{
			updated_pwd = ft_strjoin(current_pwd, "/..");
			set_env_value(env, "PWD", updated_pwd);
			free(updated_pwd);
		}
	}
}

static int	change_to_dir(char *path, t_env **env)
{
	char	current_dir[PATH_MAX];
	char	*pwd_value;

	pwd_value = get_value(*env, "PWD");
	if (pwd_value)
		ft_strlcpy(current_dir, pwd_value, PATH_MAX);
	else if (!getcwd(current_dir, PATH_MAX))
	{
		if (pwd_value)
			ft_strlcpy(current_dir, pwd_value, PATH_MAX);
		else
			ft_strlcpy(current_dir, "/", PATH_MAX);
	}
	if (chdir(path) != 0)
		return (perror("minishell: cd"), 1);
	update_pwd_vars(env, current_dir, path);
	return (0);
}

t_base	*ft_cd(t_token *token, t_base *base)
{
	char	*target;
	int		arg_count;

	if (!token || !base)
		return (base);
	arg_count = count_cd_arguments(token);
	if (arg_count > 1)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
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
