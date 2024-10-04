/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 19:48:13 by yasmine           #+#    #+#             */
/*   Updated: 2024/10/02 19:18:29 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <signal.h>
# include <sys/errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
//# include "./src/exec/execute.h"

# define GREEN      "\033[0;32m"
# define RED        "\033[0;31m"
# define YELLOW	    "\033[0;33m"
# define MAGENTA	"\033[0;35m"
# define CYAN       "\033[0;36m"
# define RST_COLOR  "\033[0m"

# define STDIN  0
# define STDOUT 1
# define STDERR 2

# define MAX_PATH 4096

# define CMD 3
# define ARG 4      
# define REDIR_OUT_SUBS 5//Output redirection, e.g. '>' | subscribe
# define REDIR_OUT_ATTACH 6//Output redirection, e.g. '>>' | attach
# define REDIR_IN 7//Input redirection, e.g. '<'
# define REDIR_DOC 8//Redirection with special handling that does not apply to simple redirection
# define FILE 9//File or directory name (ex.: 'file.txt')
# define ERROR 10//Error in the sintaxe
# define PROTECTED_DOLLAR 11//protection of strings of environment variables or file names(ex.: $HOME)
# define PROTECTED_QUOTE 12// protection of strings or arguments that are enclosed in quotes(ex.:"Hello World")
# define SKIPED_COMMAND	13//Skip current token and next token(ex.: '&&' ou '||')
# define HERE_DOC 14//Input redirection, ex.: '<<'
# define HERE_DOC_END 15//ffinal of heredoc

/*
* ' ' Everyhing inside single quotes is treated as literal text, i.e. no special characters are interpreted.
* e.g. echo 'Hello, $USER', prints 'Hello, $USER'
* " " Allow a string to contain spaces and preserve most special characters except $, `, and \.
* e.g. echo "Hello, $USER", prints 'Hello, yasmine'
*/
typedef struct s_quote
{
	bool				single_quote;
	bool				double_quote;
}						t_quote;

typedef struct s_env
{
	char				**env;
	char				**sorted;
	int					exit_status;
}						t_env;

typedef struct s_tokens
{
	int					nbr;
	int					pipe;
	int					*type;//categorizar o token (ex.:CMD, ARG, ...)
	char				**tokens;
}						t_tokens;

typedef struct s_separator
{
	int					*separator_index;
	int					nbr_separator;
	char				**content;
}						t_separator;


extern t_env					g_env;//global variable to store the environment variables and the exit status of the last command executed.

/*
										***** TO DO ******
example of the t_execution structure:
command: echo "hello" > output.txt

nb_pipelines: 1 (since there are no pipelines)
args_type: (indicating that echo is a CMD and "hello" is an ARG)
files_type: (indicating that there is a redirection >)
file_dscp: [3] (indicating the file descriptor for output.txt)
args: ["echo", "hello"]
files: ["output.txt"]
exec_path: "/bin/echo"
command: "echo"

typedef struct s_execution
{
	int					nbr_pipelines;
	int					*args_type;//(ex.:CMD, ARG, ...)
	int					*files_type;//(ex.:REDIR_OUT_SUBS, REDIR_OUT_ATTACH, ...)
	int					*file_dscp;//file descriptor
	char				**args;//(ex.:'ls -l /home', the array will be ["ls", "-l", "/home"].)
	char				**files;//(ex.: 'ls -l > file.txt', o array files conteria ["file.txt"].)
	char				*command;
}						t_execution;
*/

typedef struct s_shell
{
	t_list	*cmds; // linked list of commands
	char	**keys; // enviroment and keys
	//int nbr_keys;
	pid_t	mpid; // main process id
}		t_shell;
typedef struct s_data
{
	int					oldlen;
	int					newlen;
	int					presence;
	t_shell 			*shell;
}						t_data;

//UTILS
int			absolute_value(int nb);
int     	count(char **str);
int			max_of(int i, int j);
int			ft_error(char *description, int exit_code);
void    	free_str_array(char **arr);
void    	error_file(char *file);
int			error_redirection(int file_type);
void		error_no_such_file(char *command);
void    	error_command(char *command);
char    	*ft_stringjoin(char *str, char c);
char		*extract_substring(char const *s, unsigned int start, size_t len);
void		join_backslash(char **path);
char		*format_var(char *var, char *value);
void 		new_var(char *var, char *value, t_shell *shell);
void		update_env(char *var, char *value, t_shell *shell);
void 		append_to_env(char *variable, char *value, int size, t_shell *shell);
char		*set_dollar(char *str, int i, t_shell *shell);
int     	len_dollar(char *str, int i);
char		**dup_array(char **env);
void		sort_array(char **sorted);
void 		env_init(char **env, t_shell *shell);
int     	count_backslash(char *line, int i);
int			valid_backslash(char **tokens);
void 		free_tokens(t_tokens *token);
void		free_dollar(char **original, char **replacement);
void		free_split(char **split);
int			syntax_redirections(t_tokens *tokens);
int			syntax_grammar(char **commands, t_tokens *tokens);
int			count_separator(char *line, char separator);
int			str_isdigit(char *str);
t_quote		quote_value(char type_quote, t_quote status_quote);
t_separator position_separator(char *line, char separator);
char 		**separator_split(char *line, int *separatorI, int nbr_separator);
char    	**token_split(char *line, int nbr);
int 		get_token_type(char *content, int len);
void 		set_token_aux(int i, int *cmd, int *type);
void 		set_tokens_type(int *type, char **content);
int 		*set_token(t_tokens token);
int 		skip_space(char *line, int i);
int			get_end(char *line, int i);
int 		count_token(char *line);
int 		is_empty(char *str);
int 		is_protected(int type);
int 		is_cmd(int type);
int 		is_arg(int type);
int 		is_redirection(int type);
void 		env_update(char *env, char *new_value, t_shell *shell);
void 		replace_value(int i, char *new_value, int len, char **arr);
char		*assemble_argument(char **args, int *token_class, int i);
int 		get_next_positive(char **args, int *token_class, int i);
char 		**join_args(char **args, int *token_class);
char 		**arg_add(char *arg, char **arr);
void		print_invalid_identifier_error(char **args);
int			is_invalid_identifier(char *arg);
char		**update_or_add_env_var(char *variable, char *value, t_shell *shell);

//PARSING
t_quote		quote_init(void);
char		*find_env_value(char *str, int i, char **sorted);
void    	process_pipeline(char *line, t_shell *shell);
void    	process_command_line(char *line, t_shell *shell);
void 		dollar_replace(char **token, int i, t_shell *shell);
char    	**dollar_spaces_split(char **old, int i);
void    	arg_type(t_tokens *token, int oldsize, int newsize, int i);
int			dollar_aux_config(t_tokens *token, int *i, t_data *data);
char 		*dollar_config(char *str, int pos, t_shell *shell);
int 		dollar_presence(char *str);
char 		*process_quotes(char *str, t_shell *shell);
char		*quotes_expand(char *content, int i, t_shell *shell);
t_tokens	process_quotes_tokens(t_tokens tokens, t_shell *shell);
void		exec_process_quotes(t_tokens *tokens, t_shell *shell);
void		type_of_separator(int *type_of, char **token);
void		filler_stokens(char **cmds, t_tokens **token, int nbr, t_shell *shell);
char **split_pipes(char *line);
void		tokenize_commands(char **cmds,t_list **command_list, t_shell *shell);
int			token_dollar_end(char *str, int i);
int			token_word_end(char *str, int i);
int			token_quotes_end(char *str, int i);
int			token_redir_end(char *str, int i);
char		*parse_next_token(char *line, int reset);

#endif
