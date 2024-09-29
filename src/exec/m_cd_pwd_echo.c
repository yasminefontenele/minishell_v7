/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:15:09 by emencova          #+#    #+#             */
/*   Updated: 2024/09/25 21:35:11 by eliskam          ###   ########.fr       */
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


/*
int	m_cd(t_shell *shell)
{
	char	**str[2];
	char	*home_dir;
	char	*current_dir;

	g_env.exit_status = 0;
	str[0] = ((t_exec *)shell->cmds->content)->args;

   // current_dir = getcwd(NULL, 1024);
  //  printf("my directory is = %s\n ", current_dir);

	home_dir = get_env("HOME", shell->keys, 4);
	if (!home_dir)
		home_dir = ft_strdup(" ");
	str[1] = extend_form(NULL, home_dir);
	free(home_dir);
	current_dir = getcwd(NULL, 0);
	str[1] = extend_form(str[1], current_dir);
	free(current_dir);
	error_cd(str, );
	if (!g_env.exit_status)
		shell->keys = set_env("OLDPWD", str[1][1], shell->keys, 6);
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		current_dir = ft_strdup(" ");
	str[1] = extend_form(str[1], current_dir);
	free(current_dir);
	shell->keys = set_env("PWD", str[1][2], shell->keys, 3);
	free_form(&str[1]);
	return (g_env.exit_status);
}*/

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

int m_echo(char **args, t_shell *shell)
{
    int i;
    int no_newline;
    char *processed_value;
    
    i = 1;
    no_newline = 0;
    if (args[i] && strcmp(args[i], "-n") == 0)
    {
        no_newline = 1;
        i++;
    }
    while (args[i])
    {
        processed_value = process_quotes(args[i], shell);
        ft_putstr_fd(processed_value, STDOUT_FILENO);
        free(processed_value);
        i++;
        if (args[i])
            ft_putchar_fd(' ', STDOUT_FILENO);
    }
    if (!no_newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
    return (0);
}

