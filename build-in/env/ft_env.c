/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:45:26 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/13 16:14:27 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

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
	if (base->token->next && base->token->next->type == TOKEN_WORD)
		return ((base->exit_status = 127));
	while (head != NULL)
	{
		if (head->key && head->value && head->exported)
		{
			write(STDOUT_FILENO, head->key, ft_strlen(head->key));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, head->value, ft_strlen(head->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		head = head->next;
	}
	base->exit_status = 0;
	return (base->exit_status);
}
