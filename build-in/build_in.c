/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:08:48 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/31 19:40:10 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// echo export unset exit cd pwd
void	ft_build_in(t_token *current_prompt, t_base *base)
{
	if (ft_strcmp(current_prompt->content, "echo") == 0)
		base->exit_status = ft_echo(current_prompt, *base);
	else if (ft_strcmp(current_prompt->content, "cd") == 0)
		ft_cd(current_prompt, base);
	else if (ft_strcmp(current_prompt->content, "pwd") == 0)
		base->exit_status = ft_pwd(*base);
	else if (ft_strcmp(current_prompt->content, "env") == 0)
		ft_env(base);
	else if (ft_strcmp(current_prompt->content, "export") == 0)
		ft_export(current_prompt, base);
	else if (ft_strcmp(current_prompt->content, "unset") == 0)
		ft_unset(current_prompt, base);
	else if (ft_strcmp(current_prompt->content, "exit") == 0)
		ft_exit(current_prompt, base);
	debug_exit_status(base, "build_in.c:ft_build_in", current_prompt->content);
}
