/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/30 17:03:37 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		parse_commands(t_list *tokens, t_ast *ast)
{
	(void)tokens;
	(void)ast;
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
	return (parse_commands(tokens, ast));
}
