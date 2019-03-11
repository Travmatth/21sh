/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/11 00:58:09 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

extern char		*g_parse_table[][53];
extern t_prod	*g_prods;

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
	while (handle->rhs[symbols++])
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

int		shift(int state, t_list **stack, t_ast_node **word, t_list **tokens)
{
	int		next_state;

	next_state = ft_atoi(&g_parse_table[state][(*word)->type][1]);
	ft_lstpushback(stack, create_stack_token(STACK_TOKEN, *word, NIL));
	ft_lstpushback(stack, create_stack_token(STACK_STATE, NULL, next_state));
	*word = pop_token(tokens);
	return (SUCCESS);
}

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

// (char*)((t_stack*)((t_stack*)stack->next->next->content)->item.token->val[0])->item.token->val[0]
int		syntactic_analysis(t_list **tokens, t_ast *ast)
{
	t_list		*stack;
	t_ast_node	*word;
	int			state;
	char		*action;

	stack = NULL;
	if (!ft_lstpushfront(&stack, create_end_stack_token())
		|| !ft_lstpushback(&stack, create_stack_token(STACK_STATE, NULL, 0))
		|| !(word = pop_token(tokens)))
		return (ERROR);
	while (42)
	{
		if (!(peek_state(&stack, &state)))
			return (ERROR);
		action = g_parse_table[state][word->type];
		if (action[0] == 'r' && ERR(reduce(state, &stack, word)))
			return (ERROR);
		else if (action[0] == 's' && ERR(shift(state, &stack, &word, tokens)))
			return (ERROR);
		else if (action[0] == 'a')
			return (accept_ast(&stack, ast));
		else
			return (ERROR);
	}
	return (NIL);
}

/*
-exec p (char*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)stack_token->item.token->val[0])->val[0])->val[0])->val[0])->val[0])->val[0])->val[0])->val[0])->val[0])->val[0]
(char *) $34 = 0x0000602000002210 "cat"
-exec p (char*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)stack_token->item.token->val[0])->val[0])->val[0])->val[0])->val[0])->val[0])->val[0])->val[1])->val[0])->val[0]
(char *) $35 = 0x00006020000022b0 "author"
*/

int		prepare_ast(char *complete_cmd, t_ast *ast)
{
	int		status;
	t_list	*tokens;

	tokens = NULL;
	ft_bzero(ast, sizeof(t_ast));
	if (!OK((status = lexical_analysis(complete_cmd, &tokens, &ast->missing))))
		return (status);
	return (syntactic_analysis(&tokens, ast));
}
