/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_node.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:38:57 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/26 17:03:23 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_NODE_H
# define EXECUTION_NODE_H

/*
** Signature of shell builtin functions
*/

typedef int						(*t_builtinf)(int argc, char **argv);

/*
** Structs used to match name of builtin to its function pointer
*/

typedef struct					s_builtin
{
	char						*cmd;
	t_builtinf					f;
}								t_builtin;

# define TOTAL_BUILTINS 6

/*
** Global variable holding structs mapping builtin names to function pointers
*/

extern t_builtin				g_builtins[];

/*
** Types used by execution to differentiate types of redirections specified
*/

typedef struct					s_redir_cnv
{
	int							parse_tok;
	int							redir_tok;
}								t_redir_cnv;

enum							e_redir_type
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

enum							e_exec_token_type
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
** and array of t_exec_nodes. An exec node may be one of three types -
** logical (representing && || operations), pipes (representing |), 
** or simple commands (representing simple commands). These represtive structs
** are stored in a union, and the type member of the t_exec_node specifies
** which is contained within
*/

struct							s_operator;
struct							s_simple_command;
struct							s_pipe;

typedef struct					s_exec_node
{
	union
	{
		struct s_operator		*operator;
		struct s_simple_command	*simple_command;
		struct s_pipe			*pipe;
	};
	int							type;
}								t_exec_node;

/*
** Every command may have an associated list of redirections
*/

struct							s_redir;

typedef struct					s_redir
{
	char						*word;
	struct s_redir				*next;
	int							type;
	int							original;
	int							replacement;
	int							heredoc_quoted;
}								t_redir;

/*
** simple command tokens represent discrete commands
** which consist of command array & redirection chain
*/

typedef struct					s_simple_command
{
	char						**command;
	t_redir						*redirs;
	t_builtinf					builtin;
	int							bg;
	int							bang;
	int							is_exec;
	int							exit_status;
	int							argc;
}								t_simple;

/*
** pipe tokens represent pipes, where children are pipes or commands
*/

typedef struct					s_pipe
{
	t_exec_node					*left;
	t_exec_node					*right;
	int							bg;
	int							bang;
	int							exit_status;
}								t_pipe;

/*
** operator tokens represent logical operations, where left and right children
** are other operators, pipes or commands
*/

typedef struct					s_operator
{
	t_exec_node					*left;
	t_exec_node					*right;
	int							type;
	int							bg;
	int							exit_status;
}								t_operator;

typedef struct					s_program
{
	t_exec_node					**commands;
}								t_program;
#endif
