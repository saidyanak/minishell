/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 00:00:00 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/18 15:50:51 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

void	cleanup_base_normal(t_base *base)
{
	if (base->token)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	cleanup_heredocs(base);
	if (base->env)
	{
		free_env_list(base->env);
		base->env = NULL;
	}
}

void	cleanup_base_with_data(t_base *base)
{
	if (base->token)
	{
		free_tokens(base->token);
		base->token = NULL;
	}
	cleanup_heredocs(base);
	if (base->env)
	{
		free_env_list(base->env);
		base->env = NULL;
	}
	free_child_arg(base->data);
	base->data = NULL;
}
