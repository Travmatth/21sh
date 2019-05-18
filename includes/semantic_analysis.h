/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic_analysis.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:38:57 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/18 16:13:13 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEMANTIC_ANALYSIS_H
# define SEMANTIC_ANALYSIS_H

# include "syntactic_analysis.h"

# define IS_TERMINAL(x) (x >= 1 && x <= 24)
# define IS_A(type, node) (!ft_strcmp(type, node))
# define CONTAINS(type, node) (ft_strstr(node, type))
# define ERR_UNSET_PARAM "Semantic Error: word unset in ${parameter[:]?[word]}"
# define CMD_SUB_ERR "Semantic Error: Command Substitution not implemented\n"

/*
** Signature of shell builtin functions
*/

typedef int		(*t_builtinf)(int argc, char **argv);

# define TOTAL_BUILTINS 6

/*
** Structs used to match name of builtin to its function pointer
*/

typedef struct	s_builtin
{
	char		*cmd;
	t_builtinf	f;
}				t_builtin;

/*
** Global variable holding structs mapping builtin names to function pointers
*/

extern t_builtin	g_builtins[];

/*
** When processing simple command in its 5 possible forms, we use enum symbols
** to simplify identifying symbol position in ast
*/

enum	e_simple_positions
{
	PREFIX,
	COMMAND,
	SUFFIX
};

/*
** Used by redirection processing to differentiate between valid, optional file
** descriptors from defaults
*/

# define IO(x, y) (ERR(x) ? y : x)

/*
** Used by tilde_expansion to detect correctly-prefixed strings
** to be tilde expanded
*/

# define TILDE_PREFIX(x) (x && x[0] == '~')

/*
** Used by field_splitting to detect chars the given word should be split on
*/

# define IS_IFS(x) (x == ' ' || x == '\t' || x == '\n')

/*
** Symbols used in parameter expansion to identify given part of
** paramter, as given by the syntax
** FULL_PARAM = {NAME[TEST][SUBTYPE]WORD} 
*/

enum						e_expansion_symbol
{
	FULL_PARAM,
	NAME,
	WORD,
	TEST
};

/*
** Types used by execution to differentiate types of redirections specified
*/

typedef struct	s_redir_cnv
{
	int			parse_tok;
	int			redir_tok;
}				t_redir_cnv;

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
** and array of t_exec_nodes. An exec node may be one of three types -
** logical (representing && || operations), pipes (representing |), 
** or simple commands (representing simple commands). These represtive structs
** are stored in a union, and the type member of the t_exec_node specifies
** which is contained within
*/

struct						s_operator;
struct						s_simple_command;
struct						s_pipe;

typedef struct				s_exec_node
{
	union
	{
		struct s_operator		*operator;
		struct s_simple_command	*simple_command;
		struct s_pipe			*pipe;
	};
	int						type;
}							t_exec_node;

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
	int						heredoc_quoted;
}							t_redir;

/*
** simple command tokens represent discrete commands
** which consist of command array & redirection chain
*/

typedef struct				s_simple_command
{
	char					**command;
	t_redir					*redirs;
	t_builtinf				builtin;
	int						bg;
	int						bang;
	int						is_exec;
	int						exit_status;
	int						argc;
}							t_simple;

/*
** pipe tokens represent pipes, where children are pipes or commands
*/

typedef struct				s_pipe
{
	t_exec_node				*left;
	t_exec_node				*right;
	int						bg;
	int						bang;
	int						exit_status;
}							t_pipe;

/*
** operator tokens represent logical operations, where left and right children
** are other operators, pipes or commands
*/

typedef struct				s_operator
{
	t_exec_node				*left;
	t_exec_node				*right;
	int						type;
	int						bg;
	int						exit_status;
}							t_operator;

typedef struct				s_program
{
	t_exec_node				**commands;
}							t_program;

/*
** src/semantic_analysis/affixes.c
*/

