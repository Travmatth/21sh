/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_constructs.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 23:42:59 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/31 15:24:52 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONSTRUCTS_H
# define PARSE_CONSTRUCTS_H

/*
** Symbols used by syntactic parser to uniquely identify tokens
*/

enum			e_parse_sym
{
	PARSE_END,
	PARSE_AMPERSAND,
	PARSE_L_PAREN,
	PARSE_R_PAREN,
	PARSE_SEMICOLON,
	PARSE_LT,
	PARSE_GT,
	PARSE_AND_IF,
	PARSE_ASSIGNMENT_WORD,
	PARSE_BANG,
	PARSE_CLOBBER,
	PARSE_DGREAT,
	PARSE_DLESS,
	PARSE_DLESSDASH,
	PARSE_GREATAND,
	PARSE_IO_NUMBER,
	PARSE_LESSAND,
	PARSE_LESSGREAT,
	PARSE_LBRACE,
	PARSE_NEWLINE,
	PARSE_OR_IF,
	PARSE_RBRACE,
	PARSE_WORD,
	PARSE_FUNCTION_DEFINITION,
	PARSE_PIPE,
	PARSE_AND_OR,
	PARSE_BRACE_GROUP,
	PARSE_CMD_NAME,
	PARSE_CMD_PREFIX,
	PARSE_CMD_SUFFIX,
	PARSE_CMD_WORD,
	PARSE_COMMAND,
	PARSE_COMPLETE_COMMAND,
	PARSE_COMPOUND_COMMAND,
	PARSE_COMPOUND_LIST,
	PARSE_FILENAME,
	PARSE_HERE_END,
	PARSE_IO_FILE,
	PARSE_IO_HERE,
	PARSE_IO_REDIRECT,
	PARSE_LINEBREAK,
	PARSE_LIST,
	PARSE_NEWLINE_LIST,
	PARSE_PIPE_SEQUENCE,
	PARSE_PIPELINE,
	PARSE_REDIRECT_LIST,
	PARSE_SEPARATOR,
	PARSE_SEPARATOR_OP,
	PARSE_SEQUENTIAL_SEP,
	PARSE_SIMPLE_COMMAND,
	PARSE_SUBSHELL,
	PARSE_TERM,
	PARSE_LESS,
	PARSE_GREAT,
};

/*
** t_prod structs used by init_prods to store grammar productions listed
** in misc/productions.txt
*/

typedef struct	s_handle
{
	char		*lhs;
	char		**rhs;
}				t_prod;

/*
** hold productions used in src/syntactic_analysis/parse.c reduce function
** to map given sequence of tokens to the respective token they constitute
*/

t_prod			*g_prods;

/*
** parse table used by lr1 parser in src/syntactic_analysis/parse.c
** syntactic analysis function
*/

extern char		*g_parse_table[][53];

/*
** AST structures used to contain terminal and nonterminal tokens
** used by the syntactic parser
*/

typedef struct	s_ast_node
{
	int			type;
	char		*lhs;
	char		*rhs;
	void		**val;
}				t_ast_node;

/*
** struct used to hold parsed ast for later use in semantic analysis
*/

typedef struct	s_ast
{
	t_ast_node	*root;
	int			type;
}				t_ast;

/*
** After tokens are lexed, tokens are placed into stack nodes to allow
** syntactic analyis to interchangeably manage tokens and parse states
*/

typedef union	u_item
{
	t_ast_node	*token;
	int			state;
}				t_item;

typedef struct	s_stack
{
	int			type;
	t_item		item;
}				t_stack;

/*
** enums used by `type` member of t_stack structs to identify type of
** stack token a given struct contains
*/

enum			e_stack_token_type
{
	STACK_END,
	STACK_TOKEN,
	STACK_STATE
};

/*
** Used by syntactic parser to match given token name to equivalent type
*/

# define TOTAL_PARSE_SYMS 26

typedef struct	s_parse_sym_lookup
{
	char		*lhs;
	int			sym;
}				t_parse_sym_lookup;
#endif
