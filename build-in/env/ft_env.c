/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:45:26 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/18 19:52:07 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <unistd.h>

void	check_and_write(t_env *head)
{
	if (head->key && head->value && head->exported)
	{
		write(STDOUT_FILENO, head->key, ft_strlen(head->key));
		write(STDOUT_FILENO, "=", 1);
		write(STDOUT_FILENO, head->value, ft_strlen(head->value));
		write(STDOUT_FILENO, "\n", 1);
	}
}

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
	{
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(base->token->next->content, 2);
		ft_putstr_fd("’: No such file or directory\n", 2);
		return ((base->exit_status = 127));
	}
	while (head != NULL)
	{
		check_and_write(head);
		head = head->next;
	}
	base->exit_status = 0;
	return (base->exit_status);
}
