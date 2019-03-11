/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/10 23:33:07 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

extern char		*g_parse_table[][53];
extern t_prod	*g_prods;

t_list	*create_stack_token(int type, t_ast_node *token, int state)
{
	t_stack	*stack_token;

	if (!(stack_token = (t_stack*)ft_memalloc(sizeof(t_stack))))
		return (NULL);
	stack_token->type = type;
	if (type == STACK_STATE)
		stack_token->item.state = state;
	else if (type == STACK_TOKEN)
	{
		stack_token->item.token = token;
	}
	return (ft_lstnew((void*)stack_token, sizeof(t_stack)));
}

t_list	*create_end_stack_token(void)
{
	t_stack	*stack_token;

	if (!(stack_token = (t_stack*)ft_memalloc(sizeof(t_stack))))
		return (NULL);
	stack_token->type = STACK_END;
	return (ft_lstnew((void*)stack_token, sizeof(t_stack)));
}

t_ast_node	*pop_token(t_list **tokens)
{
	t_list	*top;

	if (!(top = ft_lsthead(tokens)))
		return (NULL);
	return (t_ast_node*)((t_stack*)top->content)->item.token;
}

int		peek_state(t_list **stack, int *state)
{
	t_list	*top;

	if (!(top = ft_lstpeektail(*stack)))
		return (ERROR);
	if (((t_stack*)top->content)->type != STACK_STATE)
		return (ERROR);
	*state = ((t_stack*)top->content)->item.state;
	return (SUCCESS);
}

int		assign_type(char *lhs, t_ast_node *node)
{
	if (!ft_strcmp(lhs, "and_or"))
		node->type = PARSE_AND_OR;
	else if (!ft_strcmp(lhs, "brace_group"))
		node->type = PARSE_BRACE_GROUP;
	else if (!ft_strcmp(lhs, "cmd_name"))
		node->type = PARSE_CMD_NAME;
	else if (!ft_strcmp(lhs, "cmd_prefix"))
		node->type = PARSE_CMD_PREFIX;
	else if (!ft_strcmp(lhs, "cmd_suffix"))
		node->type = PARSE_CMD_SUFFIX;
	else if (!ft_strcmp(lhs, "cmd_word"))
		node->type = PARSE_CMD_WORD;
	else if (!ft_strcmp(lhs, "command"))
		node->type = PARSE_COMMAND;
	else if (!ft_strcmp(lhs, "complete_command"))
		node->type = PARSE_COMPLETE_COMMAND;
	else if (!ft_strcmp(lhs, "compound_command"))
		node->type = PARSE_COMPOUND_COMMAND;
	else if (!ft_strcmp(lhs, "compound_list"))
		node->type = PARSE_COMPOUND_LIST;
	else if (!ft_strcmp(lhs, "filename"))
		node->type = PARSE_FILENAME;
	else if (!ft_strcmp(lhs, "here_end"))
		node->type = PARSE_HERE_END;
	else if (!ft_strcmp(lhs, "io_file"))
		node->type = PARSE_IO_FILE;
	else if (!ft_strcmp(lhs, "io_here"))
		node->type = PARSE_IO_HERE;
	else if (!ft_strcmp(lhs, "io_redirect"))
		node->type = PARSE_IO_REDIRECT;
	else if (!ft_strcmp(lhs, "linebreak"))
		node->type = PARSE_LINEBREAK;
	else if (!ft_strcmp(lhs, "list"))
		node->type = PARSE_LIST;
	else if (!ft_strcmp(lhs, "newline_list"))
		node->type = PARSE_NEWLINE_LIST;
	else if (!ft_strcmp(lhs, "pipe_sequence"))
		node->type = PARSE_PIPE_SEQUENCE;
	else if (!ft_strcmp(lhs, "pipeline"))
		node->type = PARSE_PIPELINE;
	else if (!ft_strcmp(lhs, "redirect_list"))
		node->type = PARSE_REDIRECT_LIST;
	else if (!ft_strcmp(lhs, "separator"))
		node->type = PARSE_SEPARATOR;
	else if (!ft_strcmp(lhs, "separator_op"))
		node->type = PARSE_SEPARATOR_OP;
	else if (!ft_strcmp(lhs, "sequential_sep"))
		node->type = PARSE_SEQUENTIAL_SEP;
	else if (!ft_strcmp(lhs, "simple_command"))
		node->type = PARSE_SIMPLE_COMMAND;
	else if (!ft_strcmp(lhs, "subshell"))
		node->type = PARSE_SUBSHELL;
	else if (!ft_strcmp(lhs, "term"))
		node->type = PARSE_TERM;
	else
		return (ERROR);
	return (SUCCESS);
}

t_stack	*reduce_symbol(t_prod *handle, t_list **tmp)
{
	int			i;
	int			size;
	t_ast_node	*node;
	t_stack	*token;
	t_stack		*stack_token;

	if (!(node = (t_ast_node*)ft_memalloc(sizeof(t_ast_node))))
		return (NULL);
	i = 0;
	size = 0;
	while (handle->rhs[size])
		size += 1;
	if (!(node->rhs = ft_strsum(handle->rhs, " ")))
		return (NULL);
	if (!(node->val = (void**)ft_memalloc(sizeof(void*) * (size + 1))))
		return (NULL);
	while (i < size && ft_strcmp("$end", handle->rhs[i]))
	{
		token = (t_stack*)ft_lsttail(tmp)->content;
		node->val[i++] = token->item.token;
	}
	assign_type(handle->lhs, node);
	if (!(node->lhs = ft_strdup(handle->lhs)))
		return (NULL);
	if (!(stack_token = (t_stack*)ft_memalloc(sizeof(t_stack))))
		return (NULL);
	stack_token->type = STACK_TOKEN;
	stack_token->item.token = node;
	return (stack_token);
}

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
	t_ast_node		*word;
	int			state;

	(void)ast;
	stack = NULL;
	if (!ft_lstpushfront(&stack, create_end_stack_token())
		|| !ft_lstpushback(&stack, create_stack_token(STACK_STATE, NULL, 0))
		|| !(word = pop_token(tokens)))
		return (ERROR);
	while (42)
	{
		if (!(peek_state(&stack, &state)))
			return (ERROR);
		if (g_parse_table[state][word->type][0] == 'r')
		{
			if (ERR(reduce(state, &stack, word)))
				return (ERROR);
		}
		else if (g_parse_table[state][word->type][0] == 's')
		{
			if (ERR(shift(state, &stack, &word, tokens)))
				return (ERROR);
		}
		else if (g_parse_table[state][word->type][0] == 'a')
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
