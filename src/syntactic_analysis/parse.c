/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/26 16:20:35 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Action / GOTO table used in LR_1 parsing to determine, given a state and
** word, the next appropriate action. Entries of the form r# are meant to be
** passed to reduce function, where # is the index of the production rule to
** used. Entries of the form s# are shifts, meant to push current word and
** state given by # onto the stack. Entries of - denote an invalid parse, and
** the 'a' entry denotes the successful parsing of a given sentence.
*/

extern char		*g_parse_table[][53];

/*
** Structs of productions in the form of { char *lhs; char **rhs },
** which give the nonterminal and the array of [non]terminals it is composed of.
*/

extern t_prod	*g_prods;

/*
** If given series of tokens on the end of the stack constitute a greater
** nonterminal symbol, these tokens should be popped of the stack and reduced
** to the children of the given nonterminal and next state of parse stack, both
** of which are then pushed onto the stack
*/

int		reduce(int state, t_list **stack, t_ast_node *word)
{
	t_list	*tmp;
	t_list	*state_node;
	t_prod	*handle;
	t_stack	sym;
	int		next;

	tmp = NULL;
	handle = &g_prods[ft_atoi(&g_parse_table[state][word->type][1])];
	next = 0;
	while (handle->rhs && handle->rhs[next++])
	{
		state_node = ft_lsttail(stack);
		ft_lstdelone(&state_node, del_stack_node);
		ft_lstpushback(&tmp, ft_lsttail(stack));
	}
	if (OK(reduce_symbol(handle, &tmp, &sym, &next)))
	{
		peek_state(stack, &next);
		ft_lstpushback(stack, ft_lstnew(&sym, sizeof(t_stack)));
		next = ft_atoi(g_parse_table[next][sym.item.token->type]);
		ft_lstpushback(stack, create_stack_token(STACK_STATE, NULL, next));
		return (SUCCESS);
	}
	else
		return (ERROR);
}

/*
** LR_1 parsing uses shift in instances where the current word is a member
** of some greater nonterminal symbol, shifting pushes this word, along with
** next onto the stack, so word can be later reduced into parent nonterminal
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
** Successful LR_1 syntactic parsing results in a single `accept` token on end
** of stack, where token contains the ast to be used in semantic parsing.
** This root of the ast is stored in the ast struct
*/

int		accept_ast(t_list **stack, t_ast *ast, t_ast_node *word)
{
	t_list	*tmp;
	t_prod	*handle;
	t_stack	sym;
	int		status;

	tmp = ft_lsttail(stack);
	ft_lstdelone(&tmp, del_stack_node);
	handle = &g_prods[START];
	ft_lstpushback(&tmp, ft_lsttail(stack));
	if (OK((status = reduce_symbol(handle, &tmp, &sym, &status))))
	{
		ast->root = sym.item.token;
		ft_lstdel(stack, del_stack_node);
		free(sym.item.token->lhs);
		free(sym.item.token->rhs);
	}
	free(word->val[0]);
	free(word->val);
	free(word->lhs);
	free(word->rhs);
	free(word);
	return (status);
}

/*
** syntactic_analysis performs LR_1 parsing of given token stream by determining
** current state of parse, and whether to shift or reduce current word and stack
** (char*)((t_stack*)((t_stack*)stack->next->next->content)->item.token->val[0])
** ->item.token->val[0]
*/

int		init_stack(t_list **tokens, t_list **stack, t_ast_node **word)
{
	if (!ft_lstpushfront(stack, create_end_stack_token())
		|| !ft_lstpushback(stack, create_stack_token(STACK_STATE, NULL, 0))
		|| !(*word = pop_token(tokens)))
		return (ERROR);
	return (SUCCESS);
}

int		syntactic_analysis(t_list **tokens, t_ast *ast)
{
	t_list		*stack;
	t_ast_node	*word;
	int			state;
	char		*action;
	int			status;

	stack = NULL;
	if (ERR(init_stack(tokens, &stack, &word)))
		return (ERROR);
	status = word->type == PARSE_END ? NIL : SUCCESS;
	while (OK(status))
	{
		if (ERR(peek_state(&stack, &state)))
			return (ERROR);
		action = g_parse_table[state][word->type];
		if (action[0] == 'r')
			status = reduce(state, &stack, word);
		else if (action[0] == 's')
			status = shift(state, &stack, &word, tokens);
		else if (action[0] == 'a')
			return (accept_ast(&stack, ast, word));
		else if (action[0] == '-')
			status = unrecognized_syntax();
	}
	return (status);
}
