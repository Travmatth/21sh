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

# include "../libftprintf/srcs/includes/ft_printf.h"
# include <unistd.h>
# include <signal.h>
# include <sys/stat.h>
# include "grammar.h"
# include <fcntl.h>

# define IS_WS(x) (x == ' ' || x == '\t' || x == '\n')
# define IS_QTE(x) (x == '\'' || x== '"')
# define IS_SEP(x) (IS_WS(x) || IS_QTE(x))
# define ESCAPABLE_CHAR(x) ()
# define END 1
# define BEGIN 0

char			**g_environ;
int				g_processes;
typedef int		(*t_builtinf)(int argc, char **argv);

typedef struct	s_handler
{
	char		*lhs;
	char		**rhs;
	char		**lookahed;
}				t_handler;

t_handler		**g_handlers;

typedef struct	s_ast
{
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

void			init_parser(void) __attribute__((constructor));
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

int				lexical_analysis(char *complete_cmd, t_list **tokens, t_list **missing);

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
