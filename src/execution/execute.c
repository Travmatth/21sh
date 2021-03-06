/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 17:59:38 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/13 16:37:26 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Execute node by detecting type and routing to appropriate exec function.
*/

int		execute_switch(t_exec_node *node)
{
	int		status;

	if (node->type == EXEC_PIPE)
		status = exec_pipe(node->node.pipe);
	else if (node->type == EXEC_AND || node->type == EXEC_OR)
		status = exec_logical(node->node.operator);
	else if (node->type == EXEC_SIMPLE_COMMAND)
		status = exec_simple_command(node->node.simple);
	else
		status = 1;
	return (status);
}

/*
** Execute programs by executing array of execution nodes
*/

int		execute(t_program *program)
{
	int		i;
	int		status;

	i = -1;
	status = SUCCESS;
	while (!ERR(status) && program->commands[++i])
		status = execute_switch(program->commands[i]);
	return (ERR(status) ? ERROR : SUCCESS);
}

/*
** Parse and execute commands
*/

int		parse_execute_input(char *complete_cmd)
{
	int			status;
	t_list		*tokens;
	t_ast		ast;
	t_program	program;

	tokens = NULL;
	ft_bzero(&ast, sizeof(t_ast));
	ft_bzero(&program, sizeof(t_program));
	status = lexical_analysis(complete_cmd, &tokens);
	if (OK(status))
		status = syntactic_analysis(&tokens, &ast);
	if (OK(status))
		status = semantic_analysis(&ast, &program);
	if (OK(status))
		status = execute(&program);
	free_tokens(&tokens);
	free_ast(&ast);
	free_program(&program);
	return (status);
}
