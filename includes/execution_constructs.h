/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_constructs.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:38:57 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/07 22:24:13 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_CONSTRUCTS_H
# define EXECUTION_CONSTRUCTS_H

# define NEW_FILE_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
# define NEGATE(x, y) (x ? !y : y)
# define WAITPID_ERR "Runtime Error: forked process %d exited with signal: %s\n"

/*
** When a bash process exits normally, return value is 0
** When exiting because of an error, return value is 1
** This is in contrast to 21sh convention of SUCCESS == 1, NIL == 0
*/

# define NORMAL_CHILD_EXIT 0
# define ERROR_CHILD_EXIT 1
# define IS_NORMAL_CHILD_EXIT(x) (x == 0)
# define IS_ERROR_CHILD_EXIT(x) (x == 1)

/*
** When creating pipes, fd[0] is read end of pipe and fd[1] is write end
*/

# define PIPE_READ 0
# define PIPE_WRITE 1

/*
** Used in HEREDOC processing to detect signals sent to terminal
*/

# define IS_DONE(state, c) (state[STATE] == state[ACCEPT] && buf == '\n')
# define IS_SIG(state) (state[STATE] == state[SIG])

/*
** enums used by HEREDOC processing
*/

enum	e_here_end_buf
{
	HERE_END_BUF,
	IN_BUF
};

enum	e_here_end_dfa
{
	ACCEPT,
	SIG,
	STATE,
	LAST_STATE,
	CHARS
};

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
