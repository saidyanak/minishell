/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/24 16:16:07 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

int check_build_in(char *input)
{// echo cd env export unset exit 
    return (ft_strcmp(input, "cd") || ft_strcmp(input, "echo") || ft_strcmp(input, "env") 
                || ft_strcmp(input, "pwd") || ft_strcmp(input, "unset") || ft_strcmp(input, "exit"), ft_strcmp(input, "export"));
}

void execute_command(t_base base)
{
    if (base.token->type == TOKEN_WORD)
    {
        if (check_build_in(base.token->content);)
               ft_build_in(base);
        else
            execv();
    }
}