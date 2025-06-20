/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:31 by yuocak            #+#    #+#             */
/*   Updated: 2025/06/04 13:00:32 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv, char **env)
{
    char    *input;
    char    **args;
    t_base  my_base;

    (void)argc;
    (void)argv;
    my_base.env = init_env(env);
    while(1)
    {
        input = readline("minishell$ ");
        if (!input)
            break;
        tokenize_input(input, &my_base);
    }
    t_token *head;
    
    head = my_base->token;
    while (head->next)
    {
        printf("%s", my_base->token->content);
        printf("\n");
        head = head->next;
    }
}
