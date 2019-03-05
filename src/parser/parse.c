/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/04 17:38:32 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

extern char		*g_parse_table[][53];

t_list	*create_stack_token(int type, t_token *token, int state)
{
	t_stack	*stack_token;

	if (!(stack_token = (t_stack*)ft_memalloc(sizeof(t_stack))))
		return (NULL);
	stack_token->type = type;
	if (type == STACK_STATE)
		stack_token->item.state = state;
	else if (type == STACK_TOKEN)
		stack_token->item.token = token;
	return (ft_lstnew((void*)stack_token, sizeof(t_stack*)));
}

t_list	*create_end_stack_token(void)
{
	t_stack	*stack_token;

	if (!(stack_token = (t_stack*)ft_memalloc(sizeof(t_stack))))
		return (NULL);
	stack_token->type = STACK_END;
	return (ft_lstnew((void*)stack_token, sizeof(t_stack*)));
}

t_token	*pop_token(t_list **tokens)
{
	t_list	*top;

	if (!(top = ft_lsthead(tokens)))
		return (NULL);
	return ((t_token*)top->content);
}

int		peek_state(t_list **stack, int *state)
{
	t_list	*top;

	if (!(top = ft_lstpeekhead(stack)))
		return (ERROR);
	if (((t_stack*)top->content)->type != STACK_STATE)
		return (ERROR);
	*state = ((t_stack*)top->content)->item.state;
	return (SUCCESS);
}

t_list	*reduce_symbol(t_prod *handle, t_list *tmp)
{
	(void)handle;
	(void)tmp;
	return (NULL);
}

int		reduce(int state, t_list **stack, t_token *word)
{
	t_list	*tmp;
	t_prod	*handle;
	int		symbols;
	int		next_state;
	t_list	*next_tok;

	handle = g_prods[ft_atoi(&g_parse_table[state][1])];
	symbols = 0;
	while (handle->rhs[symbols])
	{
		ft_lsttail(stack);
		ft_lstpushfront(&tmp, ft_lsttail);
	}
	peek_state(stack, &next_state);
	ft_lstpushback(stack, reduce_symbol(handle, tmp));
	next_state = g_parse_table[next_state][word->type];
	next_tok = create_stack_token(STACK_STATE, NULL, next_state);
	ft_lstpushback(stack, next_tok);
	return (SUCCESS);
}

int		shift(int state, t_list **stack, t_token **word)
{
	int		next_state;

	next_state = ft_atoi(&g_parse_table[state][(*word)->type][1]);
	ft_lstpushback(stack, *word);
	ft_lstpushback(stack, create_stack_token(STACK_STATE, NULL, next_state));
	*word = pop_token(stack);
}

int		syntactic_analysis(t_list *tokens, t_ast *ast)
{
	t_list		*stack;
	t_token		*word;
	int			state;
	int			next;

	(void)ast;
	if (!ft_lstpushfront(&stack, create_end_stack_token())
		|| !ft_lstpushback(&stack, create_stack_token(STACK_STATE, NULL, 0))
		|| (word = pop_token(&tokens)))
		return (ERROR);
	while (42)
	{
		if (!(peek_state(stack, &state)))
			return (ERROR);
		if (parse_table[state][word->type][0] == 'r')
		{
			if (ERR(reduce(state, &stack, word)))
				return (ERROR);
		}
		else if (parse_table[state][word->type][0] == 's')
		{
			if (ERR(shift(state, &stack, &word, tokens)))
				return (ERROR);
		}
		else if (parse_table[state][word->type][0] == 'a')
			return (SUCCESS);
		else
			return (ERROR);
	}
	return (NIL);
}

int		prepare_ast(char *complete_cmd, t_ast *ast)
{
	int		status;
	t_list	*tokens;

	tokens = NULL;
	ft_bzero(ast, sizeof(t_ast));
	if (!OK((status = lexical_analysis(complete_cmd, &tokens, &ast->missing))))
		return (status);
	return (syntactic_analysis(tokens, ast));
}
