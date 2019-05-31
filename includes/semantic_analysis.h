/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic_analysis.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:38:57 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/31 16:41:37 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEMANTIC_ANALYSIS_H
# define SEMANTIC_ANALYSIS_H

# include "syntactic_analysis.h"
# include "execution_node.h"

# define IS_TERMINAL(x) (x >= 1 && x <= 24)
# define IS_A(type, node) (!ft_strcmp(type, node))
# define CONTAINS(type, node) (ft_strstr(node, type))
# define ERR_UNSET_PARAM "Semantic Error: word unset in ${parameter[:]?[word]}"
# define CMD_SUB_ERR "Semantic Error: Command Substitution not implemented\n"

enum			e_index
{
	I,
	J
};

/*
** Struct used in managing heredoc's accepting characters to add to line
*/

typedef struct	s_heredoc
{
	char		*buf;
	char		*tmp;
	char		*line;
	size_t		here_end_len;
	size_t		buf_len;
	size_t		line_len;
	int			fd[2];
}				t_heredoc;

# define CREATE_LINE(h) (!h.line && (!(h.line = ft_strnew(h.here_end_len))))
# define H_END(h) (h.here_end_len == h.buf_len)
# define HERE_END_FOUND(n, h, r) (n == '\n' && H_END(h) && IS_A(r->word, h.buf))
# define BUF_FULL(f, h, n) (!f && (h.here_end_len == h.buf_len || n == '\n'))
# define E_CLOSE(h, tty) (ERR(restore_terminal(&tty[1])) || ERR(close(h.fd[1])))
# define RESTORE_HERE_END(h, tty) ((E_CLOSE(h, tty)))

/*
** Struct used in managing splitting line into fields used by execve
*/

typedef struct	s_field_split
{
	size_t		skip;
	int			i;
	int			start;
	int			size;
	char		**arr;
}				t_field_split;

# define NOT_IFS(p, f) ((*p)[f.i] && (!IS_IFS((*p)[f.i]) || escaped(*p, f.i)))
# define NEW_STR(p, fs) (ft_strsub(*p, fs.start, fs.i - fs.start))
# define NEW_SUB(p, fs) ((fs.arr[fs.size++] = NEW_STR(p, fs)))
# define STORE_SUBSTRING(p, fs) ((fs.i - fs.start > 0) && (!NEW_SUB(p, fs)))

/*
** When processing simple command in its 5 possible forms, we use enum symbols
** to simplify identifying symbol position in ast
*/

enum			e_simple_positions
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

enum			e_expansion_symbol
{
	FULL_PARAM,
	NAME,
	WORD,
	TEST
};

/*
** src/semantic_analysis/affixes.c
*/

int				io_here(t_simple *cmd, int io_num, t_ast_node *root);
int				io_redirect(t_simple *cmd, t_ast_node *root);
int				prefix(t_simple *cmd, t_ast_node *root);
int				suffix_word(t_simple *cmd, t_ast_node *root, int position);
int				suffix(t_simple *cmd, t_ast_node *root);

/*
** src/semantic_analysis/command.c
*/

int				cmd_name(t_simple *simple, t_ast_node *root);
void			simple_positions(char *form, int position[3]);
int				simple_command(t_simple *simple, t_ast_node *root);
int				command(t_exec_node *container
						, t_ast_node *root
						, int bg
						, int bang);

/*
** src/semantic_analysis/here_end_utils.c
*/

int				init_heredoc(t_redir *redir
							, t_heredoc *heredoc
							, struct termios tty[2]
							, int *found);
int				handle_full_buffer(t_heredoc *heredoc);
int				add_newline_char(t_redir *redir, t_heredoc *heredoc);
int				remove_last_char(t_heredoc *heredoc);
void			add_next_char(t_heredoc *heredoc, char *next);

/*
** src/semantic_analysis/pipe.c
*/

int				pipe_sequence(t_exec_node *container
							, t_ast_node *root
							, int bg
							, int bang);
int				pipeline(t_exec_node *container, t_ast_node *root, int bg);

