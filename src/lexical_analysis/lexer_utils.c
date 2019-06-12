/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 22:46:46 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Create a new blank to which lexical parsing can alter to store next
** token to be created.
*/

int		create_new_tok(t_token *token, t_lctx *ctx, int type)
{
	char	*buf;

	token->type = type;
	if (type == LEXER_WORD)
		ctx->in_word = TRUE;
	if (!(buf = ft_memalloc(sizeof(char) * (type == LEXER_WORD ? 10 : 4))))
		return (ERROR);
	if (!(token->value = ft_bufnew(buf, 0, sizeof(char) * 10)))
		return (ERROR);
	return (SUCCESS);
}

/*
** Append given character from command to the current token
*/

int		append_to_tok(char c, t_token *token)
{
	if (!ft_bufappend(token->value, &c, sizeof(char)))
		return (ERROR);
	return (SUCCESS);
}

/*
** Delete given token
*/

void	del_token(void *content, size_t len)
{
	t_stack	*node;

	(void)len;
	node = (t_stack*)content;
	free(node->item.token->val[0]);
	free(node->item.token);
	free(node);
}

/*
** 2.10.1.3 Shell Grammar Lexical Conventions
** If the string consists solely of digits and the delimiter character is
** one of '<' or '>', the token identifier IO_NUMBER shall be returned.
*/

void	detect_io_number(t_token *token)
{
	size_t	i;
	int		is_io_num;
	char	*str;

	i = 0;
	is_io_num = TRUE;
	str = token->value->buf;
	while (i < token->value->current)
	{
		if (str[i] < '0' || str[i] > '9')
		{
			is_io_num = FALSE;
			break ;
		}
		i += 1;
	}
	if (is_io_num)
		token->type = IO_NUMBER;
}
