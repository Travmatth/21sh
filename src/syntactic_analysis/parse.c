/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/17 17:50:50 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Action / GOTO table used in lr_1 parsing to determine, given a state and
** word, the next appropriate action. Entries of the form r# are meant to be
** passed to reduce function, where # is the index of the production rule to
** used. Entries of the form s# are shifts, meant to push current word and
** state given by # onto the stack. Entries of - denote an invalid parse, and
** the 'a' entry denotes the successful parsing of a given sentence.
*/

extern char		*g_parse_table[][53];

/*
** Structs of productions in the form of { char *lhs; char **rhs },
** which give the nonterminal and the array of [non]terminals it is composed by.
*/

extern t_prod	*g_prods;

/*
** If given series of tokens on the end of the stack constitute a greater
** nonterminal symbol, these tokens should be popped of the stack and reduced
** to the children of the given nonterminal, which is then pushed onto the stack
*/

int		reduce(int state, t_list **stack, t_ast_node *word)
{
	t_list	*tmp;
	t_prod	*handle;
	t_stack	*sym;
	int		symbols;
	int		next_state;

	tmp = NULL;
	handle = &g_prods[ft_atoi(&g_parse_table[state][word->type][1])];
	symbols = 0;
	while (handle->rhs && handle->rhs[symbols++])
	{
		ft_lsttail(stack);
		ft_lstpushback(&tmp, ft_lsttail(stack));
	}
	peek_state(stack, &next_state);
	sym = reduce_symbol(handle, &tmp);
	ft_lstpushback(stack, ft_lstnew(sym, sizeof(t_stack)));
	next_state = ft_atoi(g_parse_table[next_state][sym->item.token->type]);
	ft_lstpushback(stack, create_stack_token(STACK_STATE, NULL, next_state));
	return (SUCCESS);
}

/*
** LR_1 parsing uses shift in instances where the current word is a member
** of some greater nonterminal symbol, shifting pushes this word onto the
** stack so it may be later reduced into parent nonterminal 
*/

int		shift(int state, t_list **stack, t_ast_node **word, t_list **tokens)
{
	int		next_state;

	next_state = ft_atoi(&g_parse_table[state][(*word)->type][1]);
	ft_lstpushback(stack, create_stack_token(STACK_TOKEN, *word, NIL));
	ft_lstpushback(stack, create_stack_token(STACK_STATE, NULL, next_state));
	*word = pop_token(tokens);
	return (SUCCESS);
}

/*
** Successful lr_1 syntactic parsing results in a single `accept` token on end
** of stack, where token contains the ast to be used in semantic parsing.
** This root of the ast is stored in the ast struct
*/

int		accept_ast(t_list **stack, t_ast *ast)
{
	t_list	*tmp;
	t_prod	*handle;
	t_stack	*sym;

	tmp = NULL;
	ft_lsttail(stack);
	handle = &g_prods[START];
	ft_lstpushback(&tmp, ft_lsttail(stack));
	sym = reduce_symbol(handle, &tmp);
	ast->root = sym->item.token;
	return (SUCCESS);
}

/*
** syntactic_analysis performs lr_1 parsing of given token stream by determining
** current state of parse, and whether to shift or reduce current word and stack
** (char*)((t_stack*)((t_stack*)stack->next->next->content)->item.token->val[0])
** ->item.token->val[0]
*/

int		syntactic_analysis(t_list **tokens, t_ast *ast)
{
	t_list		*stack;
	t_ast_node	*word;
	int			state;
	char		*action;
	int			status;

	stack = NULL;
	if (!ft_lstpushfront(&stack, create_end_stack_token())
		|| !ft_lstpushback(&stack, create_stack_token(STACK_STATE, NULL, 0))
		|| !(word = pop_token(tokens)))
		return (ERROR);
	status = word->type == PARSE_END ? NIL : SUCCESS;
	while (OK(status))
	{
		if (!(peek_state(&stack, &state)))
			return (ERROR);
		action = g_parse_table[state][word->type];
		if (action[0] == 'r')
			status = reduce(state, &stack, word);
		else if (action[0] == 's')
			status = shift(state, &stack, &word, tokens);
		else if (action[0] == 'a')
			return (accept_ast(&stack, ast));
		else if (action[0] == '-')
		{
			ft_putendl("Syntactic Error: Unrecognized command syntax");
			status = NIL;
		}
	}
	return (status);
}
