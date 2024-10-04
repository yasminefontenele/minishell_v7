/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:23:23 by emencova          #+#    #+#             */
/*   Updated: 2024/10/04 21:41:38 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/*
int open_fd(int fd, char *path, int is_output, int append)
{
    if (fd > 2)
        close(fd);
    if (!path)
        return (-1);
    if (access(path, F_OK) == -1 && !is_output)
        m_error(ERR_NEWDIR, path, 127);
    else if (!is_output && access(path, R_OK) == -1)
        m_error(ERR_NWPERM, path, 126);
    else if (is_output && access(path, W_OK) == -1 && access(path, F_OK) == 0)
        m_error(ERR_NWPERM, path, 126);
    if (is_output)
	{
        if (append)
            return (open(path, O_CREAT | O_WRONLY | O_APPEND, 0666));
        else
            return (open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666));
	}
    else
        return (open(path, O_RDONLY));
}


int open_fd(int fd, char *path, int is_output, int append)
{
    // Close the previous file descriptor if it's greater than 2
    if (fd > 2)
        close(fd);

    // Check if the path is valid
    if (!path)
        return (-1);

    // Debugging output for the path being checked
    printf("Checking path: %s\n", path);

    // Access checks for read/write permissions
    if (access(path, F_OK) == -1 && !is_output) {
        printf("Error: File does not exist: %s\n", path);
        m_error(ERR_NEWDIR, path, 127);
    } 
    else if (!is_output && access(path, R_OK) == -1) {
        printf("Error: No read permission for: %s\n", path);
        m_error(ERR_NWPERM, path, 126);
    } 
    else if (is_output && access(path, W_OK) == -1 && access(path, F_OK) == 0) {
        printf("Error: No write permission for: %s\n", path);
        m_error(ERR_NWPERM, path, 126);
    }

    // Open the file based on whether it's input or output
    if (is_output) {
        if (append) {
            printf("Opening output file (append mode): %s\n", path);
            return (open(path, O_CREAT | O_WRONLY | O_APPEND, 0666));
        } else {
            printf("Opening output file (truncate mode): %s\n", path);
            return (open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666));
        }
    } else {
        printf("Opening input file: %s\n", path);
        return (open(path, O_RDONLY));
    }
}



///THIS ONE WORKED LAST TIME!!!!////

int open_fd(int fd, char *path, int is_output, int append)
{
    if (fd > 2)
        close(fd);
    if (!path)
    {
        fprintf(stderr, "Error: Invalid path\n");
        return (-1);
    }

    t_exec *exec = malloc(sizeof(t_exec));
    exec->out = STDOUT_FILENO;

   // printf("Opening file: %s, is_output: %d, append: %d\n", path, is_output, append);
    
    if (access(path, F_OK) == -1 && !is_output)
    {
        m_error(ERR_NEWDIR, path, 126);
        return (-1);
    }
    else if (!is_output && access(path, R_OK) == -1)
    {
        m_error(ERR_NWPERM, path, 126);
        return(-1);
    }
    else if (is_output && access(path, W_OK) == -1 && access(path, F_OK) == 0)
    {
        m_error(ERR_NWPERM, path, 126);
        return(-1);
    }   
    if (is_output)
    {
        if (append)
            return (fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644));
        else
            return (fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644));
    }
    else
        return (fd=open(path, O_RDONLY));
    if (fd == -1)
        perror("Error opening file");

    return fd;
}
*/

int open_fd(int fd, char *path, int is_output, int append)
{
    if (fd > 2)
        close(fd);
    
    if (!path)
    {
        fprintf(stderr, "Error: Invalid path\n");
        return (-1);
    }
    if (!is_output && access(path, F_OK) == -1)
    {
        m_error(ERR_NEWDIR, path, 126);
        return (-1);
    }
    if (!is_output && access(path, R_OK) == -1)
    {
        m_error(ERR_NWPERM, path, 126);
        return (-1);
    }
    if (is_output && access(path, W_OK) == -1 && access(path, F_OK) == 0)
    {
        m_error(ERR_NWPERM, path, 126);
        return (-1);
    }
    if (is_output)
    {
        if (append)
            fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    else
        fd = open(path, O_RDONLY);
    
    if (fd == -1)
        perror("Error opening file");
    
    return (fd);
}

t_exec *outfile_one(t_exec *node, char **ags, int *len)
{
    char *error_msg;
	
	error_msg = "syntax error";
    (*len)++;
   // printf("Entered outfile_one\n");
    if (ags[*len])
	{
        node->out = open_fd(node->out, ags[*len], 1, 0);
        if (node->out == -1)
		{
            ft_putendl_fd(error_msg, 2);
            g_env.exit_status = 1;
            *len = -1;
            return (node);
        }
    } 
	else
	{
        ft_putendl_fd(error_msg, 2);
        g_env.exit_status = 2;
        *len = -1;
    }
	 return (node);
}

t_exec *outfile_two(t_exec *node, char **ags, int *len) 
{
    char *new_line;
	
	new_line = "syntax error";
    (*len)++;
    if (ags[*len])
        node->out = open_fd(node->out, ags[*len], 1, 1);
    
    if (!ags[*len] || node->out == -1)
	{
        *len = -1;
        ft_putendl_fd(new_line, 2);
        g_env.exit_status = 2;
        *len = -1;
    }
    return (node);
}

t_exec *infile_one(t_exec *node, char **ags, int *len)
{
  //  char *new_line;
	
//	new_line = "syntax error";
    (*len)++;
    if (ags[*len])
        node->in = open_fd(node->in, ags[*len], 0, 0);

    if (!ags[*len] || node->in == -1)
	{
        *len = -1;
      //  ft_putendl_fd(new_line, 2);
		g_env.exit_status = 1;
        *len = -1;	
    }
    return (node);
}

t_exec	*infile_two(t_exec *node, char **ags, int *len)
{
	char	*input[2];
	char	*new_line;
	char	*deli[2];

	input[0] = NULL;
	input[1] = NULL;
	deli[0] = NULL;
	deli[1] = "warning: here-document delimited by end-of-file";
	new_line = "syntax error near unexpected token `newline'";
	(*len)++;
	if (ags[++(*len)])
	{
		deli[0] = ags[*len];
		node->in = create_here_document_fd(input, deli);
	}
	if (!ags[*len] || node->in == -1)
	{
		*len = -1;
		if (node->in != -1)
		{
			ft_putendl_fd(new_line, 2);
			g_env.exit_status = 2;
		}
	}
	return (node);
}


