/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:34:30 by emencova          #+#    #+#             */
/*   Updated: 2024/09/27 12:40:41 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "../../minishell.h"
#include <dirent.h>

/*
static char	*cmd_find(char **path_env, char *comnd, char *path_full)
{
	char	*tmp;
	int		i;

	i = -1;
	path_full = NULL;
	while (path_env && path_env[++i])
	{
		free(path_full);
		tmp = ft_strjoin(path_env[i], "/");
		if (!tmp)
			return (NULL);
		path_full = ft_strjoin(tmp, comnd);
		free(tmp);
		if (!path_full)
			return (NULL);
		if (access(path_full, F_OK) == 0)
			break ;
	}
	if (!path_env || !path_env[i])
	{
		free(path_full);
		return (NULL);
	}
	return (path_full);
}

static DIR	*check_cmd(t_shell *shell, t_list *comnd, char ***str)
{
	t_exec	*node;
	DIR		*directory;
	char	*path_env;

	directory = NULL;
	node = comnd->content;
	if (node && node->args)
		directory = opendir((const char *)node->args);
	if (node && node->args && ft_strchr(*node->args, '/') && !directory)
	{
		*str = ft_split(*node->args, '/');
		node->path = ft_strdup(*node->args);
		free(node->args[0]);
		node->args[0] = ft_strdup(str[0][form_len(*str) - 1]);
	}
	else if (!built_check(node) && node && node->args && !directory)
	{
		path_env = get_env("PATH", shell->keys, 4);	
		*str = ft_split(path_env, ':');
		if (!*str)
   			 m_error(ERR_NEWCMD, "Failed to split PATH", 127);
		free(path_env);
		node->path = cmd_find(*str, *node->args, node->path);
		if (!node->path || !node->args[0] || !node->args[0][0])
				m_error(ERR_NEWCMD, *node->args, 127);
	}
	return (directory);
}



void command_get(t_shell *shell, t_list *comnd)
{
    t_exec *node;
    DIR *directory;
    char **str;

    str = NULL;
    node = comnd->content;
   // printf("inside builtin\n");
    if (built_check(node))
    {
        builtin(shell, comnd, &g_env.exit_status, ft_strlen(node->args[0]));
        return;
    }
    directory = check_cmd(shell, comnd, &str);
    if (directory) {
        closedir(directory);
        return;
    }
    if (node->path && access(node->path, X_OK) == 0)
	{
        pid_t pid = fork();
        if (pid < 0)
            m_error(ERR_FORK, NULL, 1);
        else if (pid == 0)
		{
            execve(node->path, node->args, shell->keys);
            m_error(ERR_ISDIR, node->args[0], 126);
        }
		else
            waitpid(pid, &g_env.exit_status, 0);
	}
    else 
        m_error(ERR_NEWDIR, node->args[0], 127);
    free_form(&str);
}

static DIR *check_cmd(t_shell *shell, t_list *comnd, char ***str)
{
    t_exec *node;
    DIR *directory;
    char *path_env;
    pid_t pid;
    int pipes[2];

    directory = NULL;
    node = comnd->content;
    if (node && node->args)
        directory = opendir((const char *)node->args);
    if (node && node->args && ft_strchr(*node->args, '/') && !directory)
    {
        *str = ft_split(*node->args, '/');
        node->path = ft_strdup(*node->args);
        free(node->args[0]);
        node->args[0] = ft_strdup(str[0][form_len(*str) - 1]);
    }
    else if (!built_check(node) && node && node->args && !directory)
    {
        path_env = get_env("PATH", shell->keys, 4);    
        *str = ft_split(path_env, ':');
        if (!*str)
            m_error(ERR_NEWCMD, "Failed to split PATH", 127);
        free(path_env);
        node->path = cmd_find(*str, *node->args, node->path);
        if (!node->path || !node->args[0] || !node->args[0][0]){
            m_error(ERR_NEWCMD, *node->args, 127);
            printf("\n");
        }
    }
    if (node->path && access(node->path, X_OK) == 0)
    {
        pid = fork();
        if (pid < 0)
            m_error(ERR_FORK, NULL, 1);
        else if (pid == 0)
        {
            handle_redirect(comnd, pipes);
            execve(node->path, node->args, shell->keys);
            m_error(ERR_ISDIR, node->args[0], 126);
        }
        else
            waitpid(pid, &g_env.exit_status, 0);
    }
    return directory;
}


void command_get(t_shell *shell, t_list *comnd)
{
    t_exec *node;
    DIR *directory;
    char **str;

    str = NULL;
    node = comnd->content;
    if (built_check(node))
    {
        builtin(shell, comnd, &g_env.exit_status, ft_strlen(node->args[0]));
        return;
    }
    directory = check_cmd(shell, comnd, &str);
    if (directory)
    {
        closedir(directory);
        return;
    }
    free_form(&str);
}

*/

