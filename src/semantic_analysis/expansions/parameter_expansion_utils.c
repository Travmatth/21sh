/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:51:19 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/25 16:07:14 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

/*
** Determine ending brace for given parameter expansion, allowing for nested
** parameter expansions
*/

char	*find_end_brace(char *param)
{
	int		count;
	int		prefixed;
	size_t	i;

	i = -1;
	count = 0;
	prefixed = FALSE;
	param = &param[2];
	while (param[++i])
	{
		if (!count && param[i] == '}')
			return (&param[i]);
		else if (count && param[i] == '}')
			count -= 1;
		else if (prefixed && param[i] == '{')
			count += 1;
		else if (prefixed && param[i] != '$')
			prefixed = FALSE;
	}
	return (NULL);
}

/*
** parse parameter expansion syntax and identify full param to be replaced
** name of param and word to be substituted if given
*/

void	init_plain_exp(char *param[6], size_t len[4], char *str)
{
	size_t	i;
	int		test;

	i = 0;
	test = str[1] == '{' ? TRUE : FALSE;
	while (IS_VAR_CHAR(str[i + (test == TRUE ? 2 : 1)]))
		i += 1;
	len[FULL_PARAM] = i + (test == TRUE ? 3 : 1);
	param[FULL_PARAM] = str;
	len[NAME] = i;
	param[NAME] = &str[test == TRUE ? 2 : 1];
	len[WORD] = 0;
	param[WORD] = NULL;
	len[TEST] = FALSE;
}

int		init_param_state(char *param[6], size_t len[4], char sep, char *str)
{
	int		test;

	if (NONE(sep))
		init_plain_exp(param, len, str);
	else
	{
		test = ft_strchr(str, ':') ? TRUE : FALSE;
		len[TEST] = test;
		param[FULL_PARAM] = str;
		len[FULL_PARAM] = find_end_brace(str) - &str[1];
		param[NAME] = &str[2];
		len[NAME] = ft_strchr(str, test ? ':' : sep) - param[NAME];
		param[WORD] = &ft_strchr(str, sep)[1];
		len[WORD] = find_end_brace(str) - param[WORD];
	}
	return (SUCCESS);
}

/*
** parse parameter and copy full param, name and word values
*/

int		create_param_state(char *param[3], size_t len[3])
{
	if (!(param[FULL_PARAM] = ft_strndup(param[FULL_PARAM], len[FULL_PARAM]))
		|| !(param[NAME] = ft_strndup(param[NAME], len[NAME])))
		return (ERROR);
	if (param[WORD] && !(param[WORD] = ft_strndup(param[WORD], len[WORD])))
		return (ERROR);
	return (SUCCESS);
}

/*
** Detect if given param is enclose by braces
*/

int		enclosed(char *str, char c)
{
	char	*occurs;
	char	*close;

	occurs = ft_strchr(str, c);
	if (!(occurs))
		return (NIL);
	close = ft_strchr(str, '}');
	return (occurs < close);
}
