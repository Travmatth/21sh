/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_node.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:38:57 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/31 15:23:00 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_NODE_H
# define EXECUTION_NODE_H

/*
** Signature of shell builtin functions
*/

typedef int					(*t_builtinf)(int argc, char **argv);

/*
** Structs used to match name of builtin to its function pointer
*/

typedef struct				s_builtin
{
	char					*cmd;
	t_builtinf				f;
}							t_builtin;

# define TOTAL_BUILTINS 6

/*
** Global variable holding structs mapping builtin names to function pointers
*/

extern t_builtin			g_builtins[];

/*
** Types used by execution to differentiate types of redirections specified
*/

typedef struct				s_redir_cnv
{
	int						parse_tok;
	int						redir_tok;
}							t_redir_cnv;

enum						e_redir_type
{
	REDIR_GT,
	REDIR_LT,
	REDIR_DLESS,
	REDIR_DGREAT,
	REDIR_LESSAND,
	REDIR_GREATAND,
	REDIR_LESSGREAT,
	REDIR_DLESSDASH,
	REDIR_CLOBBER
};

# define TOTAL_REDIRS 9

/*
** Types used by execution to differentiate types of nodes tree contains
*/

enum						e_exec_token_type
{
	EXEC_NONE,
	EXEC_SIMPLE_COMMAND,
	EXEC_PIPE,
	EXEC_AND,
	EXEC_OR,
	EXEC_OP
};

/*
** The output of the semantic tree is the t_program struct, which contains
** an array of t_exec_nodes. An exec node may be one of three types -
** logical (representing && || operations), pipes (representing |),
** or simple commands (representing simple commands). These representative
** structs are stored in a union, and the type member of the t_exec_node
** specifies which is contained within
*/

typedef struct s_operator	t_operator;
typedef struct s_simple		t_simple;
typedef struct s_pipe		t_pipe;
typedef struct s_exec_node	t_exec_node;
typedef struct s_redir		t_redir;
typedef struct s_program	t_program;
union u_node				u_node;

union						u_node
{
	t_simple				*simple;
	t_pipe					*pipe;
	t_operator				*operator;
};

struct						s_exec_node
{
	union u_node			node;
	int						type;
};

/*
** Every command may have an associated list of redirections
*/

struct						s_redir
{
	char					*word;
	t_redir					*next;
	int						type;
	int						original;
	int						replacement;
	int						heredoc_quoted;
};

/*
** simple command tokens represent discrete commands
** which consist of command array & redirection chain
*/

struct						s_simple
{
	char					**command;
	t_redir					*redirs;
	t_builtinf				builtin;
	int						bg;
	int						bang;
	int						is_exec;
	int						exit_status;
	int						argc;
};

/*
** pipe tokens represent pipes, where children are pipes or commands
*/

struct						s_pipe
{
	t_exec_node				*left;
	t_exec_node				*right;
	int						bg;
	int						bang;
	int						exit_status;
};

/*
** operator tokens represent logical operations, where left and right children
** are other operators, pipes or commands
*/

struct						s_operator
{
	t_exec_node				*left;
	t_exec_node				*right;
	int						type;
	int						bg;
	int						exit_status;
};

struct						s_program
{
	t_exec_node				**commands;
};
#endif
