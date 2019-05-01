/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/15 14:37:57 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/29 15:04:47 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_MANAGEMENT_H
# define QUOTE_MANAGEMENT_H

#ifndef LIBFT_H
# include "../libftprintf/srcs/includes/ft_printf.h"
#endif

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
# define PARAM_EXP(s, i) ((s[i] == '$' && (NEXT_BRACE(s, i) || IS_VAR_CHAR(s[i + 1]))))
# define ARITH_EXP(s, i) ((s[i] == '$' && TWO_PARENS(s, i)))
# define CMD_SUB(s, i) ((s[i] == '$' && NEXT_PAREN(s, i)))
# define BACKTICK(s, i) ((s[i] == '`'))
# define BACKSLASH(s, i) ((s[i] == '\\'))

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

/*
** src/utils/quote_management.c
*/

int		backslash(char **str, size_t start, size_t *end, t_quote f);
int		quote(char **str, size_t start, size_t *end, t_quote f);
int		backtick(char **str, size_t start, size_t *end, t_quote f);
int		dbl_quote(char **str, size_t start, size_t *end, t_quote f);
int		param_exp(char **str, size_t start, size_t *end, t_quote f);
int		command_sub(char **str, size_t start, size_t *end, t_quote f);
int		arith_exp(char **str, size_t start, size_t *end, t_quote f);
#endif
