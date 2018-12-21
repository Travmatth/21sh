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

/*
** Operator DFA definitions
*/

# define AND_IF 2
# define OR_IF 4
# define DSEMI 6
# define DLESS 8
# define DGREAT 10
# define LESSAND 11
# define GREATAND 12
# define LESSGREAT 13
# define DLESSDASH 14
# define CLOBBER 15
# define ACCEPTING(x) ((x >= 2 || x <= 10) && !(x % 2) || (x >= 11) || x <= 15)
# define NOT_ERR_STATE(x) (x != 15)

# define EOI 1
# define IS_WS(x) (x == ' ' || x == '\t' || x == '\n')
# define IS_QTE(x) (x == '\'' || x== '"')
# define IS_SEP(x) (IS_WS(x) || IS_QTE(x))
# define IS_SNGL(x, s, n, o) (*x == '\'' && (s = find_next('\'', n, o)))
# define IS_DBL(x, s, n, o) (*x == '"' && (s = find_next('\'', n, o)))
# define ESCAPABLE_CHAR(x) ()
# define END 1
# define BEGIN 0

char			**g_environ;
int				g_processes;
typedef int		(*t_builtinf)(int argc, char **argv);

typedef struct	s_lctx
{
	short		i;
	short		j;
	short		stop;
	short		op_state;
	short		in_word;
}				t_lctx;

typedef struct	s_token
{
	int			type;
	char		*value;
}				t_token;

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
** builtins/lexical_analysis.c
*/

int				lexical_analysis(char *complete_cmd, t_list *tokens);
int				find_next(char c, char *complete_cmd, size_t *offset);

/*
** parse/parse.c
*/

int				remove_slash(char elem, size_t i, char *str, int *stop);
int				parse_commands(char **tokens, t_ast *ast);
int				prepare_ast(char *complete_cmd, t_ast *ast);

/*
** parse/quotes.c
*/

char			**remove_quotations(char *command, int ac);
int				find_sub_end(char c, char *complete_cmd, size_t *j);
int				escaped(char *complete_cmd, size_t i);
int				count_params(char *command, size_t *i, size_t *tok_count);
/*
** given compete_cmd with op starting at i, determine whether operator is valid
*/

int				can_form_op(char *complete_cmd, int in_op, size_t *i, int position);

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
