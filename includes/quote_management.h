/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/15 14:37:57 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/10 21:10:11 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_MANAGEMENT_H
# define QUOTE_MANAGEMENT_H

# ifndef LIBFT_H
#  include "../libftprintf/srcs/includes/ft_printf.h"
# endif

/*
** Used by quote management functions to give context pecific functionality to
** functions as they parse different quote types - lexical analysis needs to
** store unclosed quotes, word expansion needs to execute closed quoted strings
*/

typedef	int	(*t_quote)(char **str, int start, int end);

# define NEXT_BRACE(s, i) (s[i + 1] && s[i + 1] == '{')
# define NEXT_PAREN(s, i) (s[i + 1] && s[i + 1] == '(')
# define TWO_PARENS(s, i) (NEXT_PAREN(s, i) && NEXT_PAREN(s, i + 1))
# define TWO_CLOSING(s, i) (s[i] == ')' && s[i + 1] == ')')
# define UPPER(x) ((x >= 'A' && x <= 'Z'))
# define IS_VAR_CHAR(x) (((UPPER(x)) || (x >= '0' && x <= '9') || x == '_'))

# define SNGL_QUOTE(s, i) ((s[i] == '\''))
# define DBL_QUOTE(s, i) ((s[i] == '"'))
# define EPARAM_EXP(s, i) ((s[i] == '$' && (NEXT_BRACE(s, i))))
# define UPARAM_EXP(s, i) ((s[i] == '$' && (IS_VAR_CHAR(s[i + 1]))))
# define ARITH_EXP(s, i) ((s[i] == '$' && TWO_PARENS(s, i)))
# define CMD_SUB(s, i) ((s[i] == '$' && NEXT_PAREN(s, i)))
# define BACKTICK(s, i) ((s[i] == '`'))
# define BACKSLASH(s, i) ((s[i] == '\\'))

# define BS(s, in, f, i, j) (OK((s = backslash(in, i, j, f))))
# define SQ(s, in, f, i, j) (OK((s = quote(in, i, j, f))))
# define DQ(s, in, f, i, j) (OK((s = dbl_quote(in, i, j, f))))
# define BT(s, in, f, i, j) (OK((s = backtick(in, i, j, f))))
# define AE(s, in, f, i, j) (OK((s = arith_exp(in, i, j, f))))
# define CE(s, in, f, i, j) (OK((s = command_sub(in, i, j, f))))
# define UPE(s, in, f, i, j) (OK((s = uparam_exp(in, i, j, f))))
# define EPE(s, in, f, i, j) (OK((s = eparam_exp(in, i, j, f))))

# define P_BACKSLASH(s, in, f, i, j) (BACKSLASH((*in), i) && BS(s, in, f, i, j))
# define P_QUOTE(s, in, f, i, j) (SNGL_QUOTE((*in), i) && SQ(s, in, f, i, j))
# define P_DQUOTE(s, in, f, i, j) (DBL_QUOTE((*in), i) && DQ(s, in, f, i, j))
# define P_BACKTICK(s, in, f, i, j) (BACKTICK((*in), i) && BT(s, in, f, i, j))
# define P_ARITH(s, in, f, i, j) (ARITH_EXP((*in), i) && AE(s, in, f, i, j))
# define P_CMD(s, in, f, i, j) (CMD_SUB((*in), i) && CE(s, in, f, i, j))
# define P_UPARAM(s, in, f, i, j) (UPARAM_EXP((*in), i) && UPE(s, in, f, i, j))
# define P_EPARAM(s, in, f, i, j) (EPARAM_EXP((*in), i) && EPE(s, in, f, i, j))

# define IS_SUB(s, i) ((s[i] == '(' && first_word(s, i)))
# define SUB(s, in, f, i, j) (OK((s = subshell(in, i, j, f))))
# define P_SUBSHELL(s, in, f, i, j) (IS_SUB((*in), i) && SUB(s, in, f, i, j))

# define IS_SPACE(s, i) (s[i + 1] ? s[i + 1] == ' ' : 1)
# define IS_CURSH(s, i) ((s[i] == '{' && first_word(s, i) && IS_SPACE(s, i)))
# define CUR(s, in, f, i, j) (OK((s = subshell(in, i, j, f))))
# define P_CURSHELL(s, in, f, i, j) (IS_CURSH((*in), i) && CUR(s, in, f, i, j))

/*
** Used within lexer to track types of quotes/substitutions
** in given command and whether all are properly closed
*/

enum			e_missing_sym
{
	CMD_SUB = 1,
	MATH_SUB = 2,
	BRACE_SUB = 3,
	VAR_SUB = 4,
	BQUOTE = 5,
	QUOTE = 6,
	DQUOTE = 7,
	BACKSLASH_ESC = 8
};

typedef struct	s_keyval
{
	int			type;
	char		*value;
}				t_keyval;

int				first_word(char *str, int i);
int				parse_all(char **str, size_t start, size_t *end, t_quote f);
int				subshell(char **str, size_t start, size_t *end, t_quote f);
int				cur_shell(char **str, size_t start, size_t *end, t_quote f);

/*
** src/utils/compund_quoting.c
*/

int				dbl_quote(char **str, size_t start, size_t *end, t_quote f);
int				uparam_exp(char **str, size_t start, size_t *end, t_quote f);
int				eparam_exp(char **str, size_t start, size_t *end, t_quote f);
int				command_sub(char **str, size_t start, size_t *end, t_quote f);
int				arith_exp(char **str, size_t start, size_t *end, t_quote f);

/*
** src/utils/simple_quoting.c
*/

int				backslash(char **str, size_t start, size_t *end, t_quote f);
int				quote(char **str, size_t start, size_t *end, t_quote f);
int				backtick(char **str, size_t start, size_t *end, t_quote f);

/*
** src/utils/quoting_utils.c
*/

int				init_exp(size_t *i, size_t start, int *found);
int				dbl_quote_switch(size_t *end, int *found, size_t len);
int				param_exp_end(size_t *end, size_t len);
int				arith_exp_switch(size_t *end, size_t len, int *found);
int				switch_arith_exp(char **str
								, size_t i
								, size_t *count
								, int *found);
#endif
