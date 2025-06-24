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
void ft_build_in(t_base base)
{
    if (ft_strcmp(base.token->content, "echo"))//aocak
        ft_echo();   
    else if (ft_strcmp(base.token->content, "cd")) //said
        ft_cd();
    else if (ft_strcmp(base.token->content, "pwd"))// aocak
        ft_pwd();
    else if (ft_strcmp(base.token->content, "unset")) //said
        ft_unset();
    else if (ft_strcmp(base.token->content, "export")) //said
        ft_export();
    else if (ft_strcmp(base.token->content, "exit")) //said
        ft_exit();
    else if (ft_strcmp(base.token->content, "env"))//aocak
        ft_env();
}
