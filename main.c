/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 17:01:48 by emencova          #+#    #+#             */
/*   Updated: 2024/09/29 15:04:48 by eliskam          ###   ########.fr       */
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

int main(int ac, char **av, char **envp)
{
    t_shell shell;
    char    *line;
    t_list  *command_list;

    (void)ac;
	(void)av;
	shell.cmds = NULL;
	shell.mpid = getpid();
    line = NULL;
	command_list = NULL;
	env_init(envp, &shell);  
    rl_bind_key('\t', handle_tab);
    while (1)
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
			process_command(&shell, command_list);
		ft_lstclear(&command_list, free);
		command_list = NULL;
        free(line);
    }
    return (g_env.exit_status);
}
