/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 00:38:20 by yfontene          #+#    #+#             */
/*   Updated: 2024/10/01 13:19:59 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
//
static void	outpt_update(char ***form, int fd)
{
	char	**add;
	char	*tmp;
	char	*line;

	add = NULL;
	line = NULL;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		tmp = ft_strtrim(line, "\n");
		free(line);
		add = extend_form(add, tmp);
		free(tmp);
	}
	free_form(form);
	*form = add;
}

void	ft_exec(char ***out, char *full, char *ags, char **env)
{
	pid_t	pid;
	int		fd[2];
	char	**form;
	int status;

	pipe(fd);
	pid = fork();
	if (!pid)
	{
		close(fd[PIPE_READ]);
		form = ft_split(ags, ' ');
		for (int i = 0; form[i]; i++)
		dup2(fd[PIPE_WRITE], STDOUT_FILENO);
		close(fd[PIPE_WRITE]);
		if (!access(full, F_OK))
			execve(full, form, env);
		exit (127);
	}
	close(fd[PIPE_WRITE]);
	waitpid(pid, NULL, 0);
	if (WIFEXITED(status))
        g_env.exit_status = WEXITSTATUS(status);
	outpt_update(out, fd[PIPE_READ]);
	close(fd[PIPE_READ]);
}