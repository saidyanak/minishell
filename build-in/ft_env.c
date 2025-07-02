/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:45:26 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/02 11:48:48 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>

int	ft_env(t_base *base)
{
	t_env	*head;

	head = base->env;
	if (!base || !base->env)
	{
		base->exit_status = 0;
		return (base->exit_status);
	}
	head = base->env;
	while (head != NULL)
	{
		if (head->key && head->value && head->exported)
		{
			write(1, head->key, ft_strlen(head->key));
			write(1, "=", 1);
			write(1, head->value, ft_strlen(head->value));
			write(1, "\n", 1);
		}
		head = head->next;
	}
	base->exit_status = 0;
	return (base->exit_status);
}
