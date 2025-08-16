/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:14:55 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 16:04:01 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../libft/libft.h"

void	sort_and_print_env(t_env *env)
{
	t_env	**array;
	int		count;
	int		i;

	count = count_env_vars(env);
	if (count == 0)
		return ;
	array = create_env_array(env, count);
	if (!array)
		return ;
	sort_env_array(array, count);
	i = 0;
	while (i < count)
	{
		print_export_var(array[i]);
		i++;
	}
	free(array);
}

int	process_export_args(t_token *token, t_base *base)
{
	t_token	*current;
	int		exit_status;

	current = token->next;
	exit_status = 0;
	while (current)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
		{
			if (add_or_update_env(current, base) != 0)
				exit_status = 1;
		}
		current = current->next;
	}
	return (exit_status);
}

void	print_export(t_base *base)
{
	t_env	**array;
	int		count;
	int		i;

	count = count_env_vars(base->env);
	if (count == 0)
		return ;
	array = create_env_array(base->env, count);
	if (!array)
		return ;
	sort_env_array(array, count);
	i = 0;
	while (i < count)
	{
		print_export_var(array[i]);
		i++;
	}
	free(array);
}

t_base	*ft_export(t_token *token, t_base *base)
{
	if (!token || !base)
		return (base);
	if (!token->next)
	{
		print_export(base);
		base->exit_status = 0;
		return (base);
	}
	base->exit_status = process_export_args(token, base);
	return (base);
}
