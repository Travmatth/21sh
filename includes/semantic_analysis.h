/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic_analysis.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:38:57 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/07 17:41:05 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEMANTIC_ANALYSIS_H
# define SEMANTIC_ANALYSIS_H

# include "syntactic_analysis.h"

# define EXECUTABLE 1
# define BUILTIN 2
# define IO(x, y) (ERR(x) ? y : x)
# define IS_A(type, node) (!ft_strcmp(type, node))
# define TILDE_PREFIX(x) (x && x[0] == '~')
# define ERR_UNSET_PARAM "Semantic Error: word unset in ${parameter[:]?[word]}"
# define UPPER(x) (x >= 'A' && x <= 'Z')
# define IS_VAR_CHAR(x) ((UPPER(x)) || (x >= '0' && x <= '9') || x == '_')
# define CMD_SUB_ERR "Semantic Error: Command Substitution not implemented\n"
# define IS_IFS(x) (x == ' ' || x == '\t' || x == '\n')

enum						e_expansion_symbol
{
	FULL_PARAM,
	NAME,
	WORD,
	TEST
};

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
int		io_here(t_simple_command *cmd, int io_num, t_ast_node *root);
int		io_redirect(t_simple_command *cmd, t_ast_node *root);
int		prefix(t_simple_command *cmd, t_ast_node *root);
int		suffix(t_simple_command *cmd, t_ast_node *root);

/*
** src/semantic_analysis/command.c
*/

char	**ft_strjoinarrs(char **arr_1, char **arr_2);
int		cmd_name(t_simple_command *command, t_ast_node *root);
int		simple_command(t_simple_command *command, t_ast_node *root, int bg);
int		command(t_simple_command *cmd, t_ast_node *root, int bg);

/*
** src/semantic_analysis/parameter_expansion_actions.c
*/

int		substitute_word(char **parameter, char *param[3]);
int		substitute_parameter(char **parameter, char *param[3], char *env_lookup);
int		substitute_null(char **parameter, char *param[3]);
int		assign_word(char **parameter, char *param[3]);
int		error_exit(char **parameter, char *param[3]);

/*
** src/semantic_analysis/pipe.c
*/

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
int		list(t_program *p, t_ast_node *root, int last_is_bg);
int		semantic_analysis(t_ast *ast, t_program *program);

/*
** src/semantic_analysis/separator.c
*/

int		separator_op_is_linebreak(int *is_bg, t_ast_node *root);
int		separator(int *is_bg, t_ast_node *root);

/*
** src/semantic_analysis/verify_command.c
*/

int		find_exec(char *command);
int		find_command(char **command, char **paths, int i, int found);
int		is_builtin(char *command);
int		verify_command(char **command);

/*
** src/semantic_analysis/expansions/arithmetic_expansion.c
*/

int		arithmetic_expansion(char **parameter);

/*
** src/semantic_analysis/expansions/command_substitution.c
*/

int		process_backtick(char *cmd, int *skip);
int		process_arithmetic(char *cmd, int *skip);
int		process_paren(char *cmd, int *skip);
int		process_backtick(char *cmd, int *skip);
int		command_substitution(char **parameter);

/*
** src/semantic_analysis/expansions/expansion.c
*/

int		full_word_expansion(char ***new, char *old);
int		sub_expansion(char **new, char *old);

/*
** src/semantic_analysis/expansions/expansion_.c
*/

/*
** src/semantic_analysis/expansions/field_splitting.c
*/

int		field_splitting(char ***fields, char **parameter);

/*
** src/semantic_analysis/expansions/parameter_expansion.c
*/

int		join_unexpanded(char **new, char **str);
int		manage_expansions(char **new, char **str);
int		parameter_expansion(char **parameter);

/*
** src/semantic_analysis/expansions/expansion_subtypes.c
*/

enum	e_plain_vars
{
	NEW,
	NEXT
};

int		plain_param_expansion(char **parameter, char *var, size_t *i);
int		use_defaults_param_expansion(char **parameter, char *var, size_t *i);
int		assign_defaults_param_expansion(char **parameter, char *var, size_t *i);
int		error_unset_param_expansion(char **parameter, char *var, size_t *i);
int		alternative_param_expansion(char **parameter, char *var, size_t *i);

/*
** src/semantic_analysis/expansions/parameter_expansion_utils.c
*/

int		skip_parens(char *param, size_t *skip);
char	*find_end_brace(char *param);
int		init_param_state(char *param[3], size_t len[4], char sep, char *str);
int		create_param_state(char *param[3], size_t len[3]);
int		enclosed(char *str, char c);

/*
** src/semantic_analysis/expansions/pathname_expansion.c
*/

int		pathname_expansion(char ***fields);

/*
** src/semantic_analysis/expansions/quote_removal.c
*/

int		quote_removal(char ***fields);

/*
** src/semantic_analysis/expansions/tilde_expansion.c
*/

int		tilde_expansion(char **parameter);
#endif
