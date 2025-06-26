/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:08:48 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/24 16:19:18 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// echo export unset exit cd pwd 
void ft_build_in(t_token *current_prompt, t_base *base)
{
    if (ft_strcmp(current_prompt->content, "echo"))//aocak
        ft_echo(current_prompt, base); 
    else if (ft_strcmp(current_prompt->content, "cd")) //said
        ft_cd(current_prompt, base);
    else if (ft_strcmp(current_prompt->content, "pwd"))// aocak
        ft_pwd(current_prompt, base);
    else if (ft_strcmp(current_prompt->content, "unset")) //said
        ft_unset(current_prompt, base);
    else if (ft_strcmp(current_prompt->content, "export")) //said
        ft_export(current_prompt, base);
    else if (ft_strcmp(current_prompt->content, "exit")) //said
        ft_exit(current_prompt, base);
    else if (ft_strcmp(current_prompt->content, "env"))//aocak
        ft_env(current_prompt, base);
}