static char *cmd_find(char **path_env, char *comnd, char *path_full)
{
    char *tmp;
    int i;

    i = -1;
    path_full = NULL;
    while (path_env && path_env[++i])
    {
        tmp = ft_strjoin(path_env[i], "/");
        if (!tmp)
            return (NULL);
        path_full = ft_strjoin(tmp, comnd);
        free(tmp);
        if (!path_full)
            return (NULL);
        if (access(path_full, F_OK) == 0)
            return (path_full);
        free(path_full);
        path_full = NULL;
    }
    return (NULL);
}


static DIR *check_cmd(t_shell *shell, t_list *comnd, char ***str)
{
    t_exec *node;
    DIR *directory;
    char *path_env;

    directory = NULL;
    node = comnd->content;
    if (node && node->args)
        directory = opendir((const char *)node->args);
    if (node && node->args && ft_strchr(*node->args, '/') && !directory)
    {
        *str = ft_split(*node->args, '/');
        node->path = ft_strdup(*node->args);
        free(node->args[0]);
        node->args[0] = ft_strdup(str[0][form_len(*str) - 1]);
    }
    else if (!built_check(node) && node && node->args && !directory)
    {
        path_env = get_env("PATH", shell->keys, 4);
        *str = ft_split(path_env, ':');
        if (!*str)
            m_error(ERR_PIPE, "Failed to split PATH", 127);
        free(path_env);
        node->path = cmd_find(*str, *node->args, node->path);
        if (!node->path || !node->args[0] || !node->args[0][0]){
            m_error(ERR_ISDIR, *node->args, 127);
            printf("\n");
        }
    }

    return (directory);
}
/*
void command_get(t_shell *shell, t_list *comnd)
{
    t_exec *node;
    DIR *directory;
    char **str;
    pid_t pid;
    int pipes[2];

    str = NULL;
    node = comnd->content;
    if (built_check(node))
    {
        builtin(shell, comnd, &g_env.exit_status, ft_strlen(node->args[0]));
        return;
    }
    directory = check_cmd(shell, comnd, &str);
    if (directory)
    {
        closedir(directory);
        return; // Exit if it's a directory
    }
    if (node->path && access(node->path, X_OK) == 0)
    {
        pid = fork();
        if (pid < 0)
            m_error(ERR_FORK, NULL, 1);
        else if (pid == 0)
        { 
            if (comnd->next)
            {
                if (pipe(pipes) == -1)
                    m_error(ERR_PIPE, NULL, 1);
            }
            handle_redirect(comnd, pipes);
            execve(node->path, node->args, shell->keys);
            m_error(ERR_ISDIR, node->args[0], 126);
        }
        else
        {
            if (comnd->next)
                close(pipes[PIPE_WRITE]);
            waitpid(pid, &g_env.exit_status, 0);
        }
    }

    free_form(&str);
}

*/
void command_get_single(t_shell *shell, t_list *comnd)
{
    t_exec *node;
    DIR *directory;
    char **str;
    pid_t pid;

    str = NULL;
    node = comnd->content;
    if (built_check(node))
    {
        builtin(shell, comnd, &g_env.exit_status, ft_strlen(node->args[0]));
        return;
    }
    directory = check_cmd(shell, comnd, &str);
    if (directory)
    {
        closedir(directory);
        m_error(ERR_ISDIR, node->args[0], 126);
        return;
    }
    if (node->path && access(node->path, X_OK) == 0)
    {
        pid = fork();
        if (pid < 0)
        {
            m_error(ERR_FORK, "Fork failed", 1);
            return;
        }
        else if (pid == 0)
        {
            execve(node->path, node->args, shell->keys);
            m_error(ERR_ISDIR, node->args[0], 126);
            exit(1);
        }
        else 
            waitpid(pid, &g_env.exit_status, 0);
    }

    free_form(&str);
}

void command_get_pipeline(t_shell *shell, t_list *comnd)
{
    t_exec *node;
    DIR *directory;
    char **str;

    str = NULL;
    node = comnd->content;
    if (built_check(node))
    {
        builtin(shell, comnd, &g_env.exit_status, ft_strlen(node->args[0]));
        return;
    }
    directory = check_cmd(shell, comnd, &str);
    if (directory)
    {
        closedir(directory);
        m_error(ERR_ISDIR, node->args[0], 126);
        return;
    }
    if (node->path && access(node->path, X_OK) == 0)
    {
        if (execve(node->path, node->args, shell->keys) == -1)
        {
            m_error(ERR_ISDIR, node->args[0], 126);
            exit(1);
        }
    }
    else 
        m_error(ERR_NEWCMD, node->args[0], 126);
    free_form(&str);
}


void cmd_execute(t_shell *shell, t_list *commands_list)
{
     if (!commands_list)
    {
        write(STDERR_FILENO, "Error: No commands to execute\n", 31);
        return;
    }
    
     if (commands_list->next)
     {
        execute_pipeline(shell, commands_list);
      //  printf("Multiple command execution\n");
     }
     else 
     {
        command_get_single(shell, commands_list);  
   //     printf("Single command execution\n");
     }
}