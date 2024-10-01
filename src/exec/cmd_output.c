/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_output.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 20:24:02 by emencova          #+#    #+#             */
/*   Updated: 2024/10/01 13:20:25 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"


static void	store_cmd_output(char ***output, int fd)
{
	char	**new;
	char	*trimmed;
	char	*line;

	new = NULL;
	line = get_next_line(fd);
    if (!line)
			return ;
	while (line)
	{
		trimmed = ft_strtrim(line, "\n");
		free(line);
		new = extend_form(new, trimmed);
		free(trimmed);
        line = get_next_line(fd);
	}
	free_form(output);
	*output = new;
}

void run_capture_output(char ***output, char *command_path, char *command_args, char **env)
{
    pid_t pid;
    int pipe_fd[2];
    char **args_array;
    int status;

    pipe(pipe_fd);
    pid = fork();
    if (pid == 0)
    {
        close(pipe_fd[PIPE_READ]);
        args_array = ft_split(command_args, ' ');
        dup2(pipe_fd[PIPE_WRITE], STDOUT_FILENO);
        close(pipe_fd[PIPE_WRITE]);
        if (access(command_path, F_OK) == 0)
            execve(command_path, args_array, env);
        exit(127);
    }
    close(pipe_fd[PIPE_WRITE]);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        g_env.exit_status = WEXITSTATUS(status);
    store_cmd_output(output, pipe_fd[PIPE_READ]);
    close(pipe_fd[PIPE_READ]);
}