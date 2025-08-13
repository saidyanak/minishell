/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:05:21 by syanak            #+#    #+#             */
/*   Updated: 2025/08/13 20:05:53 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

int	is_valid_unset_name(char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!((name[0] >= 'a' && name[0] <= 'z') || (name[0] >= 'A'
				&& name[0] <= 'Z') || name[0] == '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A'
					&& name[i] <= 'Z') || (name[i] >= '0' && name[i] <= '9')
				|| name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	handle_unset_error(char *name)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

int	process_unset_arg(char *arg, t_base *base)
{
	if (ft_strchr(arg, '='))
		return (0);
	if (!is_valid_unset_name(arg))
		return (handle_unset_error(arg));
	unset_env_var(&(base->env), arg);
	return (0);
}

int	process_unset_args(t_token *token, t_base *base)
{
	t_token	*current;
	int		exit_status;

	current = token->next;
	exit_status = 0;
	while (current)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
		{
			if (process_unset_arg(current->content, base) != 0)
				exit_status = 1;
		}
		current = current->next;
	}
	return (exit_status);
}

t_base	*ft_unset(t_token *token, t_base *base)
{
	if (!token || !base)
		return (base);
	if (!token->next)
	{
		base->exit_status = 0;
		return (base);
	}
	base->exit_status = process_unset_args(token, base);
	return (base);
}