/*
** src/semantic_analysis/process_here_end.c
*/

int				here_end_found(void);
int				process_heredoc(t_redir *redir);

/*
** src/semantic_analysis/redir_utils.c
*/

int				push_redir(t_simple *cmd, t_redir *redir);
int				get_filename(char **filename, t_ast_node *root);
void			redir_not_implemented(char **name, int type);
int				process_redir(t_redir *redir
							, int io_num
							, char *filename
							, int type);
int				create_redir(t_redir **redir, t_ast_node *root, char **file);

/*
** src/semantic_analysis/semantic.c
*/

int				and_or(t_exec_node *container, t_ast_node *root, int is_bg);
int				list(t_program *p, t_ast_node *root, int last_is_bg);
int				semantic_analysis(t_ast *ast, t_program *p);

/*
** src/semantic_analysis/semantic_utils.c
*/

int				io_file(t_simple *cmd, int io_num, t_ast_node *root);
int				io_number(int *io_num, t_ast_node *root);
void			free_ast_node(t_ast_node *root);
void			free_ast(t_ast *ast);

/*
** src/semantic_analysis/separator.c
*/

int				separator_op_is_linebreak(int *is_bg, t_ast_node *root);
int				separator(int *is_bg, t_ast_node *root);

/*
** src/semantic_analysis/verify_command.c
*/

int				load_exec(t_simple *simple);
int				load_builtin(t_simple *simple);
int				find_exec(char *command);
int				find_command(char **command, char **paths, int i, int found);
int				verify_command(t_simple *simple);

/*
** src/semantic_analysis/expansions/arithmetic_expansion.c
*/

int				arith_exp_err(char **str, int start, int end);
int				arithmetic_expansion(char **parameter);

/*
** src/semantic_analysis/expansions/command_substitution.c
*/

int				cmd_sub_err(char **str, int start, int end);
int				command_substitution(char **parameter);

/*
** src/semantic_analysis/expansions/expansion.c
*/

int				full_word_expansion(char ***new, char *old);
int				param_expansion(char **new, char *old);
int				heredoc_expansion(t_redir *redir, char **new, char *old);
int				heredoc_line_expansion(char **new, char *old);
int				redir_expansion(char **new, char *old);

/*
** src/semantic_analysis/expansions/expansion_subtypes.c
*/

int				swap_param(char **parameter, char *param[3], int quoted);
int				positional_param(void);
int				plain_param_expansion(char **parameter, char *var, size_t *i);

/*
** src/semantic_analysis/expansions/field_splitting.c
*/

int				count_fields(char **str, int *count);
int				init_field_split(t_field_split *fs, char **parameter);
int				field_splitting(char ***fields, char **parameter);

/*
** src/semantic_analysis/expansions/parameter_expansion.c
*/

int				store_unexpanded(char **new
								, char **str
								, size_t start
								, size_t *i);
int				join_unexpanded(char **new, char **str, size_t *i);
int				switch_expansion(char *param, char **next, size_t *i);
int				manage_expansions(char **new, char **str, size_t *skip);
int				parameter_expansion(char **str);

/*
** src/semantic_analysis/expansions/parameter_expansion_utils.c
*/

char			*find_end_brace(char *param);
void			init_plain_exp(char *param[6], size_t len[4], char *str);
int				init_param(char *param[6], size_t len[4], char sep, char *str);
int				create_param(char *param[3], size_t len[3]);
int				enclosed(char *str, char c);

/*
** src/semantic_analysis/expansions/pathname_expansion.c
*/

int				find_pathname_end(char *str, int *skip);
int				expand_pathname(char **field);
int				pathname_expansion(char ***fields);

/*
** src/semantic_analysis/expansions/quote_removal.c
*/

int				remove_quotes(char **str);
int				quote_removal(char ***fields);
void			remove_quote_switch(char *quote
								, char **str
								, char *new
								, int index[2]);

/*
** src/semantic_analysis/expansions/tilde_expansion.c
*/

int				tilde_expansion(char **parameter);
#endif
