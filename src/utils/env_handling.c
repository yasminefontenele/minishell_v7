/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:06:17 by yasmine           #+#    #+#             */
/*   Updated: 2024/09/23 16:08:59 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../exec/execute.h"

//searches for the environment variable by name and when it finds it,
//updates its value.
void env_update(char *env, char *new_value, t_shell *shell)
{
    int i;
    size_t len;
    char **sorted_env;

    len = ft_strlen(env);
    i = -1;
    while (shell->keys[++i])
    {
        if (ft_strncmp(env, shell->keys[i], len) == 0)
        {
            replace_value(i, new_value, len, shell->keys);
            break;
        }
    }
    sorted_env = dup_array(shell->keys);
    sort_array(sorted_env);
    i = -1;
    while (sorted_env[++i])
    {
        if (ft_strncmp(env, sorted_env[i], len) == 0)
        {
            replace_value(i, new_value, len, sorted_env);
            break;
        }
    }
    free_str_array(sorted_env);
}



//replaces the environment variable string,
//preserving the name and changing only the value.
void replace_value(int i, char *new_value, int len, char **arr)
{
    int j;
    int k;
    char *new_var;
    int new_len;

    j = 0;
    new_len = ft_strlen(new_value);
    new_var = malloc(sizeof(char) * (len + new_len + 1));
    if (new_var == NULL)
        ft_error("malloc failed", 1);
    while (j++ < len)
        new_var[j] = arr[i][j];
    k = 0;
    while (k++ < new_len)
    {
        new_var[j] = new_value[k];
        j++;
    }
    new_var[j] = '\0';
    free(arr[i]);
    arr[i] = new_var;
}
