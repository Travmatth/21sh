/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/04 17:31:57 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		p_pipeseq(char **tokens, t_ast *ast, int *i)
{
	int		status;

	if (p_command(tokens, ast, i))
		return (SUCCESS);
	if (OK((status = p_pipeseq(tokens, ast, i))))
	{
		status = ERROR;
		if (tokens[*i] != '|'
			&& OK((status = p_linebreak(tokens, ast, i)))
			&& OK((status = p_command(tokens, ast, i)))))
			return (SUCCESS);
		return (status);
		
	}
	return (p_pipeseq(tokens, ast, i));
}

int		p_pipeline(char **tokens, t_ast *ast, int *i)
{
	if (tokens[*i] == '!')
		*i += 1;
	return (p_pipeseq(tokens, ast, i));
}

int		p_andor(char **tokens, t_ast *ast, int *i)
{
	int		status;

	if (OK((status = p_pipeline(tokens, ast, i))))
		return (SUCCESS);
	else if (NONE(status) && OK((status = p_andor(tokens, ast, i))))
	{
		if (!ft_strequ("&&", tokens[*i]) || !ft_strequ("||", tokens[*i]))
			return (ERROR);
		if (OK((status = p_linebreak(tokens, ast, i)))
			&& OK(status = p_pipeline(tokens, ast, i)))
			return (SUCCESS);
	}
	return (status);
}

int		p_list(char **tokens, t_ast *ast, int *i)
{
	int		status;

	if (OK((status = p_andor(tokens, ast, i))))
		return (SUCCESS);
	else if (NONE(status) && OK((status = p_list(tokens, ast, i)))
		&& OK(status = p_separator(tokens, ast, i))
		&& OK(status = p_andor(tokens, ast, i)))
		return (SUCCESS);
	return (status);
}

int		p_commands(char **tokens, t_ast *ast, int *i)
{
	int		status;

	i = 0;
	if (ERR((status = p_list(tokens, ast, i))))
		return (ERROR);
	if (OK(status) && tokens[*i] && OK((status = p_separator(tokens, ast, i))))
		return (p_list(tokens, ast, i));
	return (status);
}

/*
** 
** use the grammar rules in
** http://pubs.opengroup.org/onlinepubs/009695399
** /utilities/xcu_chap02.html#tag_02_10_02
*/

int		prepare_ast(char *complete_cmd, t_ast *ast)
{
	int		i;
	int		status;
	char	**tokens;

	ft_bzero(ast, sizeof(t_ast));
	if (!OK((status = lexer(complete_cmd, &tokens)))
		|| !OK((status = p_commands(tokens, ast, &i))))
		return (ERR(status) ? ERROR : NIL);
	return (SUCCESS);
}
