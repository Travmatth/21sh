/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic_analysis.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:38:57 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 23:19:17 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEMANTIC_ANALYSIS_H
# define SEMANTIC_ANALYSIS_H

# include "syntactic_analysis.h"
# include "semantic_constructs.h"
# include "execution_constructs.h"

/*
** src/semantic_analysis/affixes.c
*/

int		prefix(t_simple *cmd, t_ast_node *root);
int		suffix(t_simple *cmd, t_ast_node *root);

/*
** src/semantic_analysis/command.c
*/

int		command(t_exec_node *container, t_ast_node *root, int bg, int bang);

/*
** src/semantic_analysis/here_end_utils.c
*/

int		init_heredoc(t_redir *redir
					, t_heredoc *heredoc
					, struct termios tty[2]
					, int *found);
int		handle_full_buffer(t_heredoc *heredoc);
int		add_newline_char(t_redir *redir, t_heredoc *heredoc);
int		remove_last_char(t_heredoc *heredoc);
void	add_next_char(t_heredoc *heredoc, char *next);

/*
** src/semantic_analysis/pipe.c
*/

int		pipeline(t_exec_node *container, t_ast_node *root, int bg);

/*
** src/semantic_analysis/process_here_end.c
*/

int		process_heredoc(t_redir *redir);

/*
** src/semantic_analysis/redir_utils.c
*/

int		push_redir(t_simple *cmd, t_redir *redir);
int		process_redir(t_redir *redir, int io_num, char *filename, int type);
int		create_redir(t_redir **redir, t_ast_node *root, char **file);

/*
** src/semantic_analysis/semantic.c
*/

int		semantic_analysis(t_ast *ast, t_program *p);

/*
** src/semantic_analysis/semantic_utils.c
*/

int		io_file(t_simple *cmd, int io_num, t_ast_node *root);
int		io_number(int *io_num, t_ast_node *root);
void	free_ast(t_ast *ast);

/*
** src/semantic_analysis/separator.c
*/

int		separator_op_is_linebreak(int *is_bg, t_ast_node *root);
int		separator(int *is_bg, t_ast_node *root);

/*
** src/semantic_analysis/verify_command.c
*/

int		verify_command(t_simple *simple);

/*
** src/semantic_analysis/expansions/arithmetic_expansion.c
*/

int		arithmetic_expansion(char **parameter);

/*
** src/semantic_analysis/expansions/command_substitution.c
*/

int		command_substitution(char **parameter);

/*
** src/semantic_analysis/expansions/expansion.c
*/

int		full_word_expansion(char ***new, char *old);
int		heredoc_expansion(t_redir *redir, char **new, char *old);
int		heredoc_line_expansion(char **new, char *old);
int		redir_expansion(char **new, char *old);

/*
** src/semantic_analysis/expansions/expansion_subtypes.c
*/

int		plain_param_expansion(char **parameter, char *var, size_t *i);

/*
** src/semantic_analysis/expansions/field_splitting.c
*/

int		field_splitting(char ***fields, char **parameter);

/*
** src/semantic_analysis/expansions/parameter_expansion.c
*/

int		parameter_expansion(char **str);

/*
** src/semantic_analysis/expansions/parameter_expansion_utils.c
*/

int		init_param(char *param[6], size_t len[4], char sep, char *str);
int		create_param(char *param[3], size_t len[3]);
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
#endif
