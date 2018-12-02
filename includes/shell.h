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

# define IS_WS(x) (x == ' ' || x == '\t' || x == '\n')
# define IS_QTE(x) (x == '\'' || x== '"')
# define IS_SEP(x) (IS_WS(x) || IS_QTE(x))
# define IS_SNGL(x, s, n, o) (*x == '\'' && (s = find_next('\'', n, o)))
# define IS_DBL(x, s, n, o) (*x == '"' && (s = find_next('\'', n, o)))

char			**g_environ;
int				g_processes;
typedef int		(*t_builtinf)(int argc, char **argv);

typedef struct	s_pipeline
{
	char		***simple_commands;
	int			*io_channels;
	char		*flags;
}				t_pipeline;

typedef struct	s_job
{
	t_pipeline	*pipeline;
}				t_job;

typedef struct	s_ast_node
{
	t_pipeline	*pipeline;
}				t_ast_node;

typedef struct	s_ast_leaf
{
	t_ast_node	*left;
	t_ast_node	*right;
	char		flags[BITNSLOTS(8)];
}				t_ast_leaf;

typedef union	u_ast_elem
{
	t_ast_leaf	*leaf;
	t_ast_node	*node;
}				t_ast_elem;

typedef struct	s_ast
{
	t_ast_elem	*root;
	char		flags[BITNSLOTS(1)];
}				t_ast;

typedef struct	s_builtin
{
	char		*cmd;
	t_builtinf	f;
	int			len;
}				t_builtin;

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
** builtins/lexer.c
*/

int				lexer(char *complete_cmd, char ***tokens);

/*
** parse/parse.c
*/

int				remove_slash(char elem, size_t i, char *str, int *stop);
int				parse_commands(char **tokens, t_ast *ast);
int				prepare_ast(char *complete_cmd, t_ast *ast, int i);

/*
** parse/quotes.c
*/

char			**remove_quotations(char *command, int ac);
int				count_params(char *command);

/*
** parse/command.c
*/

void			expand_command(char **command);
int				execute_commands(char *command);
int				builtin_command(char **command);

/*
** traverse/traverse.c
*/

int				traverse_ast(t_ast *ast);
#endif
