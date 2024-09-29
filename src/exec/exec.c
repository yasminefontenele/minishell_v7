/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 22:31:57 by emencova          #+#    #+#             */
/*   Updated: 2024/09/27 12:43:29 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <dirent.h>

/*
void execute_pipeline(t_shell *shell, t_list *commands_list)
{
    int fd[2];
    t_list *cmd_node = commands_list;
    t_exec *current_cmd;
    pid_t pid;

    while (cmd_node)
    {
        current_cmd = (t_exec *)cmd_node->content;

        if (!current_cmd)
        {
            write(STDERR_FILENO, "Error: current_cmd is null in execute_pipeline\n", 48);
            return;
        }
        if (cmd_node->next)
        {
            if (pipe(fd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
            pid = fork();
            printf("after fork\n");
            if (pid == -1)
            {
                printf("this error im checking\n");
                exit(EXIT_FAILURE);
            }
            if (pid == 0)
            {
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
                command_get(shell, cmd_node);
                exit(EXIT_SUCCESS);
            }
            else
            {
                close(fd[1]);
                current_cmd->out = fd[1];
                if (cmd_node->next)
                {
                    t_exec *next_cmd = (t_exec *)cmd_node->next->content;
                    next_cmd->in = fd[0];
                }
            }
        }
        else
        {
            printf("befoe command get\n");
            command_get(shell, cmd_node);
            printf("after command get\n");
            
        }
        cmd_node = cmd_node->next; // Move to the next command
    }
}
*/

void execute_pipeline(t_shell *shell, t_list *commands_list)
{
    int fd[2];
    t_list *cmd_node = commands_list;
    t_exec *current_cmd;

    cmd_node = commands_list;
    if (!cmd_node->next)
    {
        command_get_single(shell, cmd_node);
        return;
    }
    while (cmd_node)
    {
        current_cmd = (t_exec *)cmd_node->content;
        if (!current_cmd)
        {
            write(STDERR_FILENO, "Error: current_cmd is null in execute_pipeline\n", 48);
            return;
        }
        if (cmd_node->next)
        {
            if (pipe(fd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            handle_redirect(cmd_node, fd);
            command_get_pipeline(shell, cmd_node);
            exit(EXIT_SUCCESS);
        }
        if (cmd_node->next)
        {
            close(fd[1]);
            current_cmd->out = fd[1];
            t_exec *next_cmd = (t_exec *)cmd_node->next->content;
            next_cmd->in = fd[0];
        }
        cmd_node = cmd_node->next;
    }
    while (wait(NULL) > 0);
}


void handle_redirect(t_list *cmd_node, int pipes[2])
{
    t_exec *node;

    node = cmd_node->content;
    if (node->in != STDIN_FILENO)
    {
        if (dup2(node->in, STDIN_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
        close(node->in);
    }
    if (node->out != STDOUT_FILENO)
    {
        if (dup2(node->out, STDOUT_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
        close(node->out);
    }
    else if (cmd_node->next)
    {
        if (dup2(pipes[PIPE_WRITE], STDOUT_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
    }
    if (cmd_node->next)
        close(pipes[PIPE_READ]);
}


/*
void handle_redirect(t_list *cmd_node, int pipes[2])
{
    t_exec *node;

    node = cmd_node->content;
    if (node->in != STDIN_FILENO)
    {
        if (dup2(node->in, STDIN_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
        close(node->in);
    }
    if (node->out != STDOUT_FILENO)
    {
        if (dup2(node->out, STDOUT_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
        close(node->out);
    }
    else if (cmd_node->next)
    {
        if (dup2(pipes[PIPE_WRITE], STDOUT_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
    }
    if (cmd_node->next)
        close(pipes[PIPE_READ]);
}


void	handle_redirect(t_list *cmd_node, int pipes[2])
{
    t_exec *node;
    
    node = cmd_node->content;
    if (node->in != STDIN_FILENO)
    {
        if (dup2(node->in, STDIN_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
        close(node->in);
    }
    if (node->out != STDOUT_FILENO)
    {
        if (dup2(node->out, STDOUT_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
        close(node->out);
    }
    else if (cmd_node->next)
    {
        if (dup2(pipes[PIPE_WRITE], STDOUT_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
    }
    close(pipes[PIPE_WRITE]);
}

void	handle_process(t_shell *shell, t_list *cmd, int fd[2])
{
 //  t_exec *node;
   // int cmd_len;
    
   // cmd_len = 0;
   // node = cmd->content;
	//if (node->args)
	//	cmd_len = ft_strlen(*node->args);
	handle_redirect(cmd, fd);
	close(fd[PIPE_READ]);
	//builtin(shell,cmd,&g_env.exit_status,cmd_len);
	ft_lstclear(&shell->cmds, ft_free);
	exit(g_env.exit_status);
}

int check_fork(t_shell *shell, t_list *cmd, int fd[2])
{
	pid_t pid = fork();

	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		m_error(ERR_FORK, NULL, 1);
        return (0);
	}
	else if (pid == 0)
    {
		handle_process(shell, cmd, fd);
        exit(g_env.exit_status);
    }
    else
    return (1);
}

void *prepare_exec(t_shell *shell, t_list *cmd, int fd[2])
{
	t_exec *cmd_info;
	DIR *dir;

    cmd_info = cmd->content;
    dir = NULL;
	if (cmd_info->args)
		dir = opendir(*cmd_info->args);
	if (cmd_info->in == -1 || cmd_info->out == -1)
	{
		if (dir)
			closedir(dir);
		return (NULL);
	}
	if ((cmd_info->args && access(cmd_info->path, X_OK) == 0) || built_check(cmd_info))
		check_fork(shell, cmd, fd);
	else if (cmd_info->path && (!access(cmd_info->path, F_OK) || dir))
		g_env.exit_status = 126;
	else if (cmd_info->args)
		g_env.exit_status = 127;
	if (dir)
		closedir(dir);
	return (NULL);
}





static void handle_redirect(t_list *cmd_node, int pipes[2])
{
    t_exec *node = cmd_node->content;

    // Redirect input
    printf("handle redirect\n");
    if (node->in != STDIN_FILENO)
    {
        if (dup2(node->in, STDIN_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
        close(node->in);
    }

    // Redirect output
    if (node->out != STDOUT_FILENO)
    {
        if (dup2(node->out, STDOUT_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
        close(node->out);
    }
    else if (cmd_node->next) // If there is a next command, redirect to the pipe
    {
        if (dup2(pipes[PIPE_WRITE], STDOUT_FILENO) == -1)
        {
            m_error(ERR_DUP, NULL, 1);
            return;
        }
    }
}

void handle_process(t_shell *shell, t_list *cmd, int fd[2])
{
    handle_redirect(cmd, fd);
    close(fd[PIPE_READ]); // Close read end in the child process
    // Execute built-in commands directly if needed, for example:
    // if (is_builtin(cmd))
    //     execute_builtin(shell, cmd);
    // else
    //     execve(node->path, node->args, shell->keys);
    
    ft_lstclear(&shell->cmds, ft_free); // Clear command list
    exit(g_env.exit_status); // Exit with the exit status
}

int check_fork(t_shell *shell, t_list *cmd, int fd[2])
{
    pid_t pid = fork();

    if (pid < 0)
    {
        close(fd[0]);
        close(fd[1]);
        m_error(ERR_FORK, NULL, 1);
        return 0; // Indicate an error
    }
    else if (pid == 0) // Child process
        handle_process(shell, cmd, fd); // Redirect and handle the comman
    // Parent process continues
    return 1; // Indicate success
}
*/