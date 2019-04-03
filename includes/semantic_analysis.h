/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic_analysis.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:38:57 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/02 17:34:18 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEMANTIC_ANALYSIS_H
# define SEMANTIC_ANALYSIS_H

# include "syntactic_analysis.h"

# define IO(x, y) (ERR(x) ? y : x)
# define IS_A(type, node) (!ft_strcmp(type, node))

/*
** Used by execution to differentiate types of redirections 
*/

enum						e_redir_type
{
	EXEC_DLESS,
	EXEC_DGREAT,
	EXEC_LESSAND,
	EXEC_GREATAND,
	EXEC_LESSGREAT,
	EXEC_DLESSDASH,
	EXEC_CLOBBER
};

/*
** Used by execution to differentiate types of nodes tree contains 
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
** Every command may have an associated list of redirections
*/

struct						s_redir;

typedef struct				s_redir
{
	char					*word;
	struct s_redir			*next;
	int						type;
	int						original;
	int						replacement;
}							t_redir;

/*
** simple command tokens represent discrete commands
** which consist of command array & redirection chain
*/

typedef struct				s_simple_command
{
	char					**command;
	t_redir					*redirs;
	int						is_builtin;
	int						bg;
}							t_simple_command;

/*
** pipe tokens represent pipes, where children are pipes or commands
*/

struct						s_pipe;

typedef struct				s_pipe_child
{
	union 
	{
		struct s_pipe		*pipe;
		t_simple_command	*simple_command;
	};
	int						type;
}							t_pipe_child;

typedef struct				s_pipe
{
	t_pipe_child			*left;
	t_pipe_child			*right;
	int						type;
	int						bg;
	int						bang;
}							t_pipe;

/*
** operator tokens represent logical operations, where left and right children
** are other operators, pipes or commands
*/

struct						s_operator;

typedef struct				s_op_child
{
	union
	{
		struct s_operator	*operator;
		t_pipe				*pipe;
		t_simple_command	*simple_command;
	};
	int						type;
}							t_op_child;

typedef struct				s_operator
{
	t_op_child				*left;
	t_op_child				*right;
	int						type;
	int						bg;
}							t_operator;

typedef struct				s_program
{
	t_operator				**commands;
}							t_program;
/*
** src/semantic_analysis/affixes.c
*/

int		io_file(t_simple_command *cmd, int io_num, t_ast_node *root);
int		io_number(int *io_num, t_ast_node *root);
int		io_redirect(t_simple_command *cmd, t_ast_node *root);
int		prefix(t_simple_command *cmd, t_ast_node *root);
int		suffix(t_simple_command *cmd, t_ast_node *root);

/*
** src/semantic_analysis/command.c
*/

int		cmd_name(t_simple_command *command, t_ast_node *root);
int		exec_simple_command(t_ast_node *root);

/*
** src/semantic_analysis/here_end.c
*/

int		io_here(t_simple_command *cmd, int io_num, t_ast_node *root);

/*
** src/semantic_analysis/pipe.c
*/

int		command(t_simple_command *command, t_ast_node *root, int bg);
int		pipe_sequence(t_pipe *pipe, t_ast_node *root, int bg);
int		pipeline(t_pipe *pipe, t_ast_node *root, int bg);

/*
** src/semantic_analysis/redir_utils.c
*/

int		push_redir(t_simple_command *cmd, t_redir *redir);
int		get_filename(char **filename, t_ast_node *root);
int		simple_redir(t_redir *redir, int io_num, char *filename, int type);
int		dup_redir(t_redir *redir, int io_num, char *filename, int type);
int		create_redir(t_redir **redir, t_ast_node *root, char **file);

/*
** src/semantic_analysis/semantic.c
*/

char	**push_pointer_back(char **pointers, t_ast_node *node);
void	**push_pointer_front(void **pointers, void *ptr);
int		and_or(t_operator *op, t_ast_node *root, int is_bg);
int		list(t_program *program, t_ast_node *root, int last_is_bg);
int		semantic_analysis(t_ast *ast, t_program *program);

/*
** src/semantic_analysis/expansion.c
*/

int		word_expansion(char **new, char *old);

/*
** src/semantic_analysis/separator.c
*/

int		separator_op_is_linebreak(int *is_bg, t_ast_node *root);
int		separator(int *is_bg, t_ast_node *root);
#endif
