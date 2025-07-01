/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:07:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/01 12:51:29 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>

int check_build_in(char *input)
{
    return (ft_strcmp(input, "cd") == 0 || ft_strcmp(input, "echo") == 0 || ft_strcmp(input, "env") == 0 
                || ft_strcmp(input, "pwd") == 0 || ft_strcmp(input, "unset") == 0 || ft_strcmp(input, "exit") == 0 || ft_strcmp(input, "export") == 0);
}

void execute_command(t_base base)
{
    t_token *current_promt;

    current_promt = base.token;
    if (base.token->type == TOKEN_WORD || base.token->type == TOKEN_QUOTED_WORD) // Enumlar 0123456 (TOKEN_WORD = 0 TOKEN_QUOTED_WORD = 6) diye sıralanır sadece token olduğuna bakarsak tırnek içerindeki word u build in de ele alamayız.
    {
        if (check_build_in(current_promt->content))
            ft_build_in(current_promt, base); 
        //else
            //execv();
    }
}