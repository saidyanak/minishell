/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:15:14 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/16 16:03:34 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../libft/libft.h"

int	has_equal_sign(char *content)
{
	int	i;

	i = 0;
	while (content[i])
	{
		if (content[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

char	*parse_token(t_token *token)
{
	char	*equal_sign;
	int		key_len;

	if (!token || !token->content)
		return (NULL);
	equal_sign = ft_strchr(token->content, '=');
	if (equal_sign)
	{
		key_len = equal_sign - token->content;
		return (ft_strndup(token->content, key_len));
	}
	return (ft_strdup(token->content));
}

char	*parse_value(t_token *token)
{
	char	*equal_sign;

	if (!token || !token->content)
		return (NULL);
	equal_sign = ft_strchr(token->content, '=');
	if (equal_sign)
		return (ft_strdup(equal_sign + 1));
	return (NULL);
}

int	is_valid_identifier(char *key)
{
	int	i;

	if (!key || !key[0])
		return (0);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
