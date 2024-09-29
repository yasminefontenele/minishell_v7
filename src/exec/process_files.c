/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:25:13 by emencova          #+#    #+#             */
/*   Updated: 2024/09/29 20:41:49 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

char *read_here_document(char *input_buffer[2], size_t limit_length, char *end_marker, char *error_message)
{
    char *temp;

    while (g_env.exit_status != 130 && (input_buffer[0] == NULL
     || ft_strncmp(input_buffer[0], end_marker, limit_length) != 0 
     || ft_strlen(end_marker) != limit_length))
    {
        temp = input_buffer[1];
        input_buffer[1] = ft_strjoin(input_buffer[1], input_buffer[0]);
        free(temp);
        free(input_buffer[0]);
        input_buffer[0] = readline("> ");
        if (input_buffer[0] == NULL)
        {
            printf("%s (wanted `%s`)\n", error_message, end_marker);
            break;
        }
        temp = input_buffer[0];
        input_buffer[0] = ft_strjoin(input_buffer[0], "\n");
        free(temp);
        limit_length = ft_strlen(input_buffer[0]) - 1;
    }
    free(input_buffer[0]);
    return (input_buffer[1]);
}

int create_here_document_fd(char *input_buffer[2], char *delimiter[2])
{
    int fd[2];

    g_env.exit_status = 0;
    if (pipe(fd) == -1)
    {
        m_perror(PIPE_READ, NULL, 1);
        return (-1);
    }
    input_buffer[1] = read_here_document(input_buffer, 0, delimiter[0], delimiter[1]);
    write(fd[PIPE_WRITE], input_buffer[1], ft_strlen(input_buffer[1]));
    free(input_buffer[1]);
    close(fd[PIPE_WRITE]);

    if (g_env.exit_status == 130)
    {
        close(fd[PIPE_READ]);
        return (-1);
    }
    return (fd[PIPE_READ]);
}

/*
void parse_redir(t_exec *exec, char **args)
{
    int i;

    i = 0;
    while (args[i])
    {
        if (ft_strcmp(args[i], ">") == 0)
        {
            exec->out = open_fd(exec->out, args[i + 1], 1, 0);
            i++;
        }
        else if (ft_strcmp(args[i], ">>") == 0)
        {
            exec->out = open_fd(exec->out, args[i + 1], 1, 1);
            i++;
        }
        else if (ft_strcmp(args[i], "<") == 0)
        {
            exec = infile_one(exec, args, &i);
        }
        else if (ft_strcmp(args[i], "<<") == 0)
        {
            exec = infile_two(exec, args, &i);
        }
        i++;
    }
}
*/
void parse_redir(t_exec *exec, char **args)
{
    int i = 0;
    printf("entering parse redir \n");

    while (args[i])
    {
        if (ft_strcmp(args[i], ">") == 0)
        {
            printf("Redirection found: %s\n", args[i + 1]);
            exec->out = open_fd(exec->out, args[i + 1], 1, 0); // Overwrite
            printf("out_fd set to: %d\n", exec->out);
            if (exec->out == -1)
                perror("Error opening output file");
            i++;
        }
        else if (ft_strcmp(args[i], ">>") == 0)
        {
            exec->out = open_fd(exec->out, args[i + 1], 1, 1); // Append
            if (exec->out == -1)
                perror("Error opening output file");
            i++;
        }
        else if (ft_strcmp(args[i], "<") == 0)
        {
            exec = infile_one(exec, args, &i); // Handle input redirection
        }
        else if (ft_strcmp(args[i], "<<") == 0)
        {
            exec = infile_two(exec, args, &i); // Handle here-doc
        }
        i++;
    }
}

int ft_str_is_space(char *line)
{
    while (*line)
    {
        if (!isspace(*line))
            return 0;
        line++;
    }
    return 1;
}

/*
void init_exec(t_exec *exec, t_list *cmd_list)
{	 
	exec = (t_exec *)cmd_list->content;
	exec->path = NULL;
    exec->in = 0;
    exec->out = 1;
    if (!exec->args[0] || ft_strlen(exec->args[0]) == 0 || ft_str_is_space(exec->args[0]))
       return;
	//else
    //    write(STDERR_FILENO, "Error: Command not found.\n", 26);
}*/

