/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environmente_set.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 19:24:57 by yasmine           #+#    #+#             */
/*   Updated: 2024/09/23 16:09:12 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../exec/execute.h"

char **update_or_add_env_var(char *variable, char *value, t_shell *shell)
{
    int i;
    int len;
    char *new_value;
    char    *tmp;

    i = 0;
    len = ft_strlen(variable);
    while(shell->keys[i])
    {
        if (ft_strncmp(shell->keys[i], variable, len - 1) == 0
            && shell->keys[i][len] == '=')
            break;
        i++;
    }
    if (shell->keys[i] == NULL)
        append_to_env(variable, value, i, shell);
    else
    {
        free(shell->keys[i]);
        tmp = ft_strjoin(variable, "=");
		new_value = ft_strjoin(tmp, value);
		free(tmp);
		shell->keys[i] = new_value;
    }
    char **sorted = dup_array(shell->keys);
    sort_array(sorted);
    free_str_array(sorted);
    return (shell->keys);
}

