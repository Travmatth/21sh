/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic_constructs.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 19:26:49 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 19:32:46 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEMANTIC_CONSTRUCTS_H
# define SEMANTIC_CONSTRUCTS_H

# ifndef LIBFT_H
#  include "../libftprintf/srcs/includes/ft_printf.h"
# endif

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
#endif
