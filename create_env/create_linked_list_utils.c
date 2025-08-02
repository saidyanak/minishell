/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_linked_list_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:52:07 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/02 14:53:14 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env_null_check(t_env *env_lst)
{
	if (!env_lst)
	{
		printf("minishell: error initializing environment\n");
		exit(1);
	}
}

void	update_shlvl(t_base *base)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_value;

	shlvl_str = get_env_value(*base, "SHLVL");
	shlvl = ft_atoi(shlvl_str);
	shlvl++;
	new_value = ft_itoa(shlvl);
	set_env_value(&(base->env), "SHLVL", new_value);
	free(new_value); 
}
