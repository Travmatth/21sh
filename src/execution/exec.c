/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/06 14:35:53 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		builtin_command(char **argv)
{
	int		i;
	int		ac;

	i = 0;
	while (i < 5)
	{
		if (ft_strnequ(g_builtins[i].cmd, argv[0], g_builtins[i].len))
		{
			ac = 0;
			while (argv[ac])
				ac += 1;
			i = g_builtins[i].f(ac, argv);
			ac -= 1;
			return (i);
		}
		i += 1;
	}
	return (0);
}

int		execute_command(char **av)
{
	int		status;
	pid_t	pid;

	status = 0;
	if (NONE((pid = fork())))
	{
		execve(av[0], av, g_environ);
		ft_printf("fork error: %s", av[0]);
		_exit(1);
	}
	else if (OK(pid))
		waitpid(pid, &status, NIL);
	else if (ERR(pid))
		ft_printf("fork error: %s", av[0]);
	return (WIFEXITED(status) ? WEXITSTATUS(status) : -1);
}

int		execute(t_program *program)
{
	(void)program;
	return (SUCCESS);
}

/*
** -exec p (char*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)
** ((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)
** stack_token->item.token->val[0])->val[0])->val[0])->val[0])->val[0])
** ->val[0])->val[0])->val[0])->val[0])->val[0]
** (char *) $34 = 0x0000602000002210 "cat"
** -exec p (char*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)
** ((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)((t_ast_node*)
** stack_token->item.token->val[0])->val[0])->val[0])->val[0])->val[0])
** ->val[0])->val[0])->val[1])->val[0])->val[0]
** (char *) $35 = 0x00006020000022b0 "author"
*/

int		execute_commands(char *complete_cmd)
{
	int			status;
	t_list		*tokens;
	t_ast		ast;
	t_program	program;

	tokens = NULL;
	ft_bzero(&ast, sizeof(t_ast));
	ft_bzero(&program, sizeof(t_program));
	if (!OK((status = lexical_analysis(complete_cmd, &tokens, &ast.missing)))
		|| (!OK((status = syntactic_analysis(&tokens, &ast))))
		|| (!OK((status = semantic_analysis(&ast, &program))))
		|| (!OK((status = execute(&program)))))
		return (status);
	return (status);
}
