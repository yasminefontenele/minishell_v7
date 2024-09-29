/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_env_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:15:49 by emencova          #+#    #+#             */
/*   Updated: 2024/09/27 13:44:04 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "../../minishell.h"


/*
int find_key_idx(char **keys, char *key)
{
    int i ;
    int key_len; 

	i = 0;
	key_len= second_strchr(key, '=');
    if (key_len == -1)
        return -1;
    while (keys[i])
    {
        if (!ft_strncmp(keys[i], key, key_len + 1))
            return (i);
        i++;
    }

    return (-1);
}

int	m_export(t_shell *shell)
{
	int		i;
	int		index;;
	char	**av;
    int j;
    char **keys;

	av = ((t_exec *)shell->cmds->content)->args;
	i = 1;
    j = 0;
    keys = shell->keys;
	if (av[1] == 0)
    {
        while (keys[j])
        {
        printf("%s\n", keys[i]);
        j++;
        }
        return (0);
    }
	while (av[i])
	{
		index = find_key_idx(shell->keys, av[i]);
		if (index != -1)
		{
			free(shell->keys[index]);
			shell->keys[index] = ft_strdup(av[i]);
		}
		else
			shell->keys = extend_form(shell->keys, av[i]);
		i++;
	}
	return (0);
}*/

char *ft_strndup(char *src, int n)
{
    char *dest;
    int len;
    int i;

    len = 0;
    i = 0;
    while (src[len] && len < n)
        len++;
    dest = (char *)malloc(len + 1);
    if (!dest)
        return (NULL);

    while (i < len)
    {
        dest[i] = src[i];
        i++;
    }
    dest[len] = '\0';
    return (dest);
}

int find_key_idx(char **keys,char *key)
{
    int i;
    int key_len;
    char *equal_sign;
    
    i = 0;
    equal_sign = ft_strchr(key, '=');
    if (equal_sign)
        key_len = equal_sign - key;
    else
        key_len = ft_strlen(key);

    while (keys[i])
    {
        if (!ft_strncmp(keys[i], key, key_len) && keys[i][key_len] == '=')
            return (i);
        i++;
    }

    return (-1);
}

int m_unset(t_shell *shell)
{
    char **av;
	char *var_env;
    int i;
    int index;

	i = 1;
	av = ((t_exec *)shell->cmds->content)->args;
    if (form_len(av) < 2)
        return (0);
    while (av[i])
    {
        if (av[i][ft_strlen(av[i]) - 1] != '=')
        {
            var_env = ft_strjoin(av[i], "=");
            free(av[i]);
            av[i] = var_env;
        }
        index = find_key_idx(shell->keys, av[i]);
        if (index != -1)
        {
            free(shell->keys[index]);
            while (shell->keys[index])
            {
                shell->keys[index] = shell->keys[index + 1];
                index++;
            }
        }
        i++;
    }
    return (0);
}

int m_env(t_shell *shell)
{
    int i;

    i = 0;
    if (!shell || !shell->keys)
    {
        ft_error("No enviroment is set", 1);
    }
    while (shell->keys[i])
    {
        printf("%s\n", shell->keys[i]);
        i++;
    }
    return (0);
}