int		io_file(t_simple *cmd, int io_num, t_ast_node *root);
int		io_number(int *io_num, t_ast_node *root);
int		io_here(t_simple *cmd, int io_num, t_ast_node *root);
int		io_redirect(t_simple *cmd, t_ast_node *root);
int		prefix(t_simple *cmd, t_ast_node *root);
int		suffix(t_simple *cmd, t_ast_node *root);

/*
** src/semantic_analysis/command.c
*/

int		cmd_name(t_simple *simple, t_ast_node *root);
void	simple_positions(char *form, int position[3]);
int		simple_command(t_simple *simple, t_ast_node *root);
int		command(t_exec_node *container, t_ast_node *root, int bg, int bang);

/*
** src/semantic_analysis/parameter_expansion_actions.c
*/

int		substitute_word(char **parameter, char *param[3]);
int		substitute_parameter(char **parameter, char *env_lookup);
int		substitute_null(char **parameter);
int		assign_word(char **parameter, char *param[3]);
int		error_exit(char **parameter, char *param[3]);

/*
** src/semantic_analysis/pipe.c
*/

int		pipe_sequence(t_exec_node *container, t_ast_node *root, int bg, int bang);
int		pipeline(t_exec_node *container, t_ast_node *root, int bg);

/*
** src/semantic_analysis/redir_utils.c
*/

int		push_redir(t_simple *cmd, t_redir *redir);
int		get_filename(char **filename, t_ast_node *root);
int		process_redir(t_redir *redir, int io_num, char *filename, int type);
int		create_redir(t_redir **redir, t_ast_node *root, char **file);

/*
** src/semantic_analysis/semantic.c
*/

char	**push_pointer_back(char **pointers, t_ast_node *node);
void	**push_pointer_front(void **pointers, void *ptr);
int		and_or(t_exec_node *container, t_ast_node *root, int is_bg);
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
int		load_builtin(t_simple *simple);
int		verify_command(t_simple *simple);

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
int		param_expansion(char **new, char *old);
int		heredoc_expansion(t_redir *redir, char **new, char *old);
int		redir_expansion(char **new, char *old);
int		heredoc_line_expansion(char **new, char *old);

/*
** src/semantic_analysis/expansions/field_splitting.c
*/

int		field_splitting(char ***fields, char **parameter);

/*
** src/semantic_analysis/expansions/parameter_expansion.c
*/

int		join_unexpanded(char **new, char **str, size_t *i);
int		manage_expansions(char **new, char **str, size_t *skip);
int		parameter_expansion(char **parameter);

/*
** src/semantic_analysis/expansions/expansion_subtypes.c
*/

int		plain_param_expansion(char **parameter, char *var, size_t *i);
int		use_defaults_param_expansion(char **parameter, char *var, size_t *i);
int		assign_defaults_param_expansion(char **parameter, char *var, size_t *i);
int		error_unset_param_expansion(char **parameter, char *var, size_t *i);
int		alternative_param_expansion(char **parameter, char *var, size_t *i);

/*
** src/semantic_analysis/expansions/parameter_expansion_utils.c
*/

char	*find_end_brace(char *param);
int		init_param_state(char *param[3], size_t len[4], char sep, char *str);
int		create_param_state(char *param[3], size_t len[3]);
int		enclosed(char *str, char c);

/*
** src/semantic_analysis/expansions/pathname_expansion.c
*/

int		expand_pathname(char **field);
int		pathname_expansion(char ***fields);

/*
** src/semantic_analysis/expansions/quote_removal.c
*/

int		remove_quotes(char **str);
int		quote_removal(char ***fields);

/*
** src/semantic_analysis/expansions/tilde_expansion.c
*/

int		tilde_expansion(char **parameter);

/*
** src/semantic_analysis/here_end_utils.c
*/

int		process_heredoc(t_redir *redir);

/*
** src/semantic_analysis/semantic_utils.c
*/

void	free_ast(t_ast *ast);
#endif
