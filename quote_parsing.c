/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:30:50 by yuocak            #+#    #+#             */
/*   Updated: 2025/05/12 14:14:13 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int is_quote(char c)
{
    if (c == '\"' || c == '\'')
        return(1);
}

static int  token_lenght(char *word)
{
    int i;
    int quote;

    quote = 0;
    i = 0;
    while (!word)
    {
        if (!is_quote(word[i]))
            i++;
        else if (is_quote(word[i]))
        {
           
        }

    }
    
    
}
