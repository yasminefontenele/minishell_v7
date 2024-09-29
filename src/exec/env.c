/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:16:06 by emencova          #+#    #+#             */
/*   Updated: 2024/09/25 21:39:36 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

char	**set_env(char *str, char *val, char **keys, int nbr)
{
	int		i;
	int		key_len;
	char	*new_entry;
	char	*full_entry;
	
	if (nbr < 0)
		nbr = ft_strlen(str);	
	full_entry = ft_strjoin(str, "=");
	new_entry = ft_strjoin(full_entry, val);
	free(full_entry);
	i = 0;
	while (keys && keys[i])
	{
		key_len = second_strchr(keys[i], '=');
		if (ft_strncmp(keys[i], str, nbr) == 0 && (key_len >= nbr))
		{
			free(keys[i]);
			keys[i] = new_entry;
			return (keys);
		}
		i++;
	}
	keys = extend_form(keys, new_entry);
	return (keys);
}

char	*get_env(char *name, char **env, int name_len)
{
	int	i;
	int	n;

	i = 0;
	if (name_len < 0)
		name_len = ft_strlen(name);
	while (!ft_strchr(name, '=') && env && env[i])
	{
		n = name_len;
		if (n < second_strchr(env[i], '='))
			n = second_strchr(env[i], '=');
		if (!ft_strncmp(env[i], name, n))
			return (ft_substr(env[i], n + 1, ft_strlen(env[i])));
		i++;
	}
	return (NULL);
}