/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:25:13 by emencova          #+#    #+#             */
/*   Updated: 2024/10/06 13:03:09 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/*
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



// WORKS BEFORE LAST ONE

int parse_redir(t_exec *exec, char **args)
{
    int i = 0;
    
    while (args[i])
    {
        if (ft_strcmp(args[i], ">") == 0)
        {
            exec = outfile_one(exec, args, &i);
            if (i == -1) // If open failed or file not specified
                return (0);
        }
        else if (ft_strcmp(args[i], ">>") == 0)
        {
            exec = outfile_two(exec, args, &i);
            if (i == -1)
                return (0);
        }
        else if (ft_strcmp(args[i], "<") == 0)
        {
            exec = infile_one(exec, args, &i);
            if (i == -1)
                return (0);
        }
        else if (ft_strcmp(args[i], "<<") == 0)
        {
            exec = infile_two(exec, args, &i);
            if (i == -1)
                return (0);
        }
        i++;
    }
    return (1);
}

*/


// Function to read the here-document
char *read_here_document(char *input_buffer[2], size_t limit_length, char *end_marker, char *error_message)
{
    char *temp;
    size_t line_count = 0; // To count the number of lines

    while (g_env.exit_status != 130 && (input_buffer[0] == NULL
        || ft_strncmp(input_buffer[0], end_marker, limit_length) != 0 
        || ft_strlen(end_marker) != limit_length))
    {
        // Read input from the user
        input_buffer[0] = readline("> ");
        
        // Handle EOF case
        if (input_buffer[0] == NULL)
        {
            printf("%s (wanted `%s`)\n", error_message, end_marker);
            break;
        }

        // Count the line and append to the input buffer
        line_count++;
        temp = input_buffer[1];
        input_buffer[1] = ft_strjoin(input_buffer[1], input_buffer[0]);
        free(temp);
        temp = ft_strjoin(input_buffer[1], "\n");
        free(input_buffer[1]);
        input_buffer[1] = temp;

        // Free input_buffer[0]
        free(input_buffer[0]);
        input_buffer[0] = NULL; // Reset for next input
    }

    // Print the number of lines read
    if (line_count > 0)
    {
        printf("Here-document contains %zu lines.\n", line_count);
    }
    
    return input_buffer[1];
}

// Function to create a pipe for the here-document
int create_here_document_fd(char *input_buffer[2], char *delimiter[2])
{
    int fd[2];

    g_env.exit_status = 0;

    // Create a pipe
    if (pipe(fd) == -1)
    {
        m_perror(PIPE_READ, NULL, 1);
        return (-1);
    }

    // Read here-document into input_buffer[1]
    input_buffer[1] = read_here_document(input_buffer, 0, delimiter[0], delimiter[1]);

    // Check for empty input
    if (input_buffer[1] == NULL || ft_strlen(input_buffer[1]) == 0)
    {
        close(fd[PIPE_READ]);
        close(fd[PIPE_WRITE]);
        return (-1);
    }

    // Write the content of the here-document to the pipe
    write(fd[PIPE_WRITE], input_buffer[1], ft_strlen(input_buffer[1]));
    free(input_buffer[1]);
    close(fd[PIPE_WRITE]); // Close write end

    // Check for exit status indicating cancellation (Ctrl+C)
    if (g_env.exit_status == 130)
    {
        close(fd[PIPE_READ]);
        return (-1);
    }

    return fd[PIPE_READ]; // Return the read end of the pipe
}

int parse_redir(t_exec *exec, char **args)
{
    int i;

    i = 0;
    while (args[i])
    {
        if (ft_strcmp(args[i], ">") == 0)
        {
            if (!args[i + 1])
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                exec->out = -1;
                return (0);
            }
            exec->out = open_fd(exec->out, args[i + 1], 1, 0);
            if (exec->out == -1)
            {
                perror("Error opening output file");
                return (0);
            }
            args[i] = NULL;
            args[i + 1] = NULL;
            return (1);
        }
        else if (ft_strcmp(args[i], ">>") == 0)
        {
            if (!args[i + 1])
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                exec->out = -1;
                return (0);
            }
            exec->out = open_fd(exec->out, args[i + 1], 1, 1);
            if (exec->out == -1)
            {
                perror("Error opening output file");
                return (0);
            }
            args[i] = NULL;
            args[i + 1] = NULL;
            return (1);
        }
        else if (ft_strcmp(args[i], "<") == 0)
        {
            if (!args[i + 1])
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                exec->in = -1;
                return (0);
            }
            exec = infile_one(exec, args, &i);
            if (exec->in == -1)
                return (0);
            return (1);
        }
        else if (ft_strcmp(args[i], "<<") == 0)
        {
            if (!args[i + 1])
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                exec->in = -1;
                return (0);
            }
            printf("inside parse redir ft_strcmp loop\n");
            printf("before infile two node in is %d\n", exec->in);
            infile_two(exec, args, &i); // Call infile_two without assigning its return value
            printf("after infile two node in is %d\n", exec->in);
            if (exec->in == -1)
                return (0);
            return (2); // Indicate a successful here-document processing
        }
        i++;
    }
    return (0);
}



/*
//// LAST THAT WORKS
int parse_redir(t_exec *exec, char **args)
{
    int i;

    i = 0;
    while (args[i])
    {
        if (ft_strcmp(args[i], ">") == 0)
        {
            if (!args[i + 1])
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                exec->out = -1;
                return (0);
            }
            exec->out = open_fd(exec->out, args[i + 1], 1, 0);
            if (exec->out == -1)
            {
                perror("Error opening output file");
                return (0);
            }
            args[i] = NULL;
            args[i + 1] = NULL;
            return (1);
        }
        else if (ft_strcmp(args[i], ">>") == 0)
        {
            if (!args[i + 1])
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                exec->out = -1;
                return (0);
            }
            exec->out = open_fd(exec->out, args[i + 1], 1, 1);
            if (exec->out == -1)
            {
                perror("Error opening output file");
                return (0);
            }
            args[i] = NULL;
            args[i + 1] = NULL;
            return (1);
        }
        else if (ft_strcmp(args[i], "<") == 0)
        {
            if (!args[i + 1])
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                exec->in = -1;
                return (0);
            }
            exec = infile_one(exec, args, &i);
            if (exec->in == -1)
                return (0);
            return (1);
        }
        else if (ft_strcmp(args[i], "<<") == 0)
        {
            printf("inside parse redir ft_strcmp loop\n");
            if (!args[i + 1])
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                exec->in = -1;
                return (0);
            }
            printf("before infile two node in is %d\n", exec->in);
            exec = infile_two(exec, args, &i);
            printf("after infile two node in is %d\n", exec->in);
            if (exec->in == -1)
                return (0);
            return (1);
        }
        i++;
    }
    return (0);
}
*/

int ft_str_is_space(char *line)
{
    while (*line)
    {
        if (!ft_isspace(*line))
            return 0;
        line++;
    }
    return 1;
}



