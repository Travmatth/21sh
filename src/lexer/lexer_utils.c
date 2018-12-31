/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/30 17:58:19 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		remove_slash(char elem, size_t i, char *str, int *stop)
{
	int		out;

	(void)stop;
	out = 1;
	if (elem == '\\')
		out = IS_SEP(str[i + 1]) ? 0 : 1;
	return (out);
}

/*
** manage escaped characters in given arg
*/

void	expand_command(char **command)
{
	char	*var;
	char	*tmp;
	char	*cmd;

	while (ft_strchr(*command, '~'))
	{
		var = get_env_var("HOME");
		tmp = ft_swap(*command, "~", var);
		free(*command);
		*command = tmp;
	}
	while ((tmp = ft_strchr(*command, '$')))
	{
		cmd = ft_strchr(tmp, ' ') ? ft_strchr(tmp, ' ') : ft_strchr(tmp, '\0');
		tmp = ft_strsub(tmp, 0, cmd - tmp);
		var = get_env_var(tmp + 1);
		cmd = ft_swap(*command, tmp, var ? var : "");
		free(*command);
		free(tmp);
		*command = cmd;
	}
}

/*
** find the next token in the complete_command
*/

int		escaped(char *input, size_t i)
{
	if (i == 0)
		return (FALSE);
	else if (input[i - 1] == '\\')
		return (TRUE);
	return (FALSE);
}

int		init_lexer_ctx(char *input, t_lctx *ctx, t_token *token)
{
	if (!input)
		return (ERROR);
	ctx->input = input;
	ctx->status = SUCCESS;
	ctx->i = 0;
	ctx->j = 0;
	ctx->stop = 0;
	ctx->op_state = 0;
	ctx->in_word = 0;
	ctx->missing = NULL;
	token->type = NIL;
	token->value = NULL;
	return (SUCCESS);
}

int		append_to_tok(char c, t_token *token)
{
	if (!ft_bufappend(token->value, &c, sizeof(char)))
		return (ERROR);
	return (SUCCESS);
}

int		create_new_tok(t_token *token, t_lctx *ctx, int type)
{
	char	*buf;

	token->type = type;
	if (type == WORD)
		ctx->in_word = TRUE;
	if (!(buf = ft_memalloc(sizeof(char) * (type == WORD ? 10 : 4))))
		return (ERROR);
	if (!(token->value = ft_bufnew(buf, 0, sizeof(char) * 10)))
		return (ERROR);
	return (SUCCESS);
}

int		push_token(t_token *token, t_list *node, t_list **tokens, t_lctx *ctx)
{
	if (!(node = ft_lstnew(token, sizeof(t_token))))
		return (ERROR);
	ft_lstpushback(tokens, node);
	ctx->in_word = FALSE;
	ctx->op_state = FALSE;
	token->type = NIL;
	token->value = NULL;
	return (SUCCESS);
}

int		next_missing_symbol(t_list *missing)
{
	t_list	*node;
	int		type;

	if (!(node = ft_lstpeekhead(missing)))
		return (NIL);
	type = *(short*)node->content;
	return (type);
}

int		push_missing_symbol(short type, t_list **missing)
{
	t_list	*node;

	if (!(node = ft_lstnew(&type, sizeof(short))))
		return (ERROR);
	ft_lstadd(missing, node);
	return (SUCCESS);
}

int		pop_missing_symbol(t_list **missing)
{
	t_list	*node;

	node = ft_lsthead(missing);
	free(node->content);
	free(node);
	return (SUCCESS);
}
