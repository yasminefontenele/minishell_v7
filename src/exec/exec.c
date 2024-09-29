/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 22:31:57 by emencova          #+#    #+#             */
/*   Updated: 2024/09/29 20:48:25 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <dirent.h>

int pipe_builtin(t_shell *shell, t_list *cmd_ls, int *exit, int len)
{
    char **args;
    int builtin_result;   
    while (cmd_ls)
    {
        args = ((t_exec *)cmd_ls->content)->args;
        len = ft_strlen(*args);
        if (args && !ft_strncmp(args[0], "exit", 4) && len == 4)
            return(g_env.exit_status = m_exit(shell, cmd_ls, exit));
        else if (*args && !ft_strncmp(args[0], "cd", 2) && len == 2)
            return(g_env.exit_status = m_cd(shell));
        else if (!cmd_ls->next && args && !ft_strncmp(args[0], "export", 6))
              return(g_env.exit_status = m_export(shell));
        else if (!cmd_ls->next && args && (builtin_result = handle_basic_builtins(shell,args)) != -1)
            return(g_env.exit_status = builtin_result);
        cmd_ls = cmd_ls->next;
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN); 
    }        
    return g_env.exit_status;
}

void execute_pipeline(t_shell *shell, t_list *commands_list)
{
    int fd[2];
    int prev_fd;
    t_list *cmd_node;
   // t_exec *exec;
    
    cmd_node = commands_list;
    prev_fd = -1;
    while (cmd_node)
    {
       // exec = (t_exec *)cmd_node->content;
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
          //  parse_redir(exec, exec->args);
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(prev_fd);
            }
            if (cmd_node->next)
            {
                if (dup2(fd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(fd[1]);
            }
            close(fd[0]);
            command_get_pipeline(shell, cmd_node);
            exit(EXIT_SUCCESS);
        }
        else
        {
            if (prev_fd != -1)
                close(prev_fd);
            if (cmd_node->next)
            {
                close(fd[1]);
                prev_fd = fd[0];
            }
        }
        cmd_node = cmd_node->next;
    }
    while (wait(NULL) > 0);
}