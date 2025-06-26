/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/26 17:35:51 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

int check_build_in(char *input)
{
    return (ft_strcmp(input, "cd") == 1 || ft_strcmp(input, "echo") == 1 || ft_strcmp(input, "env") == 1 
                || ft_strcmp(input, "pwd") == 1 || ft_strcmp(input, "unset") == 1 || ft_strcmp(input, "exit") == 1 || ft_strcmp(input, "export") == 1);
}

void execute_command(t_base base)
{
    t_token *current_promt;

    current_promt = base.token;
    if (base.token->type == TOKEN_WORD)
    {
        if (check_build_in(current_promt->content))
            ft_build_in(current_promt, base); 
        //else
            //execv();
    }
}