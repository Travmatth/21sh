/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/01 16:29:41 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** use the grammar rules in
** http://pubs.opengroup.org/onlinepubs/009695399
** /utilities/xcu_chap02.html#tag_02_10_02
** to parse the tokens into an ast
*/

int		parse_commands(char **tokens, t_ast *ast)
{
	(void)tokens;
	(void)ast;
	return (ERROR);
}

int		prepare_ast(char *complete_cmd, t_ast *ast, int i)
{
	int		status;
	char	**tokens;

	(void)i;
	ft_bzero(ast, sizeof(t_ast));
	if (!OK((status = lexer(complete_cmd, &tokens)))
		|| !OK((status = parse_commands(tokens, ast))))
		return (ERR(status) ? ERROR : NIL);
	return (SUCCESS);
}
