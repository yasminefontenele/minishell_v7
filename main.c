/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 17:01:48 by emencova          #+#    #+#             */
/*   Updated: 2024/09/29 19:56:14 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./src/exec/execute.h"

t_env   g_env;

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_env.exit_status = 130;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

int handle_tab()
{
    return 0;
}

void cleanup_shell(t_shell *shell)
{
    if (shell->cmds)
    {
        ft_lstclear(&(shell->cmds), ft_free);
        shell->cmds = NULL;
    }
    if (shell->keys)
    {
        int i = 0;
        while (shell->keys[i])
        {
            free(shell->keys[i]);
            i++;
        }
        free(shell->keys);
        shell->keys = NULL;
    }
}

int main(int ac, char **av, char **envp)
{
    t_shell shell;
    char    *line;
    t_list  *command_list;

	shell.cmds = NULL;
	shell.mpid = getpid();
    line = NULL;
	command_list = NULL;
	env_init(envp, &shell);  
    rl_bind_key('\t', handle_tab);
    while (ac && av)
    {
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT,SIG_IGN);
		line = readline("minishell🔥$ ");
        if (!line)
        {
            printf("exit\n");
            break;
        }
        if (*line)
        {
            add_history(line);
            tokenize_commands(&line, &command_list, &shell);
        }  
        shell.cmds = command_list;
		if (command_list)
			cmd_execute(&shell, command_list);
		ft_lstclear(&command_list, free);
		command_list = NULL;
        free(line);
    }
    cleanup_shell(&shell);
    return (g_env.exit_status);
}
