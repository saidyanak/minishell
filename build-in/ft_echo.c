/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:18:32 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/26 17:30:33 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <unistd.h>

int is_valid_n_flag(char *str)
{
    int i;

    i = 0;
    if (!str || str[0] != '-')
        return (0);
    i = 1;
    if (str[i] == '\0')
        return (0);
    while (str[i])
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    return(1);
}

int ft_echo(t_token *current_prompt, t_base base)
{
    (void)base;
    t_token *current;
    int newline;
    int first_arg;

    current = current_prompt->next;
    newline = 1;
    first_arg = 1;
    while (current && current->content
        && current->type == TOKEN_WORD
        && is_valid_n_flag(current->content))
        {
            newline = 0;
            current = current->next;
        }
    while (current && current->content)
    {
        if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
        {
            if (!first_arg)
                write(1, " ", 1);
            write(1, current->content, ft_strlen(current->content));
        }
        else if (current->type == TOKEN_PIPE || 
            current->type == TOKEN_REDIRECT_IN ||
            current->type == TOKEN_REDIRECT_OUT ||
            current->type == TOKEN_APPEND ||
            current->type == TOKEN_HEREDOC)
                break;
        current = current->next;
    }
    if (newline)
        write(1, "\n", 1);
    base.exit_status = 0;
    return(0);
}