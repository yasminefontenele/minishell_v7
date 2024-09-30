/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_cd_pwd_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:15:09 by emencova          #+#    #+#             */
/*   Updated: 2024/09/30 23:13:04 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int m_cd(t_shell *shell)
{
    char    **args;
    char    *home_dir;
    char    *current_dir;
    char    *target_dir;

    g_env.exit_status = 0;
    args = ((t_exec *)shell->cmds->content)->args;
    if (args[1] && args[2])
    {
        ft_putstr_fd("cd: too many arguments\n", 2);
        g_env.exit_status = 1;
        return (g_env.exit_status);
    }
    home_dir = get_env("HOME", shell->keys, 4);
    if (!home_dir)
    {
        ft_putstr_fd("minishell: cd: HOME not set\n", 2);
        g_env.exit_status = 1;
        return g_env.exit_status;
    }
    if (!args[1] || ft_strcmp(args[1], "") == 0)
    {
        target_dir = ft_strdup(home_dir);
    }
    else if (ft_strcmp(args[1], "-") == 0)
    {
        target_dir = get_env("OLDPWD", shell->keys, 6);
        if (!target_dir)
        {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
            g_env.exit_status = 1;
            free(home_dir);
            return g_env.exit_status;
        }
    }
    else
        target_dir = ft_strdup(args[1]);
    error_cd(args, target_dir);
    if (!g_env.exit_status)
    {
        current_dir = getcwd(NULL, 1024);
        if (current_dir)
        {
            shell->keys = set_env("OLDPWD", current_dir, shell->keys, 6);  // Set OLDPWD
            shell->keys = set_env("PWD", current_dir, shell->keys, 3);     // Set PWD
            free(current_dir);
        }
    }

    free(home_dir);
    free(target_dir);
    return g_env.exit_status;
}

int	m_pwd(void)
{
	char *buffer;

    buffer = getcwd(NULL, 1024);  // Altere 0 para um valor padrão, como 1024
    if (!buffer)
    {
        perror("pwd\n");
        return (1);
    }
    ft_putendl_fd(buffer, 1);
    free(buffer);
    return (0);
}

int	char_count(char *str, char c)
{
	int	nbr;

	if (!str)
		return (0);
	nbr = 0;
	while (*str)
	{
		if (*str == c)
			nbr++;
		str++;
	}
	return (nbr);
}

int m_echo(char **args)
{
    int new_line;
    int valid_arg;
    int i;

    new_line = 1;
    valid_arg = 0;
    i = 1;
    if (!args || !args[0])
        return (0);
    while (args[i])
    {
        if (!valid_arg && !ft_strncmp(args[i], "-n", 2) &&
            (char_count(args[i], 'n') == (int)(ft_strlen(args[i]) - 1)))
            new_line = 0;
        else
        {
            valid_arg = 1;
            ft_putstr_fd(args[i], STDOUT_FILENO);
            if (args[i + 1])
                ft_putchar_fd(' ', STDOUT_FILENO);
        }
        i++;
    }
    if (valid_arg && new_line)
        ft_putchar_fd('\n', STDOUT_FILENO);
    return (0);
}

