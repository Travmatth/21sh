/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/18 15:06:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/29 18:04:09 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <sys/stat.h>
# include <sys/ioctl.h>
# include <unistd.h>
# include <termcap.h>
# include <fcntl.h>
# include <errno.h>
# include <termios.h>
# include <signal.h>

# include "../libftprintf/srcs/includes/ft_printf.h"
# include "grammar.h"
# include "traverse.h"

# define IS_WS(x) (x == ' ' || x == '\t' || x == '\n')
# define IS_QTE(x) (x == '\'' || x== '"')
# define IS_SEP(x) (IS_WS(x) || IS_QTE(x))
# define ESCAPABLE_CHAR(x) ()
# define END 1
# define BEGIN 0

char			**g_environ;
int				g_processes;
typedef int		(*t_builtinf)(int argc, char **argv);

t_prod			*g_prods;

typedef struct	s_ast
{
	t_ast_node	*root;
	int			type;
	t_list		*missing;
}				t_ast;

typedef struct	s_builtin
{
	char		*cmd;
	t_builtinf	f;
	int			len;
}				t_builtin;

/*
** src/main.c
*/

/*
** src/init.c
*/

void			init_environ(int argc, char **argv, char **environ);
void			init_parser(void);
int				init_shell(void);

/*
** signal.c
*/

void			child_sig_handler(int sig);
void			sig_handler(int sig);

/*
** builtins/
*/

void			set_env_var(char *name, char *val);
int				builtin_echo(int argc, char **argv);
int				builtin_cd(int argc, char **argv);
int				builtin_setenv(int argc, char **argv);
int				builtin_unsetenv(int argc, char **argv);
int				builtin_env(int argc, char **argv);
char			*get_env_var(char *var);

/*
** lexer/lexer.c
*/

int				lexical_analysis(char *complete_cmd
								, t_list **tokens
								, t_list **missing);

/*
** parse/parse.c
*/

int				parse_commands(t_list *tokens, t_ast *ast);
int				prepare_ast(char *complete_cmd, t_ast *ast);

/*
** parse/command.c
*/

int				execute_commands(char *command);
int				builtin_command(char **command);

/*
** traverse/traverse.c
*/

int				traverse_ast(t_ast *ast);
#endif
