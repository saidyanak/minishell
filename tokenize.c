/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:38:34 by yuocak            #+#    #+#             */
/*   Updated: 2025/05/07 13:22:51 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "minishell.h"

static int  ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

static int  count_words(char *input)
{
    int count = 0;
    int in_word = 0;

    while (*input)
    {
        if (!ft_isspace(*input) && in_word == 0)
        {
            in_word = 1;
            count++;
        }
        else if (ft_isspace(*input))
            in_word = 0;
        input++;
    }
    return (count);
}

static char *copy_word(char *start, int len)
{
    char    *word;

    word = malloc(sizeof(char) * (len + 1));
    if (!word)
        return (NULL);
    strncpy(word, start, len);
    word[len] = '\0';
    return (word);
}

char **tokenize_input(char *input)
{
    int     word_count;
    char    **tokens;
    char    *start;
    int     i;

    word_count = count_words(input);
    tokens = malloc(sizeof(char *) * (word_count + 1));
    if (!tokens)
        return (NULL);
    i = 0;
    while (*input)
    {
        while (*input && ft_isspace(*input))
            input++;
        if (*input)
        {
            start = input;
            while (*input && !ft_isspace(*input))
                input++;
            tokens[i] = copy_word(start, input - start);
            i++;
        }
    }
    tokens[i] = NULL;
    return (tokens);
}
