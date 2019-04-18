/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/16 18:08:57 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Array listing nonterminals and their equivalent type
*/

t_parse_sym_lookup	g_sym_lookup[27] =
{
	{ "and_or", PARSE_AND_OR },
	{ "brace_group", PARSE_BRACE_GROUP },
	{ "cmd_name", PARSE_CMD_NAME },
	{ "cmd_prefix", PARSE_CMD_PREFIX },
	{ "cmd_suffix", PARSE_CMD_SUFFIX },
	{ "cmd_word", PARSE_CMD_WORD },
	{ "command", PARSE_COMMAND },
	{ "complete_command", PARSE_COMPLETE_COMMAND },
	{ "compound_command", PARSE_COMPOUND_COMMAND },
	{ "compound_list", PARSE_COMPOUND_LIST },
	{ "filename", PARSE_FILENAME },
	{ "here_end", PARSE_HERE_END },
	{ "io_file", PARSE_IO_FILE },
	{ "io_here", PARSE_IO_HERE },
	{ "io_redirect", PARSE_IO_REDIRECT },
	{ "linebreak", PARSE_LINEBREAK },
	{ "list", PARSE_LIST },
	{ "newline_list", PARSE_NEWLINE_LIST },
	{ "pipe_sequence", PARSE_PIPE_SEQUENCE },
	{ "pipeline", PARSE_PIPELINE },
	{ "redirect_list", PARSE_REDIRECT_LIST },
	{ "separator", PARSE_SEPARATOR },
	{ "separator_op", PARSE_SEPARATOR_OP },
	{ "sequential_sep", PARSE_SEQUENTIAL_SEP },
	{ "simple_command", PARSE_SIMPLE_COMMAND },
	{ "subshell", PARSE_SUBSHELL },
	{ "term", PARSE_TERM }
};

/*
** Determine state at top of syntactic parse stack
*/

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

/*
** Once tokens are successfully reduced into parent nonterminal, assigns
** the type of parent onto ast_node
*/

int		assign_type(char *lhs, t_ast_node *node)
{
	int		i;

	i = -1;
	while (++i <= TOTAL_PARSE_SYMS)
	{
		if (!ft_strcmp(lhs, g_sym_lookup[i].lhs))
		{
			node->type = g_sym_lookup[i].sym;
			return (SUCCESS);
		}
	}
	return (ERROR);
}

/*
** Reduces a given sequence of [non]terminals to the parent nonterminal
** specified by the handle, embeds this ast_node into a stack token to be pushed
** back onto the syntactic parse stack
*/

t_stack	*reduce_symbol(t_prod *handle, t_list **tmp)
{
	int			i;
	int			size;
	t_ast_node	*node;
	t_stack		*stack_token;

	if (!(node = (t_ast_node*)ft_memalloc(sizeof(t_ast_node))))
		return (NULL);
	i = 0;
	size = 0;
	while (handle->rhs && handle->rhs[size])
		size += 1;
	if (handle->rhs && (!(node->rhs = ft_strsum(handle->rhs, " "))
		|| !(node->val = (void**)ft_memalloc(sizeof(void*) * (size + 1)))))
		return (NULL);
	while (handle->rhs && i < size && ft_strcmp("$end", handle->rhs[i]))
		node->val[i++] = ((t_stack*)ft_lsttail(tmp)->content)->item.token;
	assign_type(handle->lhs, node);
	if (!(node->lhs = ft_strdup(handle->lhs))
		|| !(stack_token = (t_stack*)ft_memalloc(sizeof(t_stack))))
		return (NULL);
	stack_token->type = STACK_TOKEN;
	stack_token->item.token = node;
	return (stack_token);
}
