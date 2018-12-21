/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/19 15:37:13 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** use the grammar rules in
** http://pubs.opengroup.org/onlinepubs/009695399
** /utilities/xcu_chap02.html#tag_02_10_02
** to parse the tokens into an ast
*/

/*
** if open quote is detected
** prompt user for more input that will close the quote
*/

int		close_quote_prompt(char *complete_cmd, size_t i, size_t *tok_count)
{
	(void)complete_cmd;
	(void)i;
	(void)tok_count;
	return (SUCCESS);
}

int		parse_commands(char **tokens, t_ast *ast)
{
	(void)tokens;
	(void)ast;
	return (ERROR);
}

int		prepare_ast(char *complete_cmd, t_ast *ast)
{
	int		status;
	char	**tokens;

	ft_bzero(ast, sizeof(t_ast));
	if (!OK((status = lexical_analysis(complete_cmd, &tokens)))
		|| !OK((status = parse_commands(tokens, ast))))
		return (ERR(status) ? ERROR : NIL);
	return (SUCCESS);
}
