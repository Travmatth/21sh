/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/02 23:28:14 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** parse given tokens into an ast
*/

int		parse_commands(char **tokens, t_ast *ast)
{
	(void)tokens;
	(void)ast;
	return (SUCCESS);
}

/*
** 
** use the grammar rules in
** http://pubs.opengroup.org/onlinepubs/009695399
** /utilities/xcu_chap02.html#tag_02_10_02
*/

int		prepare_ast(char *complete_cmd, t_ast *ast)
{
	int		status;
	char	**tokens;

	ft_bzero(ast, sizeof(t_ast));
	if (!OK((status = lexer(complete_cmd, &tokens)))
		|| !OK((status = parse_commands(tokens, ast))))
		return (ERR(status) ? ERROR : NIL);
	return (SUCCESS);
}
