/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 11:59:25 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/04 15:02:49 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <unistd.h>

static int	is_get_env_value(t_base base)
{
	write(1, get_value(base.env, "PWD"), ft_strlen(get_value(base.env, "PWD")));
	write(1, "\n", 1);
	base.exit_status = 0;
	return (base.exit_status);
}

int	ft_pwd(t_base base)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
		free(cwd);
		base.exit_status = 0;
		return (base.exit_status);
	}
	else
	{
		if (get_value(base.env, "PWD") != NULL)
		{
			return (is_get_env_value(base));
		}
		else
		{
			perror("getcwd() error");
			base.exit_status = 1;
			return (base.exit_status);
		}
	}
}
