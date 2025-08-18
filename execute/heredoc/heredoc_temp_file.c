/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_temp_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuocak <yuocak@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:39:24 by yuocak            #+#    #+#             */
/*   Updated: 2025/08/18 15:51:03 by yuocak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../minishell.h"
#include <unistd.h>

char	*create_temp_filename(void)
{
	char	*pid_str;
	char	*counter_str;
	char	*temp;
	char	*temp_file;

	temp_file = NULL;
	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (NULL);
	counter_str = ft_itoa(get_unique_heredoc_id());
	if (!counter_str)
	{
		free(pid_str);
		return (NULL);
	}
	temp = ft_strjoin(pid_str, "_");
	free(pid_str);
	if (!temp)
	{
		free(counter_str);
		return (NULL);
	}
	temp_file = unique_name_return(temp, counter_str, temp_file);
	return (temp_file);
}
